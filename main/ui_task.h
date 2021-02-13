#pragma once

#include <memory>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>

class Display;
class VolumeDisplay;

/**
 * The task responsible for doing **all** UI rendering to screens.
 */
class UITask {
 public:
  static esp_err_t Start();

 private:
  static void IRAM_ATTR TaskFunc(void* arg);

  UITask();

  esp_err_t Initialize();
  void IRAM_ATTR Run();

  std::unique_ptr<Display> display_;
  std::unique_ptr<VolumeDisplay> volume_display_;
  TaskHandle_t task_ = nullptr;
};
