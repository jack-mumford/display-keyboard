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
    : parent_(parent),
      volume_(0),
      x_(x),
      y_(y),
      width_(width),
      height_(height)
#ifdef USE_CANVAS
      ,
      cbuf_(new lv_color_t[LV_CANVAS_BUF_SIZE_TRUE_COLOR(width, height)])
#endif
{
}

esp_err_t VolumeWidget::Initialize() {
#ifdef USE_CANVAS
  if (!cbuf_)
    return ESP_OK;

  lv_draw_rect_dsc_t rect_dsc;
  lv_draw_rect_dsc_init(&rect_dsc);
  rect_dsc.radius = 10;
  rect_dsc.bg_opa = LV_OPA_COVER;
  rect_dsc.bg_grad_dir = LV_GRAD_DIR_HOR;
  rect_dsc.bg_color = LV_COLOR_RED;
  rect_dsc.bg_grad_color = LV_COLOR_BLUE;
  rect_dsc.border_width = 2;
  rect_dsc.border_opa = LV_OPA_90;
  rect_dsc.border_color = LV_COLOR_WHITE;
  rect_dsc.shadow_width = 5;
  rect_dsc.shadow_ofs_x = 5;
  rect_dsc.shadow_ofs_y = 5;

  lv_draw_label_dsc_t label_dsc;
  lv_draw_label_dsc_init(&label_dsc);
  label_dsc.color = LV_COLOR_YELLOW;

  lv_obj_t* canvas = lv_canvas_create(parent_, nullptr);
  lv_canvas_set_buffer(canvas, cbuf_.get(), width_, height_,
                       LV_IMG_CF_TRUE_COLOR);
  lv_obj_align(canvas, nullptr, LV_ALIGN_CENTER, 0, 0);
  lv_canvas_fill_bg(canvas, LV_COLOR_SILVER, LV_OPA_COVER);

  lv_canvas_draw_rect(canvas, 70, 60, 100, 70, &rect_dsc);

  lv_canvas_draw_text(canvas, 40, 20, 100, &label_dsc,
                      "Some text on text canvas", LV_LABEL_ALIGN_LEFT);

  /* Test the rotation. It requires an other buffer where the orignal image is
   * stored. So copy the current image to buffer and rotate it to the canvas */
  const size_t num_pixels = LV_CANVAS_BUF_SIZE_TRUE_COLOR(width_, height_);
  std::unique_ptr<lv_color_t> cbuf_tmp(new lv_color_t[num_pixels]);
  memcpy(cbuf_tmp.get(), cbuf_.get(), sizeof(lv_color_t) * num_pixels);
  lv_img_dsc_t img;
  img.data = reinterpret_cast<const uint8_t*>(cbuf_tmp.get());
  img.header.cf = LV_IMG_CF_TRUE_COLOR;
  img.header.w = width_;
  img.header.h = height_;

  lv_canvas_fill_bg(canvas, LV_COLOR_SILVER, LV_OPA_COVER);
  lv_canvas_transform(canvas, &img, 30, LV_IMG_ZOOM_NONE, 0, 0, width_ / 2,
                      height_ / 2, true);
#else
  bar_ = lv_obj_create(parent_, nullptr);
  lv_obj_set_pos(bar_, x_, y_);
  _lv_obj_set_style_local_color(
      bar_, LV_OBJ_PART_MAIN,
      LV_STYLE_BG_COLOR | (LV_STATE_DEFAULT << LV_STYLE_STATE_POS), kDrawColor);

  _lv_obj_set_style_local_int(
      bar_, LV_OBJ_PART_MAIN,
      LV_STYLE_RADIUS | (LV_STATE_DEFAULT << LV_STYLE_STATE_POS),
      LV_STYLE_ATTR_NONE);
#endif
  SetVolumeWidgetSize();

  return ESP_OK;
}

void VolumeWidget::SetVolume(uint8_t volume) {
  if (volume > kMaxVolume)
    volume = kMaxVolume;
  if (volume == volume_)
    return;
  volume_ = volume;
  SetVolumeWidgetSize();
}

void VolumeWidget::SetVolumeWidgetSize() {
  const lv_coord_t widget_width =
      (static_cast<lv_coord_t>(volume_) * width_) / kMaxVolume;

#ifdef USE_CANVAS
#else
  lv_obj_set_size(bar_, widget_width, height_);
#endif
}
