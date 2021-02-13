
#include <utility>

#include <lv_core/lv_obj.h>
#include <lvgl_tft/ssd1306.h>

#include "volume_display.h"

#include "volume_widget.h"

namespace {
constexpr uint16_t kNumBufferRows = 16;
constexpr uint16_t kDisplayWidth = 128;
constexpr uint16_t kDisplayHeight = 64;
constexpr uint32_t kNumBufferPixels = kDisplayWidth * kNumBufferRows;
constexpr uint16_t kMaxVolumeWidgetWidth = kDisplayWidth;
}  // namespace

VolumeDisplay::VolumeDisplay()
    : disp_driver_(nullptr),
      display_buf_1_(new lv_color_t[kNumBufferPixels]),
      display_buf_2_(new lv_color_t[kNumBufferPixels]),
      screen_(nullptr) {}

void VolumeDisplay::SetVolume(uint8_t volume) {
  if (!volume_widget_)
    return;
  volume_widget_->SetVolume(volume);
}

bool VolumeDisplay::Initialize() {
  if (!display_buf_1_ || !display_buf_2_)
    return false;

  // The lvgl_esp32_drivers library initializes the one configured
  // display when lvgl_driver_init() is called. We need to manually
  // initialize supplemental drivers. Maybe that library can support
  // more than one display type in the future. Bug added at
  // https://github.com/lvgl/lvgl_esp32_drivers/issues/31
  ssd1306_init();

  lv_disp_buf_init(&disp_buf_, display_buf_1_.get(), display_buf_2_.get(),
                   kNumBufferPixels);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.flush_cb = ssd1306_flush;
  disp_drv.rounder_cb = ssd1306_rounder;
  disp_drv.set_px_cb = ssd1306_set_px_cb;
  disp_drv.buffer = &disp_buf_;
  disp_drv.hor_res = kDisplayWidth;
  disp_drv.ver_res = kDisplayHeight;

  disp_driver_ = lv_disp_drv_register(&disp_drv);
  if (!disp_driver_)
    return false;

  screen_ = lv_disp_get_scr_act(disp_driver_);
  if (!screen_)
    return false;

  volume_widget_.reset(
      new VolumeWidget(screen_, 0, 0, kDisplayWidth, kDisplayHeight));
  return true;
}
