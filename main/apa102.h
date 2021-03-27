#pragma once

#include <cstdint>

#include <driver/spi_master.h>
#include <esp_err.h>
#include <hal/gpio_types.h>
#include <hal/spi_types.h>

/**
 * Control a single APA102 2020 RGB LED.
 *
 * http://www.led-color.com/upload/201604/APA102-2020%20SMD%20LED.pdf
 */
class APA102 {
 public:
  struct Color {
    uint32_t unused : 3;
    uint32_t intensity : 5;
    uint32_t blue : 8;
    uint32_t green : 8;
    uint32_t red : 8;

    Color() : Color(0, 0, 0) {}
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t intensity = 31);
  };

  APA102(gpio_num_t sclk_gpio,
         gpio_num_t mosi_gpio,
         spi_host_device_t spi_host);
  ~APA102();

  esp_err_t Initlialize();
  esp_err_t Set(Color color);

 private:
  const gpio_num_t sclk_gpio_;
  const gpio_num_t mosi_gpio_;
  const spi_host_device_t spi_host_;
  spi_device_handle_t spi_device_;
  void* led_frame_;
  spi_transaction_t trans_desc_;
};
