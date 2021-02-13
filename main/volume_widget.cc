#include "volume_widget.h"

namespace {
constexpr int16_t kMaxVolume = 100;
constexpr lv_color_t kDrawColor = LV_COLOR_BLACK;
}  // namespace

VolumeWidget::VolumeWidget(lv_obj_t* parent,
                           int x,
                           int y,
                           int width,
                           int height)
    : volume_(0), x_(x), y_(y), width_(width), height_(height) {
  bar_ = lv_obj_create(parent, nullptr);
  lv_obj_set_pos(bar_, x_, y_);
  SetVolumeWidgetSize();
  _lv_obj_set_style_local_color(
      bar_, LV_OBJ_PART_MAIN,
      LV_STYLE_BG_COLOR | (LV_STATE_DEFAULT << LV_STYLE_STATE_POS), kDrawColor);
}

void VolumeWidget::SetVolume(uint8_t volume) {
  if (volume == volume_)
    return;
  volume_ = volume > kMaxVolume ? kMaxVolume : volume;
  SetVolumeWidgetSize();
}

void VolumeWidget::SetVolumeWidgetSize() {
  const lv_coord_t widget_width =
      (static_cast<lv_coord_t>(volume_) * width_) / kMaxVolume;
  lv_obj_set_size(bar_, widget_width, height_);
}
