#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <i2clib/master.h>
#include <nvs_flash.h>
#include <sdkconfig.h>

#include "app.h"
#include "gpio_pins.h"
#include "keyboard.h"

namespace {

constexpr char TAG[] = "kbd_main";

void WaitForDebugMonitor() {
  // Poor man's way of waiting till the monitor has connected.
  const TickType_t kStartupDelay = 1000 / portTICK_PERIOD_MS;
  ESP_LOGI(TAG, "Waiting for debug monitor connection");
  vTaskDelay(kStartupDelay);
}

void LogHardwareInfo() {
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  ESP_LOGI(TAG, "This is %s chip with %d CPU core(s), WiFi%s%s.",
           CONFIG_IDF_TARGET, chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

  ESP_LOGI(TAG, "Silicon revision %u.", chip_info.revision);

  ESP_LOGI(
      TAG, "%dMB %s flash", spi_flash_get_chip_size() / (1024 * 1024),
      (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

  ESP_LOGI(TAG, "Minimum free heap size: %d bytes.",
           esp_get_minimum_free_heap_size());
}

esp_err_t KeyboardTest() {
  const i2c::Master::InitParams i2c_config = {
      .i2c_bus = I2C_NUM_0,
      .sda_gpio = kI2C0SDA,
      .scl_gpio = kI2C0SCL,
      .clk_speed = 400000,
      .sda_pullup_enable = true,
      .scl_pullup_enable = true,
  };
  i2c::Master i2c_master(I2C_NUM_0, /*mutex=*/nullptr);
  if (!i2c_master.Initialize(i2c_config))
    return ESP_FAIL;
  Keyboard keyboard(std::move(i2c_master));
  esp_err_t err = keyboard.Initialize();
  if (err != ESP_OK)
    return err;

  int count = 1;
  while (true) {
    ESP_LOGI(TAG, "Iteration %d", count++);
    keyboard.LogEventCount();
    vTaskDelay(pdMS_TO_TICKS(1000));
  }

  return ESP_OK;
}

}  // namespace

extern "C" void app_main(void) {
  WaitForDebugMonitor();

  ESP_LOGI(TAG, "Keyboard app!");
  LogHardwareInfo();

  App app;
  ESP_ERROR_CHECK(app.Initialize());
  app.Run();
}
