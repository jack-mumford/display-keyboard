#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_timer.h>

#include "event_ids.h"
#include "main_display.h"

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
  static void IRAM_ATTR TickTimerCb(void* arg);
  static void IRAM_ATTR UpdateTimeCb(void* arg);
  static void IRAM_ATTR TouchDriverFeedback(_lv_indev_drv_t*, lv_event_t);

  UITask();

  void SetDarkMode();
  void UpdateTime();
  esp_err_t CreateUpdateTimeTimer();
  esp_err_t CreateTickTimer();
  void Tick();
  esp_err_t Initialize();
  esp_err_t InitializeTouchPanel();
  void IRAM_ATTR Run();

  SemaphoreHandle_t mutex_;
  MainDisplay main_display_;
  TaskHandle_t task_ = nullptr;
  esp_timer_handle_t tick_timer_ = nullptr;
  esp_timer_handle_t time_update_timer_ = nullptr;
  WiFiStatus wifi_status_ = WiFiStatus::Offline;
  int64_t last_tick_time_ = -1;
  lv_indev_drv_t indev_drv_;
  lv_indev_t* input_device_ = nullptr;
};
