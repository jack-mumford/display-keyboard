#include "app.h"

#include <utility>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <class/hid/hid.h>
#include <esp_log.h>
#include <esp_spi_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2clib/master.h>
#include <nvs_flash.h>

#include "config.h"
#include "config_reader.h"
#include "display.h"
#include "filesystem.h"
#include "https_server.h"
#include "keyboard.h"
#include "led_controller.h"
#include "spotify.h"
#include "usb_device.h"
#include "usb_hid.h"
#include "wifi.h"

namespace {

enum { REPORT_ID_KEYBOARD = 1, REPORT_ID_MOUSE };

constexpr char TAG[] = "kbd_app";

constexpr uint64_t kMaxMainLoopWaitMSecs = 100;
constexpr uint32_t kMinMainLoopWaitMSecs = 10;
constexpr gpio_num_t kActivityGPIO = GPIO_NUM_2;

// Make sure min wait time is at least one tick.
static_assert((kMinMainLoopWaitMSecs / portTICK_PERIOD_MS) > 0);
static_assert(kMaxMainLoopWaitMSecs > kMinMainLoopWaitMSecs);

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
    : config_(new Config()),
      wifi_event_group_(nullptr),
      main_task_(nullptr),
      online_(false),
      did_spotify_test_(false) {}

App::~App() {
  if (wifi_event_group_)
    vEventGroupDelete(wifi_event_group_);
}

esp_err_t App::CreateWiFiStatusTask() {
  // https://www.freertos.org/FAQMem.html#StackSize
  // TODO: Reduce this size and take the HTTPS requtest out of this task.
  constexpr uint32_t kStackDepthWords = 2048;

  BaseType_t task = xTaskCreate(WiFiStatusTask, "wifi-status", kStackDepthWords,
                                this, tskIDLE_PRIORITY, &main_task_);
  if (task != pdPASS)
    return ESP_FAIL;
  return ESP_OK;
}

// static:
void IRAM_ATTR App::WiFiStatusTask(void* arg) {
  App* app = static_cast<App*>(arg);
  ESP_LOGW(TAG, "In Wi-Fi status task handler.");
  while (true) {
    EventBits_t bits =
        xEventGroupWaitBits(app->wifi_event_group_, WiFi::EVENT_ALL, pdFALSE,
                            pdFALSE, portMAX_DELAY);
    xEventGroupClearBits(app->wifi_event_group_, WiFi::EVENT_ALL);
    if (bits & WiFi::EVENT_NETWORK_GOT_IP) {
      ESP_LOGI(TAG, "Wi-Fi is connected.");
      app->online_ = true;
    } else if (bits & WiFi::EVENT_NETWORK_DISCONNECTED) {
      ESP_LOGW(TAG, "Wi-Fi connection failed.");
      app->online_ = false;
      // TODO: Set a timer so that we can retry in a little while.
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
  bool on = false;
  const std::string kTypedString = "Super Display Keyboard. ";
  int idx = 0;
  while (true) {
    if (usb::Device::Suspended()) {
      if (usb::Device::RemoteWakup() != ESP_OK)
        ESP_LOGE(TAG, "Error waking up");
      vTaskDelay(pdMS_TO_TICKS(500));
      continue;
    }
    if (!usb::HID::Ready()) {
      ESP_LOGW(TAG, "USB not ready.");
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }
    if (usb::Device::Mounted()) {
      ESP_LOGD(TAG, "Mounted, sending keyboard event.");
      // Merely setting GPIO2 (built-in LED) to OUTPUT on the Cucumber ESP32-S2
      // turns on the LED. Using gpio_set_level(_, 0) doesn't turn it off :-(.
      // TODO: Figure this out.
      gpio_set_direction(kActivityGPIO,
                         on ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT);
      on = !on;

      usb::HID::KeyboardPress(REPORT_ID_KEYBOARD, kTypedString[idx++]);
      if (idx == kTypedString.length())
        idx = 0;

      // For some reason a delay is required. Without one the keyboard release
      // doesn't seem to work and auto-repeats forever. 5ms was impirically
      // found to be short enough to prevent auto-repeat.
      ets_delay_us(5000);
      usb::HID::KeyboardRelease(REPORT_ID_KEYBOARD);
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

esp_err_t App::Initialize() {
  esp_err_t err = InitNVRAM();
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

  ESP_LOGI(TAG, "Wi-Fi SSID: \"%s\"", config_->wifi.ssid.c_str());
  wifi_event_group_ = xEventGroupCreate();
  wifi_.reset(new WiFi(wifi_event_group_));
  err = CreateWiFiStatusTask();
  if (err != ESP_OK)
    return err;

  err = CreateUSBTask();
  if (err != ESP_OK)
    return err;

  const i2c::Master::InitParams i2c_config = {
      .i2c_bus = I2C_NUM_0,
      .sda_gpio = 41,
      .scl_gpio = 40,
      .clk_speed = 400000,
      .sda_pullup_enable = true,
      .scl_pullup_enable = true,
  };
  i2c::Master i2c_master(I2C_NUM_0, /*mutex=*/nullptr);
  if (!i2c_master.Initialize(i2c_config))
    return ESP_FAIL;
  keyboard_.reset(new Keyboard(std::move(i2c_master)));

  err = keyboard_->Initialize();
  if (err != ESP_OK)
    return err;

  err = wifi_->Inititialize();
  if (err != ESP_OK)
    return err;

  err = wifi_->Connect(config_->wifi.ssid, config_->wifi.key);
  if (err != ESP_OK)
    return err;

  err = usb::Device::Initialize();
  if (err != ESP_OK)
    return err;

  https_server_.reset(new HTTPSServer());  // Initialize once online.

  display_.reset(new Display(320, 240));
  if (!display_->Initialize())
    return ESP_FAIL;

  spotify_.reset(new Spotify(config_.get(), https_server_.get()));

  CreateKeyboardSimulatorTask();

  return ESP_OK;
}

void App::Run() {
  display_->Update();
  while (true) {
    uint32_t wait_msecs = display_->HandleTask() / 1000;
    if (wait_msecs < kMinMainLoopWaitMSecs)
      wait_msecs = kMinMainLoopWaitMSecs;
    else if (wait_msecs > kMaxMainLoopWaitMSecs)
      wait_msecs = kMaxMainLoopWaitMSecs;

    if (online_ && !did_spotify_test_) {
      did_spotify_test_ = true;
      ESP_ERROR_CHECK_WITHOUT_ABORT(spotify_->Initialize());
    }
    // Need to use vTaskDelay to avoid triggering the task WDT.
    vTaskDelay(pdMS_TO_TICKS(wait_msecs));
    taskYIELD();  // Not sure if this is necessary.
  }
}
