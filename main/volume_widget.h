
#pragma once

#include <cstdint>

#include <lvgl.h>

class VolumeWidget {
 public:
  VolumeWidget(lv_obj_t* parent, int x, int y, int width, int height);

  /**
   * @brief Set the Volume.
   *
   * @param volume The volume (0..100). Values outide of range will be clamped.
   */
  void SetVolume(uint8_t volume);

 private:
  void SetVolumeWidgetSize();

  uint8_t volume_;
  const int x_;
  const int y_;
  const int width_;
  const int height_;
  lv_obj_t* bar_;
};
