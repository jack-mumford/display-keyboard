#pragma once

#include <cstdint>

#include <driver/spi_master.h>
#include <esp_err.h>
#include <hal/gpio_types.h>

// Colors are (brightness, blue, green, red).
typedef uint32_t APAColor;

class APA102 {
 public:
  APA102(gpio_num_t sclk_gpio, gpio_num_t mosi_gpio);
  ~APA102();

  esp_err_t Initlialize();
  esp_err_t Set(APAColor color);

 private:
  const gpio_num_t sclk_gpio_;
  const gpio_num_t mosi_gpio_;
  spi_device_handle_t spi_device_;
  void* led_frame_;
};
