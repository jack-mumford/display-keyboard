#include "keyboard.h"

#include <cstdint>
#include <cstring>
#include <utility>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>

#if !defined(SET_BITS)
#define SET_BITS(value, bits) (value |= (bits))
#endif

struct RegisterCFG {
  uint8_t KE_IEN : 1;
  uint8_t GPI_IEN : 1;
  uint8_t K_LCK_IEN : 1;
  uint8_t OVR_FLOW_IEN : 1;
  uint8_t INT_CFG : 1;
  uint8_t OVR_FLOW_M : 1;
  uint8_t GPI_E_CFG : 1;
  uint8_t AI : 1;
};

namespace {
constexpr char TAG[] = "kbd_kbd";
constexpr uint8_t kSlaveAddress = 0x34;  // I2C address of TCA8418 IC.

// clang-format off
enum class Register : uint8_t {
  Reserved     = 0x00,
  CFG          = 0x01,
  INT_STAT     = 0x02,
  KEY_LCK_EC   = 0x03,
  KEY_EVENT_A  = 0x04,
  KEY_EVENT_B  = 0x05,
  KEY_EVENT_C  = 0x06,
  KEY_EVENT_D  = 0x07,
  KEY_EVENT_E  = 0x08,
  KEY_EVENT_F  = 0x09,
  KEY_EVENT_G  = 0x0A,
  KEY_EVENT_H  = 0x0B,
  KEY_EVENT_I  = 0x0C,
  KEY_EVENT_J  = 0x0D,
  KP_LCK_TIMER = 0x0E,
  UNLOCK1      = 0x0F,
  UNLOCK2      = 0x10
};
// clang-format on

static_assert(sizeof(RegisterCFG) == sizeof(uint8_t));
}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Initialize() {
  esp_err_t err = WriteConfig();
  if (err != ESP_OK)
    return err;

  RegisterCFG config;
  err = ReadConfig(&config);
  if (err != ESP_OK)
    return err;

  return ESP_OK;
}

esp_err_t Keyboard::ReadConfig(RegisterCFG* config) {
  uint8_t value;
  if (!i2c_master_.ReadRegister(kSlaveAddress,
                                static_cast<uint8_t>(Register::CFG), &value))
    return ESP_FAIL;

  std::memcpy(config, &value, sizeof(RegisterCFG));
  return ESP_OK;
}

esp_err_t Keyboard::WriteConfig() {
  constexpr RegisterCFG config = {
      .KE_IEN = true,
      .GPI_IEN = false,
      .K_LCK_IEN = false,
      .OVR_FLOW_IEN = false,
      .INT_CFG = false,
      .OVR_FLOW_M = false,
      .GPI_E_CFG = false,
      .AI = false,
  };

  uint8_t value;
  std::memcpy(&value, &config, sizeof(value));
  return i2c_master_.WriteRegister(kSlaveAddress,
                                   static_cast<uint8_t>(Register::CFG), value)
             ? ESP_OK
             : ESP_FAIL;
}
