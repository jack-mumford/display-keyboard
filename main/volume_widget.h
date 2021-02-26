
#pragma once

#include <cstdint>
#include <memory>

#include <esp_err.h>

struct SSD1306_Device;

class VolumeWidget {
 public:
  VolumeWidget(SSD1306_Device* display, int x, int y, int width, int height);

  /**
   * @brief Set the Volume.
   *
   * @param volume The volume (0..100). Values outide of range will be clamped.
   */
  void SetVolume(uint8_t volume);

  esp_err_t Initialize();

 private:
  int CalcWidth(uint8_t volume) const;

  SSD1306_Device* display_;
  uint8_t volume_;
  const int x_;
  const int y_;
  const int width_;
  const int height_;
};
