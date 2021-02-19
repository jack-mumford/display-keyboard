#include "app.h"

#include <utility>

#include <time.h>

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG

#include <class/hid/hid.h>
#include <esp_log.h>
#include <esp_sntp.h>
#include <esp_spi_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2clib/master.h>
#include <nvs_flash.h>

#include "config_reader.h"
#include "event_ids.h"
#include "filesystem.h"
#include "gpio_pins.h"
#include "http_server.h"
#include "keyboard_simulator_task.h"
#include "keyboard_task.h"
#include "led_controller.h"
#include "spotify.h"
#include "ui_task.h"
#include "usb_device.h"
#include "usb_hid.h"
#include "usb_task.h"

#include "wifi.h"

namespace {

constexpr char TAG[] = "kbd_app";

constexpr uint32_t kMainLoopWaitMSecs = 1000;

// Make sure min wait time is at least one tick.
static_assert((kMainLoopWaitMSecs / portTICK_PERIOD_MS) > 0);

App* g_app;

esp_err_t InitNVRAM() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  return ret;
}

}  // namespace

App::App()
    : filesystem_(new Filesystem()),
      https_server_(new HTTPServer()),
      led_controller_(new LEDController(kActivityGPIO)),
      event_group_(xEventGroupCreate()),
      wifi_(new WiFi(event_group_)),
      spotify_(new Spotify(&config_,
                           https_server_.get(),
                           wifi_.get(),
                           event_group_)) {
  g_app = this;
}

App::~App() {
  g_app = nullptr;
  if (event_group_)
    vEventGroupDelete(event_group_);
}

esp_err_t App::SetTimezone() {
  if (config_.time.timezone.empty()) {
    ESP_LOGE(TAG, "No timezone");
    return ESP_FAIL;
  }
  ESP_LOGD(TAG, "Setting timezone: %s", config_.time.timezone.c_str());

  setenv("TZ", config_.time.timezone.c_str(), 1);
  tzset();

  return ESP_OK;
}

// static
void App::SNTPSyncEventHandler(struct timeval* tv) {}

esp_err_t App::CreateAppEventTask() {
  // https://www.freertos.org/FAQMem.html#StackSize
  // TODO: Reduce this size and take the HTTPS requtest out of this task.
  constexpr uint32_t kStackDepthWords = 2048;

  BaseType_t task = xTaskCreate(AppEventTask, "app-event", kStackDepthWords,
                                this, tskIDLE_PRIORITY, &main_task_);
  if (task != pdPASS)
    return ESP_FAIL;
  return ESP_OK;
}

// static:
void IRAM_ATTR App::AppEventTask(void* arg) {
  App* app = static_cast<App*>(arg);
  ESP_LOGW(TAG, "In Wi-Fi status task handler.");
  while (true) {
    EventBits_t bits = xEventGroupWaitBits(app->event_group_, EVENT_ALL,
                                           pdFALSE, pdFALSE, portMAX_DELAY);
    xEventGroupClearBits(app->event_group_, EVENT_ALL);
    if (bits & EVENT_NETWORK_GOT_IP) {
      ESP_LOGD(TAG, "Wi-Fi is connected.");
      app->online_ = true;
      if (!app->sntp_initialized_)
        app->InitializSNTP();
    } else if (bits & EVENT_NETWORK_DISCONNECTED) {
      ESP_LOGW(TAG, "Wi-Fi connection failed.");
      app->online_ = false;
      // TODO: Set a timer so that we can retry in a little while.
    }
    if (bits & EVENT_SPOTIFY_GOT_AUTHORIZATION_CODE) {
      ESP_LOGD(TAG, "Have authorization code");
    }
    if (bits & EVENT_SPOTIFY_ACCESS_TOKEN_GOOD) {
      ESP_LOGD(TAG, "Have access token");
    }
    if (bits & EVENT_SPOTIFY_ACCESS_TOKEN_EXPIRE) {
      ESP_LOGD(TAG, "Access token needs refresh");
      app->spotify_need_access_token_refresh_ = true;
    }
  }
}

/**
 * Initialize SNTP and get the current time.
 *
 * Call once online.
 */
esp_err_t App::InitializSNTP() {
  if (config_.time.ntp_server.empty()) {
    ESP_LOGE(TAG, "No NTP server");
    return ESP_FAIL;
  }
  ESP_LOGD(TAG, "Initializing SNTP using server: %s",
           config_.time.ntp_server.c_str());
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, config_.time.ntp_server.c_str());
  sntp_set_time_sync_notification_cb(SNTPSyncEventHandler);
  sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
  sntp_init();
  sntp_initialized_ = true;
  return ESP_OK;
}

esp_err_t App::InitializeI2C() {
  // The volume display (SSD1306) is on port 0. It supports 100kHz and 400kHz.
  constexpr i2c::Master::InitParams i2c_0_config = {
      .i2c_bus = I2C_NUM_0,
      .sda_gpio = kI2C0_SDA_GPIO,
      .scl_gpio = kI2C0_SCL_GPIO,
      .clk_speed = 400000,
      .sda_pullup_enable = false,
      .scl_pullup_enable = false,
  };
  if (!i2c::Master::Initialize(i2c_0_config))
    return ESP_FAIL;

  // The keyboard IC (LM8330) is on port 1. It supports 100kHz and 400kHz.
  constexpr i2c::Master::InitParams i2c_1_config = {
      .i2c_bus = I2C_NUM_1,
      .sda_gpio = kI2C1_SDA_GPIO,
      .scl_gpio = kI2C1_SCL_GPIO,
      .clk_speed = 100000,
      .sda_pullup_enable = false,
      .scl_pullup_enable = false,
  };
  if (!i2c::Master::Initialize(i2c_1_config))
    return ESP_FAIL;

  return ESP_OK;
}

esp_err_t App::Initialize() {
  if (!event_group_)
    return ESP_FAIL;

  esp_err_t err = InitNVRAM();
  if (err != ESP_OK)
    return err;

  err = InitializeI2C();
  if (err != ESP_OK)
    return err;

  err = esp_event_loop_create_default();
  if (err != ESP_OK)
    return err;

  err = filesystem_->Initialize();
  if (err != ESP_OK)
    return err;

  ConfigReader config_reader;
  err = config_reader.Read(&config_);
  if (err != ESP_OK)
    return err;

  ESP_ERROR_CHECK_WITHOUT_ABORT(SetTimezone());

  err = led_controller_->Initialize();
  if (err != ESP_OK)
    return err;

  ESP_LOGI(TAG, "Wi-Fi SSID: \"%s\"", config_.wifi.ssid.c_str());
  err = CreateAppEventTask();
  if (err != ESP_OK)
    return err;

  err = USBTask::Start();
  if (err != ESP_OK)
    return err;

  err = KeyboardTask::Start();
  if (err != ESP_OK)
    return err;

  // Just for testing.
  KeyboardSimulatorTask::Start();

  err = wifi_->Inititialize();
  if (err != ESP_OK)
    return err;

  err = wifi_->InitiateConnection(config_.wifi.ssid, config_.wifi.key);
  if (err != ESP_OK)
    return err;

  err = UITask::Start();
  if (err != ESP_OK)
    return err;

  return ESP_OK;
}

void App::Run() {
  while (true) {
    if (online_) {
      if (!spotify_->initialized()) {
        ESP_ERROR_CHECK_WITHOUT_ABORT(spotify_->Initialize());
        std::string auth_start_url = spotify_->GetAuthStartURL();
        ESP_LOGI(TAG, "To login to Spotify navigate to %s",
                 auth_start_url.c_str());
      }

      if (spotify_->initialized()) {
        if (spotify_->HaveAuthorizatonCode()) {
          ESP_LOGD(TAG, "Got authorization code, getting token.");
          ESP_ERROR_CHECK_WITHOUT_ABORT(spotify_->ContinueLogin());
        } else if (spotify_need_access_token_refresh_) {
          spotify_need_access_token_refresh_ = false;
          spotify_->RefreshAccessToken();
        } else if (!started_spotify_currently_playing_ &&
                   spotify_->HaveAccessToken()) {
          ESP_LOGD(TAG, "Getting Spotify currently playing info.");
          started_spotify_currently_playing_ = true;
          ESP_ERROR_CHECK_WITHOUT_ABORT(spotify_->GetCurrentlyPlaying());
        }
      }
    }
    // Need to use vTaskDelay to avoid triggering the task WDT.
    vTaskDelay(pdMS_TO_TICKS(kMainLoopWaitMSecs));
    taskYIELD();  // Not sure if this is necessary.
  }
}
