#pragma once

#include <cstdint>
#include <memory>

#include <esp_timer.h>
#include <lvgl.h>

#include "event_ids.h"

class MainScreen;

class MainDisplay {
 public:
  MainDisplay(uint16_t width, uint16_t height);
  ~MainDisplay();

  esp_err_t Initialize();
  bool Update();
  uint32_t HandleTask();
  lv_obj_t* screen() { return lv_screen_; }
  void SetWiFiStatus(WiFiStatus status);

 private:
  static void TickTimerCb(void* arg);

  esp_err_t CreateTickTimer();
  void Tick();

  std::unique_ptr<MainScreen> screen_;
  bool initialized_;
  int64_t last_tick_time_;
  esp_timer_handle_t tick_timer_;
  const uint16_t width_;
  const uint16_t height_;
  std::unique_ptr<lv_color_t[]> display_buf_1_;
  std::unique_ptr<lv_color_t[]> display_buf_2_;
  lv_disp_buf_t disp_buf_;
  lv_disp_t* disp_driver_;
  lv_obj_t* lv_screen_;
  lv_indev_t* input_driver_;
};
