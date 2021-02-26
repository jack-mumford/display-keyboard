#pragma once

#include <cstdint>
#include <memory>

#include <esp_err.h>
#include <ssd1306.h>

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
  SSD1306_Device ssd_device_;
  std::unique_ptr<VolumeWidget> volume_widget_;
};
