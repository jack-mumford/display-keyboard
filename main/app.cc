#include "app.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_log.h>
#include <esp_spi_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>

#include "config.h"
#include "config_reader.h"
#include "display.h"
#include "filesystem.h"
#include "usb.h"
#include "wifi.h"

namespace {

constexpr char TAG[] = "kbd_app";

constexpr uint64_t kMaxMainLoopWaitMSecs = 100;
constexpr uint32_t kMinMainLoopWaitMSecs = 10;

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

App::App() : wifi_event_group_(nullptr), main_task_(nullptr) {}

App::~App() {
  if (wifi_event_group_)
    vEventGroupDelete(wifi_event_group_);
}

esp_err_t App::CreateWiFiStatusTask() {
  constexpr char kTaskName[] = "wifi-status";
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  BaseType_t task = xTaskCreate(TaskHandler, kTaskName, kStackDepthWords, this,
                                tskIDLE_PRIORITY, &main_task_);
  if (task != pdPASS)
    return ESP_FAIL;
  return ESP_OK;
}

// static:
void IRAM_ATTR App::TaskHandler(void* arg) {
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
    }
  }
}

esp_err_t App::Initialize() {
  ESP_ERROR_CHECK(InitNVRAM());

  ESP_ERROR_CHECK(esp_event_loop_create_default());

  fs_.reset(new Filesystem());
  ESP_ERROR_CHECK(fs_->Initialize());
  ConfigReader config_reader;
  std::unique_ptr<Config> config(new Config());
  ESP_ERROR_CHECK(config_reader.Read(config.get()));

  ESP_LOGI(TAG, "Wi-Fi SSID: \"%s\"", config->wifi.ssid.c_str());
  wifi_event_group_ = xEventGroupCreate();
  wifi_.reset(new WiFi(wifi_event_group_));
  ESP_ERROR_CHECK(CreateWiFiStatusTask());
  ESP_ERROR_CHECK(wifi_->Inititialize());
  ESP_ERROR_CHECK(wifi_->Connect(config->wifi.ssid, config->wifi.key));

  usb_.reset(new USB());
  display_.reset(new Display(320, 240));
  if (!display_->Initialize())
    return ESP_FAIL;

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