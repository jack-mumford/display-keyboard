
#include <utility>

#include <lv_core/lv_obj.h>
#include <lv_widgets/lv_canvas.h>
#include <lvgl_tft/ssd1306.h>

#include "volume_display.h"

namespace {
constexpr uint16_t kNumBufferRows = 16;
constexpr uint16_t kDisplayWidth = 128;
constexpr uint16_t kDisplayHeight = 64;
constexpr uint16_t kCanvasWidth = 40;
constexpr uint16_t kCanvasHeight = 40;
constexpr uint32_t kNumBufferPixels = kDisplayWidth * kNumBufferRows;
constexpr int16_t kMaxVolume = 100;
constexpr int16_t kMinVolume = 0;
}  // namespace

VolumeDisplay::VolumeDisplay(i2c::Master master)
    : i2c_master_(std::move(master)),
      disp_driver_(nullptr),
      display_buf_1_(new lv_color_t[kNumBufferPixels]),
      display_buf_2_(new lv_color_t[kNumBufferPixels]),
      canvas_buffer_(
          new lv_color_t[LV_CANVAS_BUF_SIZE_TRUE_COLOR(kCanvasWidth,
                                                       kCanvasHeight)]),
      screen_(nullptr),
      canvas_(nullptr),
      volume_(25) {}

bool VolumeDisplay::Initialize() {
  if (!display_buf_1_ || !display_buf_2_ || !canvas_buffer_)
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

#if 0
  canvas_ = lv_canvas_create(screen_, nullptr);
  if (!canvas_)
    return true;
  static lv_color_t
      cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(kCanvasWidth, kCanvasHeight)];
  lv_canvas_set_buffer(canvas_, cbuf, kCanvasWidth, kCanvasHeight,
                       LV_IMG_CF_TRUE_COLOR);
  lv_canvas_fill_bg(canvas_, LV_COLOR_WHITE, LV_OPA_COVER);
#endif

#if 0
  canvas_ = lv_canvas_create(screen_, nullptr);
  if (!canvas_)
    return true;
  lv_canvas_set_buffer(canvas_, canvas_buffer_.get(), kCanvasWidth,
                       kCanvasHeight, LV_IMG_CF_TRUE_COLOR);
  lv_canvas_fill_bg(canvas_, LV_COLOR_WHITE, LV_OPA_COVER);
#endif
#if 0
  lv_draw_rect_dsc_t draw_dsc;
  lv_draw_rect_dsc_init(&draw_dsc);
  draw_dsc.bg_color = LV_COLOR_BLACK;
  lv_canvas_draw_rect(canvas_, 0, 0, 60, 20, &draw_dsc);
#endif

  return true;
}

void VolumeDisplay::Update() {
  // lv_bar_set_value(bar, new_value, LV_ANIM_ON/OFF)
}
