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

#include "config.h"
#include "config_reader.h"
#include "event_ids.h"
#include "filesystem.h"
#include "gpio_pins.h"
#include "http_server.h"
#include "keyboard.h"
#include "led_controller.h"
#include "spotify.h"
#include "ui_task.h"
#include "usb_device.h"
#include "usb_hid.h"
#include "wifi.h"

namespace {

constexpr char TAG[] = "kbd_app";

constexpr uint32_t kMainLoopWaitMSecs = 1000;
// Interrupt allocation flags.
// Combination of  ESP_INTR_FLAG_* flags.
constexpr int ESP_INTR_FLAG_DEFAULT = 0x0;  // No flags set.

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

App::App() : config_(new Config()) {
  g_app = this;
}

App::~App() {
  g_app = nullptr;
  if (event_group_)
    vEventGroupDelete(event_group_);
}

esp_err_t App::SetTimezone() {
  if (config_->time.timezone.empty()) {
    ESP_LOGE(TAG, "No timezone");
    return ESP_FAIL;
  }
  ESP_LOGI(TAG, "Setting timezone to %s", config_->time.timezone.c_str());

  setenv("TZ", config_->time.timezone.c_str(), 1);
  tzset();

  return ESP_OK;
}

// static
void App::SNTPSyncEventHandler(struct timeval* tv) {}

/**
 * Initialize SNTP and get the current time.
 *
 * Call once online.
 */
esp_err_t App::InitializSNTP() {
  if (config_->time.ntp_server.empty()) {
    ESP_LOGE(TAG, "No NTP server");
    return ESP_FAIL;
  }
  ESP_LOGI(TAG, "Initializing SNTP using server: %s",
           config_->time.ntp_server.c_str());
  sntp_setoperatingmode(SNTP_OPMODE_POLL);
  sntp_setservername(0, config_->time.ntp_server.c_str());
  sntp_set_time_sync_notification_cb(SNTPSyncEventHandler);
  sntp_set_sync_mode(SNTP_SYNC_MODE_SMOOTH);
  sntp_init();
  sntp_initialized_ = true;
  return ESP_OK;
}

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
      ESP_LOGI(TAG, "Wi-Fi is connected.");
      app->online_ = true;
      if (!app->sntp_initialized_)
        app->InitializSNTP();
    } else if (bits & EVENT_NETWORK_DISCONNECTED) {
      ESP_LOGW(TAG, "Wi-Fi connection failed.");
      app->online_ = false;
      // TODO: Set a timer so that we can retry in a little while.
    }
    if (bits & EVENT_SPOTIFY_GOT_AUTHORIZATION_CODE) {
      ESP_LOGI(TAG, "Have authorization code");
    }
    if (bits & EVENT_SPOTIFY_ACCESS_TOKEN_GOOD) {
      ESP_LOGI(TAG, "Have access token");
    }
    if (bits & EVENT_SPOTIFY_ACCESS_TOKEN_EXPIRE) {
      ESP_LOGI(TAG, "Access token needs refresh");
      app->spotify_need_access_token_refresh_ = true;
    }
    if (bits & EVENT_KEYBOARD_EVENT) {
      app->keyboard_->HandleEvents();
    }
  }
}

esp_err_t App::CreateKeyboardSimulatorTask() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;
  return xTaskCreate(KeyboardSimulatorTask, "usb-test", kStackDepthWords, this,
                     tskIDLE_PRIORITY, &main_task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

// static
void IRAM_ATTR App::KeyboardSimulatorTask(void* arg) {
  // App* app = static_cast<App*>(arg);
  ESP_LOGW(TAG, "In USB keyboard simulator task.");
#if 0
  const std::string kTypedString = "Super Display Keyboard. ";
  int idx = 0;
#endif
  while (true) {
    if (usb::Device::Suspended()) {
      if (usb::Device::RemoteWakup() != ESP_OK)
        ESP_LOGE(TAG, "Error waking up");
      vTaskDelay(pdMS_TO_TICKS(500));
      continue;
    }
    if (!usb::HID::Ready()) {
      ESP_LOGV(TAG, "USB not ready.");
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }
    if (usb::Device::Mounted()) {
#if 0
      ESP_LOGD(TAG, "Mounted, sending keyboard event.");

      usb::HID::KeyboardPress(REPORT_ID_KEYBOARD, kTypedString[idx++]);
      if (idx == kTypedString.length())
        idx = 0;

      // For some reason a delay is required. Without one the keyboard release
      // doesn't seem to work and auto-repeats forever. 5ms was impirically
      // found to be short enough to prevent auto-repeat.
      ets_delay_us(5000);
      usb::HID::KeyboardRelease(REPORT_ID_KEYBOARD);
#endif
      vTaskDelay(pdMS_TO_TICKS(2000));
    } else {
      ESP_LOGD(TAG, "USB not mounted");
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}

esp_err_t App::CreateUSBTask() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  return xTaskCreate(USBTask, "usb-tick", kStackDepthWords, this,
                     tskIDLE_PRIORITY + 1, &main_task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

// static
void IRAM_ATTR App::USBTask(void* arg) {
  // App* app = static_cast<App*>(arg);
  while (true) {
    usb::Device::Tick();
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

// static
void IRAM_ATTR App::KeyboardISR(void* arg) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  BaseType_t xResult = xEventGroupSetBitsFromISR(
      static_cast<App*>(arg)->event_group_, EVENT_KEYBOARD_EVENT,
      &xHigherPriorityTaskWoken);

  if (xResult != pdFAIL) {
    // See https://www.freertos.org/xEventGroupSetBitsFromISR.html
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

esp_err_t App::InstallKeyboardISR() {
  constexpr uint64_t pin_mask = 1ULL << kKeyboardINTGPIO;
  constexpr gpio_config_t io_conf = {
      .pin_bit_mask = pin_mask,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_ANYEDGE,
  };

  esp_err_t err = gpio_config(&io_conf);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Unable to config keyboard INT pin: %s.",
             esp_err_to_name(err));
    return err;
  }

  err = gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "gpio_install_isr_service failure: %s.",
             esp_err_to_name(err));
    return err;
  }

  err = gpio_isr_handler_add(kKeyboardINTGPIO, KeyboardISR, this);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "gpio_isr_handler_add failure: %s.", esp_err_to_name(err));
    return err;
  }
  ESP_LOGI(TAG, "Keyboard interrupt handler installed on GPIO %u.",
           kKeyboardINTGPIO);
  return ESP_OK;
}

esp_err_t App::InitializeI2C() {
  constexpr i2c::Master::InitParams i2c_0_config = {
      .i2c_bus = I2C_NUM_0,
      .sda_gpio = kI2C0SDA,
      .scl_gpio = kI2C0SCL,
      .clk_speed = 400000,
      .sda_pullup_enable = false,
      .scl_pullup_enable = false,
  };
  if (!i2c::Master::Initialize(i2c_0_config))
    return ESP_FAIL;

  constexpr i2c::Master::InitParams i2c_1_config = {
      .i2c_bus = I2C_NUM_1,
      .sda_gpio = kI2C1SDA,
      .scl_gpio = kI2C1SCL,
      .clk_speed = 400000,
      .sda_pullup_enable = false,
      .scl_pullup_enable = false,
  };
  if (!i2c::Master::Initialize(i2c_1_config))
    return ESP_FAIL;

  return ESP_OK;
}

esp_err_t App::Initialize() {
  esp_err_t err = InitNVRAM();
  if (err != ESP_OK)
    return err;

  err = InitializeI2C();
  if (err != ESP_OK)
    return err;

  err = esp_event_loop_create_default();
  if (err != ESP_OK)
    return err;

  fs_.reset(new Filesystem());
  err = fs_->Initialize();
  if (err != ESP_OK)
    return err;

  ConfigReader config_reader;
  err = config_reader.Read(config_.get());
  if (err != ESP_OK)
    return err;

  led_controller_.reset(new LEDController(kActivityGPIO));

  SetTimezone();  // Ignore return value - not critical.

  ESP_LOGI(TAG, "Wi-Fi SSID: \"%s\"", config_->wifi.ssid.c_str());
  event_group_ = xEventGroupCreate();
  wifi_.reset(new WiFi(event_group_));
  err = CreateAppEventTask();
  if (err != ESP_OK)
    return err;

  err = CreateUSBTask();
  if (err != ESP_OK)
    return err;

#if 0
  keyboard_.reset(new Keyboard(i2c::Master(I2C_NUM_0, /*mutex=*/nullptr)));
  err = keyboard_->Initialize();
  if (err != ESP_OK)
    return err;
#endif

  err = wifi_->Inititialize();
  if (err != ESP_OK)
    return err;

  err = wifi_->Connect(config_->wifi.ssid, config_->wifi.key);
  if (err != ESP_OK)
    return err;

  err = usb::Device::Initialize();
  if (err != ESP_OK)
    return err;

  https_server_.reset(new HTTPServer());  // Initialize once online.

  spotify_.reset(new Spotify(config_.get(), https_server_.get(), wifi_.get(),
                             event_group_));

  err = UITask::Start();
  if (err != ESP_OK)
    return err;

  CreateKeyboardSimulatorTask();
  if (err != ESP_OK)
    return err;

  err = InstallKeyboardISR();
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
