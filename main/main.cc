#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2clib/master.h>
#include <nvs_flash.h>
#include <sdkconfig.h>

#include "gpio_pins.h"
#include "main_task.h"

namespace {

constexpr char TAG[] = "App";

#if (BOARD_FEATHERS2 == 0)
void WaitForDebugMonitor() {
  // Poor man's way of waiting till the monitor has connected.
  const TickType_t kStartupDelay = 1000 / portTICK_PERIOD_MS;
  ESP_LOGI(TAG, "Waiting for debug monitor connection");
  vTaskDelay(kStartupDelay);
}
#endif  // BOARD_FEATHERS2

void LogHardwareInfo() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  ESP_LOGI(TAG, "This is %s chip with %d CPU core(s), WiFi%s%s.",
           CONFIG_IDF_TARGET, chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  ESP_LOGI(TAG, "Silicon revision %u.", chip_info.revision);

  ESP_LOGI(
      TAG, "%zu MB %s flash", spi_flash_get_chip_size() / (1024 * 1024),
      (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  ESP_LOGI(TAG, "Minimum free heap size: %u bytes.",
           esp_get_minimum_free_heap_size());
}

}  // namespace

extern "C" void app_main(void) {
#if (BOARD_FEATHERS2 == 0)
  // The serial UART is used for Feather S2 debugging (which is always setup)
  // so no need to wait.
  WaitForDebugMonitor();
#endif

  ESP_LOGI(TAG, "Keyboard app!");
  LogHardwareInfo();

  ESP_ERROR_CHECK(MainTask::Start());
}
