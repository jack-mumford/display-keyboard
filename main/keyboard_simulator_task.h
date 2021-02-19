#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>

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
