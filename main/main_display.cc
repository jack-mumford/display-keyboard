#include "main_display.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_err.h>
#include <esp_idf_version.h>
#include <esp_log.h>
#include <lv_lib_png/lv_png.h>
#include <lv_lib_split_jpg/lv_sjpg.h>
#include <lvgl.h>
#include <lvgl_helpers.h>

#include "lvgl_drive.h"
#include "main_screen.h"

namespace {

const char TAG[] = "MainDisp";

bool my_touchpad_read(lv_indev_drv_t* indev_driver, lv_indev_data_t* data) {
  return false;
}

}  // namespace

MainDisplay::MainDisplay()
    : screen_(new MainScreen(*this)),
      initialized_(false),
      display_buf_1_(new lv_color_t[DISP_BUF_SIZE]),
      display_buf_2_(new lv_color_t[DISP_BUF_SIZE]),
      disp_driver_(nullptr),
      lv_screen_(nullptr),
      input_driver_(nullptr) {}

MainDisplay::~MainDisplay() = default;

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

  if (lvgl::Drive::Initialize() != ESP_OK)
    return ESP_FAIL;

  esp_err_t err = screen_->Initialize();
  if (err != ESP_OK)
    return err;

  initialized_ = true;
  ESP_LOGD(TAG, "Display successfully initialized");
  return ESP_OK;
}

void MainDisplay::SetWiFiStatus(WiFiStatus status) {
  screen_->SetWiFiStatus(status);
}

void MainDisplay::UpdateTime() {
  if (!screen_)
    return;
  screen_->UpdateTime();
}
