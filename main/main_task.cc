#include "main_task.h"

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include <esp_log.h>
#include <esp_sntp.h>
#include <i2clib/bus.h>
#include <nvs_flash/include/nvs_flash.h>

#include "config_reader.h"
#include "event_ids.h"
#include "gpio_pins.h"
#include "keyboard_simulator_task.h"
#include "keyboard_task.h"
#include "ui_task.h"
#include "usb_device.h"
#include "usb_hid.h"
#include "usb_task.h"
#include "volume_task.h"

namespace {

constexpr char TAG[] = "MainTask";

MainTask* g_main_task;

esp_err_t InitNVRAM() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  return ret;
}

// static
void SNTPSyncEventHandler(struct timeval* tv) {
  ESP_LOGD(TAG, "NTP sync complete.");
}

}  // namespace

MainTask::MainTask()
    : led_controller_(kActivityGPIO),
      event_group_(xEventGroupCreate()),
      wifi_(event_group_),
      spotify_(&config_, &https_server_, &wifi_, event_group_) {}

MainTask::~MainTask() {
  if (event_group_)
    vEventGroupDelete(event_group_);
  g_main_task = nullptr;
}

esp_err_t MainTask::SetTimezone() {
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
esp_err_t MainTask::Start() {
  ESP_LOGD(TAG, "Starting Main task");
  if (g_main_task)
    return ESP_FAIL;

  g_main_task = new MainTask();
  return g_main_task->Initialize();
}

/**
 * Initialize SNTP and get the current time.
 *
 * Call once online.
 */
esp_err_t MainTask::InitializSNTP() {
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

// static
esp_err_t MainTask::InitializeI2C() {
  // Used for Keyboard.
  constexpr i2c::Bus::InitParams i2c_0_config = {
      .i2c_bus = I2C_NUM_0,
      .sda_gpio = kI2C0_SDA_GPIO,
      .scl_gpio = kI2C0_SCL_GPIO,
      .clk_speed = 400'000,
      .sda_pullup_enable = false,
      .scl_pullup_enable = false,
  };
  if (!i2c::Bus::Initialize(i2c_0_config)) {
    ESP_LOGE(TAG, "Can't initialize I2C0");
    return ESP_FAIL;
  }

// Initialize if not using tarablessd1306 for volume display.
#ifndef CONFIG_SSD1306_ENABLE_DEFAULT_I2C_INTERFACE
  // Used for volume display.
  constexpr i2c::Bus::InitParams i2c_1_config = {
      .i2c_bus = I2C_NUM_1,
      .sda_gpio = kI2C1_SDA_GPIO,
      .scl_gpio = kI2C1_SCL_GPIO,
      .clk_speed = 400'000,
      .sda_pullup_enable = false,
      .scl_pullup_enable = false,
  };
  if (!i2c::Bus::Initialize(i2c_1_config)) {
    ESP_LOGE(TAG, "Can't initialize I2C1");
    return ESP_FAIL;
  }
#endif  // CONFIG_SSD1306_ENABLE_DEFAULT_I2C_INTERFACE

  return ESP_OK;
}

esp_err_t MainTask::Initialize() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 3 * 1024;

  esp_err_t err;

  if (!event_group_)
    return ESP_FAIL;

  err = InitNVRAM();
  if (err != ESP_OK)
    return err;

  err = UITask::Start();
  if (err != ESP_OK)
    return err;

  err = InitializeI2C();
  if (err != ESP_OK)
    return err;

  err = esp_event_loop_create_default();
  if (err != ESP_OK)
    return err;

  err = filesystem_.Initialize();
  if (err != ESP_OK)
    return err;

  ConfigReader config_reader;
  err = config_reader.Read(&config_);
  if (err != ESP_OK)
    return err;
  ESP_LOGI(TAG, "Wi-Fi SSID: \"%s\"", config_.wifi.ssid.c_str());

  ESP_ERROR_CHECK_WITHOUT_ABORT(SetTimezone());

  err = led_controller_.Initialize();
  if (err != ESP_OK)
    return err;

  err = USBTask::Start();
  if (err != ESP_OK)
    return err;

  err = KeyboardTask::Start();
  if (err != ESP_OK)
    return err;

#if 0
  // Just for testing.
  KeyboardSimulatorTask::Start();
#endif

  err = VolumeTask::Start();
  if (err != ESP_OK)
    return err;

  err = wifi_.Inititialize();
  if (err != ESP_OK)
    return err;

  err = wifi_.InitiateConnection(config_.wifi.ssid, config_.wifi.key);
  if (err != ESP_OK)
    return err;

  return xTaskCreate(TaskFunc, TAG, kStackDepthWords, this,
                     tskIDLE_PRIORITY + 1, &task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

// TODO: Clean this up. Make Spotify manage it's state and
// this task is only informed of that state.
void MainTask::UpdateSpotify() {
  if (!online_)
    return;
  if (!spotify_.initialized()) {
    ESP_ERROR_CHECK_WITHOUT_ABORT(spotify_.Initialize());
    std::string auth_start_url = spotify_.GetAuthStartURL();
    ESP_LOGI(TAG, "To login to Spotify navigate to %s", auth_start_url.c_str());
  }

  if (spotify_.initialized()) {
    if (spotify_.HaveAuthorizatonCode()) {
      ESP_LOGD(TAG, "Got authorization code, getting token.");
      ESP_ERROR_CHECK_WITHOUT_ABORT(spotify_.ContinueLogin());
    } else if (spotify_need_access_token_refresh_) {
      spotify_need_access_token_refresh_ = false;
      spotify_.RefreshAccessToken();
    } else if (!started_spotify_currently_playing_ &&
               spotify_.HaveAccessToken()) {
      ESP_LOGD(TAG, "Getting Spotify currently playing info.");
      started_spotify_currently_playing_ = true;
      ESP_ERROR_CHECK_WITHOUT_ABORT(spotify_.GetCurrentlyPlaying());
    }
  }
}

void IRAM_ATTR MainTask::Run() {
  ESP_LOGW(TAG, "In Wi-Fi status task handler.");
  while (true) {
    EventBits_t bits =
        xEventGroupWaitBits(event_group_, EVENT_ALL, /*xClearOnExit=*/pdTRUE,
                            /*xWaitForAllBits=*/pdFALSE, portMAX_DELAY);
    if (bits & EVENT_NETWORK_GOT_IP) {
      ESP_LOGD(TAG, "Wi-Fi is connected.");
      online_ = true;
      UITask::SetWiFiStatus(WiFiStatus::Online);
      if (!sntp_initialized_)
        InitializSNTP();
      UpdateSpotify();
    } else if (bits & EVENT_NETWORK_DISCONNECTED) {
      ESP_LOGW(TAG, "Wi-Fi connection failed.");
      online_ = false;
      UITask::SetWiFiStatus(WiFiStatus::Offline);
      // TODO: Set a timer so that we can retry in a little while.
    }
    if (bits & EVENT_SPOTIFY_GOT_AUTHORIZATION_CODE) {
      ESP_LOGD(TAG, "Got authorization code, getting token.");
      UpdateSpotify();
    }
    if (bits & EVENT_SPOTIFY_ACCESS_TOKEN_GOOD) {
      ESP_LOGD(TAG, "Have access token");
      UpdateSpotify();
    }
    if (bits & EVENT_SPOTIFY_ACCESS_TOKEN_EXPIRE) {
      ESP_LOGD(TAG, "Access token needs refresh");
      UpdateSpotify();
    }
  }
}

// static
void IRAM_ATTR MainTask::TaskFunc(void* arg) {
  static_cast<MainTask*>(arg)->Run();
}
