#pragma once

#include <cstdint>
#include <memory>

#include <lvgl.h>

#include "event_ids.h"

class MainScreen;

/**
 * Owns the LVGL display, touch panel, and any of our screen
 * objects to display content.
 *
 * @note This is not thread-safe, and caller must ensure that all
 * access occurs safely on the same thread (task).
 */
class MainDisplay {
 public:
  MainDisplay();
  ~MainDisplay();

  esp_err_t Initialize();
  lv_obj_t* lv_screen() const { return lv_screen_; }
  MainScreen* screen() const { return screen_.get(); }

 private:
  static void IRAM_ATTR TouchDriverFeedback(_lv_indev_drv_t*, lv_event_t);

  esp_err_t InitializeDisplayDriver();
  esp_err_t InitializeTouchPanelDriver();

  std::unique_ptr<MainScreen> screen_;
  bool initialized_ = false;
  std::unique_ptr<lv_color_t[]> display_buf_1_;
  std::unique_ptr<lv_color_t[]> display_buf_2_;
  lv_disp_buf_t disp_buf_;
  lv_disp_t* disp_driver_ = nullptr;
  lv_obj_t* lv_screen_ = nullptr;
  lv_indev_drv_t indev_drv_;
  lv_indev_t* input_device_ = nullptr;
};
