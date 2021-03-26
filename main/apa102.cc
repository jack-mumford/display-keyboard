#include "apa102.h"

#include <cstring>

#include <esp_log.h>
#include <hal/spi_types.h>

namespace {

struct LEDFrame {
  uint32_t frame_start_indicator;  // 0x000000.
  APAColor color;                  // The LED color value.
  uint32_t frame_end_indicator;    // 0xffffffff
};

constexpr uint8_t kBytesPerPixel = 4;
constexpr int kMaxSPIFrameBytes = 256;
constexpr int kSPIClockFrequencyHz = 1000000;
constexpr spi_host_device_t kSPIHost = HSPI_HOST;

constexpr char TAG[] = "APA102";

}  // namespace

APA102::APA102(gpio_num_t sclk_gpio, gpio_num_t mosi_gpio)
    : sclk_gpio_(sclk_gpio),
      mosi_gpio_(mosi_gpio),
      spi_device_(nullptr),
      led_frame_(nullptr) {}

APA102::~APA102() {
  if (spi_device_)
    spi_bus_remove_device(spi_device_);
  if (led_frame_)
    heap_caps_free(led_frame_);
}

esp_err_t APA102::Initlialize() {
  static_assert(sizeof(LEDFrame) == 12);
  led_frame_ = heap_caps_malloc(sizeof(LEDFrame), MALLOC_CAP_DMA);
  LEDFrame* frame = static_cast<LEDFrame*>(led_frame_);
  frame->frame_start_indicator = 0x0;
  frame->color = 0x0;
  frame->frame_start_indicator = 0xffffffff;

  const spi_bus_config_t buscfg = {
      .mosi_io_num = mosi_gpio_,
      .miso_io_num = -1,  // not used.
      .sclk_io_num = sclk_gpio_,
      .quadwp_io_num = -1,  // not used.
      .quadhd_io_num = -1,  // not used.
      .max_transfer_sz = kMaxSPIFrameBytes,
      .flags = 0x0,
      .intr_flags = SPICOMMON_BUSFLAG_MASTER,
  };

  esp_err_t err = spi_bus_initialize(kSPIHost, &buscfg, SPI_DMA_CH_AUTO);
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

  return spi_bus_add_device(kSPIHost, &devcfg, &spi_device_);
}

esp_err_t APA102::Set(APAColor color) {
  LEDFrame* frame = static_cast<LEDFrame*>(led_frame_);
  frame->color = color;
  spi_transaction_t trans_desc = {
      .flags = 0,
      .cmd = 0,
      .addr = 0,
      .length = sizeof(LEDFrame) * sizeof(uint8_t),  // # of bits.
      .rxlength = 0,
      .user = nullptr,
      .tx_buffer = led_frame_,
      .rx_buffer = nullptr,
  };

  ESP_LOGW(TAG, "Setting RGB LED to 0x%08x.", color);
  esp_err_t err =
      spi_device_queue_trans(spi_device_, &trans_desc, portMAX_DELAY);
  ESP_LOGW(TAG, "SPI xfer done: %s", esp_err_to_name(err));
  return err;
}
