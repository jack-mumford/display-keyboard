#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>

#include "volume_display.h"

/**
 * @brief Volume display and control.
 *
 * Owns the volume display, and responsible for reading from the volume
 * encoder, updating the display, and relaying volume level to the HID.
 *
 * @note All public functions are thread-safe.
 */
class VolumeTask {
 public:
  static esp_err_t Start();

 private:
  static void IRAM_ATTR TaskFunc(void* arg);

  VolumeTask();

  void Run();
  esp_err_t Initialize();

  TaskHandle_t task_ = nullptr;
  VolumeDisplay volume_display_;
};
