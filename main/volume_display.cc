
#include <utility>

#include "volume_display.h"

#include <esp_log.h>
#include <ssd1306_default_if.h>
#include <ssd1306_draw.h>

#include "volume_widget.h"

namespace {
constexpr char TAG[] = "VolDisp";
constexpr int kSSD1306_I2C_ADDR = 0x3C;
constexpr uint16_t kNumBufferRows = 16;
constexpr uint16_t kDisplayWidth = 128;
constexpr uint16_t kDisplayHeight = 64;
constexpr int kResetPin = -1;
constexpr uint32_t kNumBufferPixels = kDisplayWidth * kNumBufferRows;
constexpr uint16_t kMaxVolumeWidgetWidth = kDisplayWidth;
}  // namespace

VolumeDisplay::VolumeDisplay() = default;

void VolumeDisplay::SetVolume(uint8_t volume) {
  if (!volume_widget_)
    return;
  volume_widget_->SetVolume(volume);
  // Set contrast just for testing.
  SSD1306_SetContrast(&ssd_device_, volume > 50 ? 255 : 10);
  SSD1306_Update(&ssd_device_);
}

esp_err_t VolumeDisplay::Initialize() {
  if (!SSD1306_I2CMasterInitDefault()) {
    ESP_LOGE(TAG, "Unable to initialize tarablessd1306 library.");
    return ESP_FAIL;
  }

  if (!SSD1306_I2CMasterAttachDisplayDefault(&ssd_device_, kDisplayWidth,
                                             kDisplayHeight, kSSD1306_I2C_ADDR,
                                             kResetPin)) {
    ESP_LOGE(TAG, "Unable to initialize tarablessd1306 display.");
    return ESP_FAIL;
  }

  SSD1306_Clear(&ssd_device_, SSD_COLOR_BLACK);
  SSD1306_SetHFlip(&ssd_device_, true);

  volume_widget_.reset(
      new VolumeWidget(&ssd_device_, 0, 0, kDisplayWidth, kDisplayHeight));
  if (!volume_widget_)
    return ESP_ERR_NO_MEM;
  esp_err_t err = volume_widget_->Initialize();
  if (err != ESP_OK)
    return err;

  SSD1306_Update(&ssd_device_);
  return ESP_OK;
}
