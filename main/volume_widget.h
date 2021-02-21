
#pragma once

#include <cstdint>
#include <memory>

#include <esp_err.h>
#include <lvgl.h>

// The canvas implementation of this widget currently crashes LVGL. Probably
// a bug in this app or issue in LVGL's nascent multi-screen implementation.
#undef USE_CANVAS

class VolumeWidget {
 public:
  VolumeWidget(lv_obj_t* parent, int x, int y, int width, int height);

  /**
   * @brief Set the Volume.
   *
   * @param volume The volume (0..100). Values outide of range will be clamped.
   */
  void SetVolume(uint8_t volume);

  esp_err_t Initialize();

 private:
  void SetVolumeWidgetSize();

  lv_obj_t* parent_;
  uint8_t volume_;
  const int x_;
  const int y_;
  const int width_;
  const int height_;
#ifdef USE_CANVAS
  std::unique_ptr<lv_color_t> cbuf_;
#else
  lv_obj_t* bar_ = nullptr;
#endif
};
