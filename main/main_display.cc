#include "main_display.h"

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

const char TAG[] = "MainDisp";
const uint64_t kTickTimerPeriodUsec = 1000;

bool my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
  return false;
}

}  // namespace

MainDisplay::MainDisplay(uint16_t width, uint16_t height)
    : screen_(new MainScreen(*this)),
      initialized_(false),
      last_tick_time_(-1),
      tick_timer_(nullptr),
      width_(width),
      height_(height),
      display_buf_1_(new lv_color_t[DISP_BUF_SIZE]),
      display_buf_2_(new lv_color_t[DISP_BUF_SIZE]),
      disp_driver_(nullptr),
      lv_screen_(nullptr),
      input_driver_(nullptr) {}

MainDisplay::~MainDisplay() = default;

void MainDisplay::Tick() {
  int64_t now = esp_timer_get_time();
  uint32_t tick_period = last_tick_time_ == -1 ? 0 : now - last_tick_time_;
  lv_tick_inc(tick_period);
  last_tick_time_ = now;
}

void MainDisplay::TickTimerCb(void* arg) {
  static_cast<MainDisplay*>(arg)->Tick();
}

esp_err_t MainDisplay::CreateTickTimer() {
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

esp_err_t MainDisplay::Initialize() {
  ESP_LOGD(TAG, "Initializing display");
  if (initialized_) {
    ESP_LOGW(TAG, "Already initialized");
    return ESP_OK;
  }

// Not controllers used by this project, but checking for future flexibility.
#if defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_IL3820 ||   \
    defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_JD79653A || \
    defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_UC8151D
  constexpr uint32_t kDispBufSizeInPixels = 8 * DISP_BUF_SIZE;
#else
  constexpr uint32_t kDispBufSizeInPixels = DISP_BUF_SIZE;
#endif

  lv_disp_buf_init(&disp_buf_, display_buf_1_.get(), display_buf_2_.get(),
                   kDispBufSizeInPixels);

  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.flush_cb = disp_driver_flush;
#ifdef CONFIG_LV_TFT_DISPLAY_MONOCHROME
  // Only needed for monochrome displays.
  disp_drv.rounder_cb = disp_driver_rounder;
  disp_drv.set_px_cb = disp_driver_set_px;
#endif
  disp_drv.buffer = &disp_buf_;
  disp_driver_ = lv_disp_drv_register(&disp_drv);
  if (!disp_driver_)
    return ESP_FAIL;

  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);

  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  input_driver_ = lv_indev_drv_register(&indev_drv);
  if (!input_driver_)
    return ESP_FAIL;

  lv_screen_ = lv_disp_get_scr_act(disp_driver_);
  if (!lv_screen_)
    return ESP_FAIL;

  if (CreateTickTimer() != ESP_OK)
    return ESP_FAIL;

  if (lvgl::Drive::Initialize() != ESP_OK)
    return ESP_FAIL;

  esp_err_t err = screen_->Initialize();
  if (err != ESP_OK)
    return err;

  initialized_ = true;
  ESP_LOGD(TAG, "Display successfully initialized");
  return ESP_OK;
}

uint32_t MainDisplay::HandleTask() {
  return lv_task_handler();
}

void MainDisplay::SetWiFiStatus(WiFiStatus status) {
  screen_->SetWiFiStatus(status);
}
