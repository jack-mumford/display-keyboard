#include "volume_widget.h"

#include <ssd1306_draw.h>

namespace {
constexpr int16_t kMaxVolume = 100;
}  // namespace

VolumeWidget::VolumeWidget(SSD1306_Device* display,
                           int x,
                           int y,
                           int width,
                           int height)
    : display_(display),
      volume_(0),
      x_(x),
      y_(y),
      width_(width),
      height_(height) {}

esp_err_t VolumeWidget::Initialize() {
  SetVolume(volume_);
  return ESP_OK;
}

int VolumeWidget::CalcWidth(uint8_t volume) const {
  return (static_cast<int>(volume) * width_) / kMaxVolume;
}

void VolumeWidget::SetVolume(uint8_t volume) {
  if (volume > kMaxVolume)
    volume = kMaxVolume;
  if (volume == volume_)
    return;
  const int prev_width = CalcWidth(volume_);
  volume_ = volume;
  const int new_width = CalcWidth(volume_);
  if (prev_width == new_width)
    return;

  if (new_width > prev_width) {
    // Nothing to erase. Fill the delta between prior value and new.
    SSD1306_DrawBox(display_, prev_width, y_, new_width, height_,
                    SSD_COLOR_WHITE,
                    /*Fill=*/true);
  } else {
    SSD1306_DrawBox(display_, new_width, y_, prev_width, height_,
                    SSD_COLOR_BLACK,
                    /*Fill=*/true);
  }
}
