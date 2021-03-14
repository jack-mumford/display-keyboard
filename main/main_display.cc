#include "main_display.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_err.h>
#include <esp_idf_version.h>
#include <esp_log.h>
#include <lvgl.h>
#include <lvgl_helpers.h>
#include <lvgl_touch/touch_driver.h>

#include "main_screen.h"

namespace {
constexpr char TAG[] = "MainDisp";
}  // namespace

MainDisplay::MainDisplay()
    : screen_(new MainScreen(*this)),
      display_buf_1_(DISP_BUF_SIZE),
      display_buf_2_(DISP_BUF_SIZE) {}

MainDisplay::~MainDisplay() = default;

// static
void IRAM_ATTR MainDisplay::TouchDriverFeedback(_lv_indev_drv_t* driver,
                                                lv_event_t event) {
  ESP_LOGD(TAG, "Got touch feedback");
}

esp_err_t MainDisplay::InitializeDisplayDriver() {
// Not controllers used by this project, but checking for future flexibility.
#if defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_IL3820 ||   \
    defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_JD79653A || \
    defined CONFIG_LV_TFT_DISPLAY_CONTROLLER_UC8151D
  constexpr uint32_t kDispBufSizeInPixels = 8 * DISP_BUF_SIZE;
#else
  constexpr uint32_t kDispBufSizeInPixels = DISP_BUF_SIZE;
#endif

  lv_disp_buf_init(&disp_buf_, display_buf_1_.data(), display_buf_2_.data(),
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
  return ESP_OK;
}

esp_err_t MainDisplay::InitializeTouchPanelDriver() {
#if CONFIG_LV_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
  lv_indev_drv_init(&indev_drv_);
  indev_drv_.read_cb = touch_driver_read;
  indev_drv_.feedback_cb = TouchDriverFeedback;
  indev_drv_.type = LV_INDEV_TYPE_POINTER;
  input_device_ = lv_indev_drv_register(&indev_drv_);
  if (!input_device_)
    return ESP_FAIL;
#endif
  return ESP_OK;
}

esp_err_t MainDisplay::Initialize() {
  ESP_LOGD(TAG, "Initializing display");
  if (initialized_) {
    ESP_LOGW(TAG, "Already initialized");
    return ESP_OK;
  }

  esp_err_t err = InitializeDisplayDriver();
  if (err != ESP_OK)
    return err;

  err = InitializeTouchPanelDriver();
  if (err != ESP_OK)
    return err;

  lv_screen_ = lv_disp_get_scr_act(disp_driver_);
  if (!lv_screen_)
    return ESP_FAIL;

  err = screen_->Initialize();
  if (err != ESP_OK)
    return err;

  initialized_ = true;
  ESP_LOGD(TAG, "Display successfully initialized");
  return ESP_OK;
}
