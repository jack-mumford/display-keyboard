#include "app.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <class/hid/hid.h>
#include <esp_log.h>
#include <esp_spi_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>

#include "config.h"
#include "config_reader.h"
#include "display.h"
#include "filesystem.h"
#include "led_controller.h"
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
    : config_(new Config()), wifi_event_group_(nullptr), main_task_(nullptr) {}

App::~App() {
  if (wifi_event_group_)
    vEventGroupDelete(wifi_event_group_);
}

esp_err_t App::CreateWiFiStatusTask() {
  constexpr char kTaskName[] = "wifi-status";
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  BaseType_t task = xTaskCreate(WiFiTaskHandler, kTaskName, kStackDepthWords,
                                this, tskIDLE_PRIORITY, &main_task_);
  if (task != pdPASS)
    return ESP_FAIL;
  return ESP_OK;
}

// static:
void IRAM_ATTR App::WiFiTaskHandler(void* arg) {
  App* app = static_cast<App*>(arg);
  ESP_LOGW(TAG, "In Wi-Fi status task handler.");
  while (true) {
    EventBits_t bits =
        xEventGroupWaitBits(app->wifi_event_group_, WiFi::EVENT_ALL, pdFALSE,
                            pdFALSE, portMAX_DELAY);
    xEventGroupClearBits(app->wifi_event_group_, WiFi::EVENT_ALL);
    if (bits & WiFi::EVENT_CONNECTED) {
      ESP_LOGI(TAG, "Wi-Fi is connected.");
    } else if (bits & WiFi::EVENT_CONNECTION_FAILED) {
      ESP_LOGW(TAG, "Wi-Fi connection failed.");
      // TODO: Set a timer so that we can retry in a little while.
    }
  }
}

esp_err_t App::CreateUSBTestTask() {
  constexpr char kTaskName[] = "usb-test";
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  return xTaskCreate(USBTestTaskHandler, kTaskName, kStackDepthWords, this,
                     tskIDLE_PRIORITY, &main_task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

// static
void IRAM_ATTR App::USBTestTaskHandler(void* arg) {
  App* app = static_cast<App*>(arg);
  ESP_LOGW(TAG, "In USB test task handler.");
  uint8_t keycodes[6] = {HID_KEY_A,    HID_KEY_NONE, HID_KEY_NONE,
                         HID_KEY_NONE, HID_KEY_NONE, HID_KEY_NONE};
  while (true) {
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    if (app->usb_device_->Suspended()) {
      if (app->usb_device_->RemoteWakup() != ESP_OK)
        ESP_LOGE(TAG, "Error waking up");
      continue;
    }
    if (!app->usb_hid_->Ready()) {
      ESP_LOGW(TAG, "USB not ready.");
      continue;
    }
    if (!app->usb_device_->Mounted()) {
      ESP_LOGI(TAG, "Sending 'A' key");
      app->usb_hid_->KeyboardReport(REPORT_ID_KEYBOARD, 0, keycodes);
      app->usb_hid_->KeyboardRelease(REPORT_ID_KEYBOARD);
    } else {
      ESP_LOGD(TAG, "USB not mounted");
    }
  }
}

esp_err_t App::CreateUSBTask() {
  constexpr char kTaskName[] = "usb-tick";
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  return xTaskCreate(USBTaskHandler, kTaskName, kStackDepthWords, this,
                     tskIDLE_PRIORITY, &main_task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

// static
void IRAM_ATTR App::USBTaskHandler(void* arg) {
  App* app = static_cast<App*>(arg);
  bool on = false;
  while (true) {
    // Merely setting GPIO2 (built-in LED) to OUTPUT on the Cucumber ESP32-S2
    // turns on the LED. Using gpio_set_level(_, 0) doesn't turn if off :-(.
    // TODO: Figure this out later.
    gpio_set_direction(kActivityGPIO, on ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT);
    on = !on;
    app->usb_device_->Tick();
    vTaskDelay(pdMS_TO_TICKS(100));
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

  err = wifi_->Inititialize();
  if (err != ESP_OK)
    return err;

  err = wifi_->Connect(config_->wifi.ssid, config_->wifi.key);
  if (err != ESP_OK)
    return err;

  usb_device_.reset(new usb::Device());
  err = usb_device_->Initialize();
  if (err != ESP_OK)
    return err;
  usb_hid_.reset(new usb::HID());
  err = usb_hid_->Initialize();
  if (err != ESP_OK)
    return err;

  display_.reset(new Display(320, 240));
  if (!display_->Initialize())
    return ESP_FAIL;

  CreateUSBTestTask();

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
    // Need to use vTaskDelay to avoid triggering the task WDT.
    vTaskDelay(pdMS_TO_TICKS(wait_msecs));
    taskYIELD();
  }
}