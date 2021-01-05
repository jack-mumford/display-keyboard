#include "keyboard.h"

#include <cstdint>
#include <cstring>
#include <utility>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>

#if !defined(SET_BITS)
#define SET_BITS(value, bits) (value |= (bits))
#endif

namespace {
constexpr char TAG[] = "kbd_kbd";
constexpr uint8_t kSlaveAddress = 0x34;  // I2C address of LM8330 IC.
}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Initialize() {
  return ESP_OK;
}

esp_err_t Keyboard::LogEventCount() {
  return ESP_OK;
}
