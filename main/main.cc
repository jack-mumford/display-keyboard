/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_log.h>
#include <esp_spi_flash.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <nvs_flash.h>

#include <sdkconfig.h>
#include "config.h"
#include "config_reader.h"
#include "display.h"
#include "filesystem.h"
#include "usb.h"
#include "wifi.h"

namespace {

constexpr char TAG[] = "main";

constexpr uint64_t kMaxMainLoopWaitMSecs = 100;
constexpr uint32_t kMinMainLoopWaitMSecs = 10;

// Make sure min wait time is at least one tick.
static_assert((kMinMainLoopWaitMSecs / portTICK_PERIOD_MS) > 0);
static_assert(kMaxMainLoopWaitMSecs > kMinMainLoopWaitMSecs);

void WaitForDebugMonitor() {
  // Poor man's way of waiting till the monitor has connected.
  const TickType_t kStartupDelay = 1000 / portTICK_PERIOD_MS;
  ESP_LOGI(TAG, "Waiting for debug monitor connection");
  vTaskDelay(kStartupDelay);
}

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

// See GPIO pin assignments in sdconfig.defaults

extern "C" void app_main(void) {
  WaitForDebugMonitor();

  ESP_ERROR_CHECK(InitNVRAM());

  ESP_ERROR_CHECK(esp_event_loop_create_default());

  Filesystem fs;
  ESP_ERROR_CHECK(fs.Initialize());
  std::unique_ptr<ConfigReader> config_reader(new ConfigReader);
  std::unique_ptr<Config> config(new Config());
  ESP_ERROR_CHECK(config_reader->Read(config.get()));

  ESP_LOGI(TAG, "Wi-Fi SSID: \"%s\"", config->wifi.ssid.c_str());
  ESP_LOGI(TAG, "Wi-Fi pre shared key: \"%s\"", config->wifi.key.c_str());

  std::unique_ptr<WiFi> wifi(new WiFi());
  ESP_ERROR_CHECK(wifi->Inititialize());
  ESP_ERROR_CHECK(wifi->Connect(config->wifi.ssid, config->wifi.key));

  USB usb;
  Display display(320, 240);
  display.Initialize();

  printf("Keyboard app!\n");

  /* Print chip information */
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  printf("This is %s chip with %d CPU core(s), WiFi%s%s, ", CONFIG_IDF_TARGET,
         chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
         (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  printf("silicon revision %d, ", chip_info.revision);

  printf(
      "%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
      (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  printf("Minimum free heap size: %d bytes\n",
         esp_get_minimum_free_heap_size());

  printf("Going into loop.\n");
  fflush(stdout);
  display.Update();
  while (true) {
    uint32_t wait_msecs = display.HandleTask() / 1000;
    if (wait_msecs < kMinMainLoopWaitMSecs)
      wait_msecs = kMinMainLoopWaitMSecs;
    else if (wait_msecs > kMaxMainLoopWaitMSecs)
      wait_msecs = kMaxMainLoopWaitMSecs;
    // Need to use vTaskDelay to avoid triggering the task WDT.
    vTaskDelay(pdMS_TO_TICKS(wait_msecs));
    taskYIELD();
  }
}
