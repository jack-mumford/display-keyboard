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
constexpr uint8_t kSlaveAddress = 0x88;  // I2C address of LM8330 IC.

// clang-format off
enum class Register : uint8_t {
  KBDSETTLE  = 0x01,
  KBDBOUNCE  = 0x02,
  KBDSIZE    = 0x03,
  KBDDEDCFG  = 0x04,
  KBDRIS     = 0x06,
  KBDMIS     = 0x07,
  KBDIC      = 0x08,
  KBDMSK     = 0x09,
  KBDCODE0   = 0x0B,
  KBDCODE1   = 0x0C,
  KBDCODE2   = 0x0D,
  KBDCODE3   = 0x0E,
  EVTCODE    = 0x10,
  TIMCFG0    = 0x60,
  PWMCFG0    = 0x61,
  TIMCFG1    = 0x68,
  PWMCFG1    = 0x69,
  TIMCFG2    = 0x70,
  PWMCFG2    = 0x71,
  TIMSWRES   = 0x78,
  TIMRIS     = 0x7A,
  TIMMIS     = 0x7B,
  TIMIC      = 0x7C,
  PWMWP      = 0x7D,
  PWMCFG     = 0x7E,
  I2CSA      = 0x80,
  MFGCODE    = 0x80,
  SWREV      = 0x81,
  RSTCTRL    = 0x82,
  RSTINTCLR  = 0x84,
  CLKMODE    = 0x88,
  CLKEN      = 0x8A,
  AUTOSLP    = 0x8B,
  AUTOSLPTIL = 0x8C,
  AUTOSLPTIH = 0x8D,
  IRQST      = 0x91,
};
// clang-format on

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
