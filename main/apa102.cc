#include "apa102.h"

#include <cstring>

#include <driver/gpio.h>
#include <esp_log.h>

namespace {

struct LEDFrames {
  uint32_t frame_start_indicator;  // APA102 start frame: 0x00000000.
  APA102::Color color;             // The LED color value.
  uint32_t frame_end_indicator;    // APA102 end frame: 0xffffffff.
};

constexpr size_t kBitsPerByte = 8;
constexpr int kMaxSPIFrameBytes = 256;
constexpr int kSPIClockFrequencyHz = 1e6;

constexpr char TAG[] = "APA102";

}  // namespace

APA102::APA102(gpio_num_t sclk_gpio,
               gpio_num_t mosi_gpio,
               spi_host_device_t spi_host)
    : sclk_gpio_(sclk_gpio),
      mosi_gpio_(mosi_gpio),
      spi_host_(spi_host),
      spi_device_(nullptr),
      led_frame_(nullptr) {
  std::memset(&trans_desc_, 0, sizeof(trans_desc_));
}

APA102::~APA102() {
  if (spi_device_)
    spi_bus_remove_device(spi_device_);
  if (led_frame_)
    heap_caps_free(led_frame_);
}

esp_err_t APA102::Initlialize() {
  static_assert(sizeof(LEDFrames) == 12);
  led_frame_ = heap_caps_malloc(sizeof(LEDFrames), MALLOC_CAP_DMA);
  LEDFrames* frame = static_cast<LEDFrames*>(led_frame_);
  frame->frame_start_indicator = 0x0;
  frame->frame_end_indicator = 0xffffffff;

  const gpio_config_t config = {
      .pin_bit_mask = (1UL << sclk_gpio_) | (1UL << mosi_gpio_),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  esp_err_t err = gpio_config(&config);
  if (err != ESP_OK)
    return err;

  const spi_bus_config_t buscfg = {
      .mosi_io_num = mosi_gpio_,
      .miso_io_num = -1,  // not used.
      .sclk_io_num = sclk_gpio_,
      .quadwp_io_num = -1,  // not used.
      .quadhd_io_num = -1,  // not used.
      .max_transfer_sz = kMaxSPIFrameBytes,
      .flags = SPICOMMON_BUSFLAG_MASTER,
      .intr_flags = 0x0,
  };

  err = spi_bus_initialize(spi_host_, &buscfg, SPI_DMA_CH_AUTO);
  if (err != ESP_OK)
    return err;

  const spi_device_interface_config_t devcfg = {
      .command_bits = 0,
      .address_bits = 0,
      .dummy_bits = 0,
      .mode = 0,  // 0: (0, 0)
      .duty_cycle_pos = 0,
      .cs_ena_pretrans = 0,
      .cs_ena_posttrans = 0,
      .clock_speed_hz = kSPIClockFrequencyHz,
      .input_delay_ns = 0,
      .spics_io_num = -1,  // not used.
      .flags = 0x0,
      .queue_size = 2,
      .pre_cb = nullptr,
      .post_cb = nullptr,
  };

  return spi_bus_add_device(spi_host_, &devcfg, &spi_device_);
}

esp_err_t APA102::Set(Color color) {
  LEDFrames* frame = static_cast<LEDFrames*>(led_frame_);
  frame->color = color;
  trans_desc_.length = sizeof(LEDFrames) * kBitsPerByte,
  trans_desc_.tx_buffer = led_frame_;

  ESP_LOGW(TAG, "Setting RGB LED to (%u, %u, %u) %u", color.red, color.green,
           color.blue, color.intensity);

  esp_err_t err =
      spi_device_queue_trans(spi_device_, &trans_desc_, portMAX_DELAY);

  ESP_LOGW(TAG, "SPI xfer done: %s", esp_err_to_name(err));
  return err;
}
