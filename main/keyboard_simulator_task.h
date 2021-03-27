#pragma once

#include <freertos/include/freertos/FreeRTOS.h>
#include <freertos/include/freertos/task.h>

#include <esp_err.h>

/**
 * Task used only for testing to simulate key presses.
 */
class KeyboardSimulatorTask {
 public:
  static esp_err_t Start();

 private:
  static void IRAM_ATTR TaskFunc(void* arg);

  KeyboardSimulatorTask();
  ~KeyboardSimulatorTask();

  esp_err_t Initialize();
  void IRAM_ATTR Run();

  TaskHandle_t task_ = nullptr;  // This task.
};
