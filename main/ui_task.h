#pragma once

#include <string>
#include <vector>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_timer.h>

#include "event_ids.h"
#include "main_display.h"
#include "resource_fetcher.h"

/**
 * The task responsible for doing **all** UI rendering to screens.
 */
class UITask : public ResourceFetchClient {
 public:
  static esp_err_t Start();

  /**
   * Set WiFi status.
   *
   * thread-safe.
   */
  static void SetWiFiStatus(WiFiStatus status);

  // ResourceFetchClient:
  void FetchImageResult(uint32_t request_id, lv_img_dsc_t image) override;
  void FetchResult(uint32_t request_id,
                   int http_status_code,
                   std::vector<uint8_t> resource_data,
                   std::string mime_type) override;
  void FetchError(uint32_t request_id, esp_err_t err) override;

 private:
  static void IRAM_ATTR TaskFunc(void* arg);
  static void IRAM_ATTR TickTimerCb(void* arg);
  static void IRAM_ATTR UpdateTimeCb(void* arg);

  UITask();

  std::string GetCoverArtURL() const;
  void SetDarkMode();
  void UpdateTime();
  esp_err_t CreateUpdateTimeTimer();
  esp_err_t CreateTickTimer();
  void Tick();
  esp_err_t Initialize();
  void IRAM_ATTR Run();

  SemaphoreHandle_t mutex_;
  MainDisplay main_display_;
  TaskHandle_t task_ = nullptr;
  esp_timer_handle_t tick_timer_ = nullptr;
  esp_timer_handle_t time_update_timer_ = nullptr;
  uint32_t time_update_count_ = 0;  // Just for testing.
  WiFiStatus wifi_status_ = WiFiStatus::Offline;
  int64_t last_tick_time_ = -1;
  uint8_t test_covar_art_img_idx_ = 1;  // Just for testing.
  ResourceFetcher* fetcher_;
  uint32_t next_fetch_id_ = 1;
};
