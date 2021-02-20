#pragma once

#include <cstdint>
#include <memory>

#include <lvgl.h>

#include "event_ids.h"

class MainScreen;

class MainDisplay {
 public:
  MainDisplay();
  ~MainDisplay();

  esp_err_t Initialize();
  lv_obj_t* screen() { return lv_screen_; }
  void SetWiFiStatus(WiFiStatus status);

 private:
  std::unique_ptr<MainScreen> screen_;
  bool initialized_;
  std::unique_ptr<lv_color_t[]> display_buf_1_;
  std::unique_ptr<lv_color_t[]> display_buf_2_;
  lv_disp_buf_t disp_buf_;
  lv_disp_t* disp_driver_;
  lv_obj_t* lv_screen_;
  lv_indev_t* input_driver_;
};
