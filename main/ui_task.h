#pragma once

#include <memory>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <esp_err.h>

#include "event_ids.h"

class MainDisplay;
class VolumeDisplay;

/**
 * The task responsible for doing **all** UI rendering to screens.
 */
class UITask {
 public:
  static esp_err_t Start();

  /**
   * Set WiFi status.
   *
   * thread-safe.
   */
  static void SetWiFiStatus(WiFiStatus status);

 private:
  static void IRAM_ATTR TaskFunc(void* arg);

  UITask();

  esp_err_t Initialize();
  void IRAM_ATTR Run();

  SemaphoreHandle_t mutex_;
  std::unique_ptr<MainDisplay> main_display_;
  std::unique_ptr<VolumeDisplay> volume_display_;
  TaskHandle_t task_ = nullptr;
  WiFiStatus wifi_status_ = WiFiStatus::Offline;
};
