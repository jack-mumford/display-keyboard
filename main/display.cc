#include "display.h"

#include <esp_err.h>
#include <esp_idf_version.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <lv_lib_png/lv_png.h>
#include <lv_lib_split_jpg/lv_sjpg.h>
#include <lvgl.h>
#include <lvgl_helpers.h>

#include "lvgl_drive.h"
#include "main_screen.h"

namespace {

const char TAG[] = "display";
const uint64_t kTickTimerPeriodUsec = 1000;
const uint16_t kNumBufferRows = 20;

bool my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
  return false;
}

}  // namespace

Display::Display(uint16_t width, uint16_t height)
    : initialized_(false),
      last_tick_time_(-1),
      tick_timer_(nullptr),
      width_(width),
      height_(height),
      display_buf_1_(new lv_color_t[width * kNumBufferRows]),
      display_buf_2_(new lv_color_t[width * kNumBufferRows]),
      disp_driver_(nullptr),
      lv_screen_(nullptr),
      input_driver_(nullptr) {
  ESP_LOGD(TAG, "Created display %ux%u.", width, height);

  lv_init();
  lvgl_driver_init();
  lv_png_init();
  lv_split_jpeg_init();
}

Display::~Display() = default;

void Display::Tick() {
  int64_t now = esp_timer_get_time();
  uint32_t tick_period = last_tick_time_ == -1 ? 0 : now - last_tick_time_;
  lv_tick_inc(tick_period);
  last_tick_time_ = now;
}

void Display::TickTimerCb(void* arg) {
  static_cast<Display*>(arg)->Tick();
}

esp_err_t Display::CreateTickTimer() {
  const esp_timer_create_args_t timer_args = {
    .callback = TickTimerCb,
    .arg = this,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "RadioDisplay::TickTimer",
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 3, 0)
    .skip_unhandled_events = true,
#endif
  };
  esp_err_t err = esp_timer_create(&timer_args, &tick_timer_);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Unable to create the periodic tick timer");
    return err;
  }
  err = esp_timer_start_periodic(tick_timer_, kTickTimerPeriodUsec);
  if (err != ESP_OK)
    ESP_LOGE(TAG, "Unable to install the periodic tick timer");
  return err;
}

bool Display::Initialize() {
  ESP_LOGD(TAG, "Initializing display");
  if (initialized_) {
    ESP_LOGW(TAG, "Already initialized");
    return true;
  }

  const uint32_t num_pixels = width_ * kNumBufferRows;
  lv_disp_buf_init(&disp_buf_, display_buf_1_.get(), display_buf_2_.get(),
                   num_pixels);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.flush_cb = disp_driver_flush;
  disp_drv.buffer = &disp_buf_;
  disp_driver_ = lv_disp_drv_register(&disp_drv);
  if (!disp_driver_)
    return false;

  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);

  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  input_driver_ = lv_indev_drv_register(&indev_drv);
  if (!input_driver_)
    return false;

  lv_screen_ = lv_disp_get_scr_act(disp_driver_);
  if (!lv_screen_)
    return false;

  if (CreateTickTimer() != ESP_OK)
    return false;

  if (lvgl::Drive::Initialize() != ESP_OK)
    return false;

  initialized_ = true;
  ESP_LOGD(TAG, "Display successfully initialized");
  return true;
}

uint32_t Display::HandleTask() {
  return lv_task_handler();
}

bool Display::Update() {
  if (!initialized_) {
    ESP_LOGE(TAG, "Display not initialized (or failed).");
    return false;
  }

  if (!screen_)
    screen_.reset(new MainScreen(*this));

  screen_->Update();
  return true;
}
