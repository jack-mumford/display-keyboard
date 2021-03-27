#pragma once

#include <freertos/include/freertos/FreeRTOS.h>
#include <freertos/include/freertos/task.h>

#include <esp_err.h>

class USBTask {
 public:
  static esp_err_t Start();

 private:
  static void IRAM_ATTR TaskFunc(void* arg);

  USBTask();

  esp_err_t Initialize();

  TaskHandle_t task_ = nullptr;
};
