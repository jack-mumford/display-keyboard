#pragma once

#include <cstdint>
#include <memory>

#include <esp_err.h>
#include <lvgl.h>

#include "volume_widget.h"

class VolumeDisplay {
 public:
  VolumeDisplay();

  /**
   * @brief Set the Volume.
   *
   * @param volume The volume (0..100). Values outide of range will be clamped.
   */
  void SetVolume(uint8_t volume);
  esp_err_t Initialize();

 private:
  lv_disp_buf_t disp_buf_;
  lv_disp_t* disp_driver_;
  std::unique_ptr<lv_color_t[]> display_buf_1_;
  std::unique_ptr<lv_color_t[]> display_buf_2_;
  lv_obj_t* screen_;
  std::unique_ptr<VolumeWidget> volume_widget_;
};
