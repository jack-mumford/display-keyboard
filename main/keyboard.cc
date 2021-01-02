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
constexpr uint8_t kSlaveAddress = 0x34;  // I2C address of TCA8418 IC.

// clang-format off
enum class Register : uint8_t {
  Reserved       = 0x00,
  CFG            = 0x01,
  INT_STAT       = 0x02,
  KEY_LCK_EC     = 0x03,
  KEY_EVENT_A    = 0x04,
  KEY_EVENT_B    = 0x05,
  KEY_EVENT_C    = 0x06,
  KEY_EVENT_D    = 0x07,
  KEY_EVENT_E    = 0x08,
  KEY_EVENT_F    = 0x09,
  KEY_EVENT_G    = 0x0A,
  KEY_EVENT_H    = 0x0B,
  KEY_EVENT_I    = 0x0C,
  KEY_EVENT_J    = 0x0D,
  KP_LCK_TIMER   = 0x0E,
  UNLOCK1        = 0x0F,
  UNLOCK2        = 0x10,
  GPIO_INT_STAT1 = 0x11,
  GPIO_INT_STAT2 = 0x12,
  GPIO_INT_STAT3 = 0x13,
  GPIO_DAT_STAT1 = 0x14,
  GPIO_DAT_STAT2 = 0x15,
  GPIO_DAT_STAT3 = 0x16,
  GPIO_DAT_OUT1  = 0x17,
  GPIO_DAT_OUT2  = 0x18,
  GPIO_DAT_OUT3  = 0x19,
  GPIO_INT_EN1   = 0x1A,
  GPIO_INT_EN2   = 0x1B,
  GPIO_INT_EN3   = 0x1C,
  KP_GPIO1       = 0x1D,
  KP_GPIO2       = 0x1D,
  KP_GPIO3       = 0x1F,

};
// clang-format on

struct Register_CFG {
  uint8_t KE_IEN : 1;        // Key events interrupt enable to host processor.
  uint8_t GPI_IEN : 1;       // GPI interrupt enable to host processor.
  uint8_t K_LCK_IEN : 1;     // Keypad lock interrupt enable.
  uint8_t OVR_FLOW_IEN : 1;  // Overflow interrupt enable.
  uint8_t INT_CFG : 1;       // Interrupt configuration.
  uint8_t OVR_FLOW_M : 1;    // Overflow mode.
  uint8_t GPI_E_CFG : 1;     // GPI event mode configuration.
  uint8_t AI : 1;            // Auto-increment for read and write operations.
};

struct Register_INT_STAT {
  uint8_t K_INT : 1;         // Key events interrupt status.
  uint8_t GPI_INT : 1;       // GPI interrupt status.
  uint8_t K_LCK_INT : 1;     // Keypad lock interrupt status.
  uint8_t OVR_FLOW_INT : 1;  // Overflow interrupt status.
  uint8_t CAD_INT : 1;       // CTRL-ALT-DEL key sequence status.
  uint8_t Reserved : 3;      // Always zero.
};

struct Register_KEY_LCK_EC {
  uint8_t KEC : 4;       // How many key events are in the FIFO.
  uint8_t LCK1 : 1;      // Keypad lock status.
  uint8_t LCK2 : 1;      // Keypad lock status.
  uint8_t K_LCK_EN : 1;  // Key lock enable.
  uint8_t Reserved : 1;  // Always 0.
};

struct Register_KEY_EVENT_A {
  uint8_t key_code;
};

static_assert(sizeof(Register_CFG) == sizeof(uint8_t));
static_assert(sizeof(Register_INT_STAT) == sizeof(uint8_t));
static_assert(sizeof(Register_KEY_LCK_EC) == sizeof(uint8_t));

esp_err_t ReadRegister(i2c::Master& i2c_master, Register reg, void* val) {
  return i2c_master.ReadRegister(kSlaveAddress, static_cast<uint8_t>(reg),
                                 static_cast<uint8_t*>(val))
             ? ESP_OK
             : ESP_FAIL;
}

esp_err_t WriteRegister(i2c::Master& i2c_master,
                        Register reg,
                        const void* val) {
  return i2c_master.WriteRegister(kSlaveAddress, static_cast<uint8_t>(reg),
                                  *static_cast<const uint8_t*>(val))
             ? ESP_OK
             : ESP_FAIL;
}

}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Initialize() {
  constexpr Register_CFG reg_config = {
      .KE_IEN = true,
      .GPI_IEN = false,
      .K_LCK_IEN = false,
      .OVR_FLOW_IEN = false,
      .INT_CFG = true,
      .OVR_FLOW_M = false,
      .GPI_E_CFG = false,
      .AI = false,
  };

  esp_err_t err = WriteRegister(i2c_master_, Register::CFG, &reg_config);
  if (err != ESP_OK)
    return err;

#if 1
  const uint8_t int_status = 0xff;  // Interrupts on all keys.

#if 0
  for (uint8_t reg = 0x01; reg <= 0x10; reg++)
    WriteRegister(i2c_master_, (Register)reg, &int_status);
#endif

#if 1
  for (uint8_t reg = 0x11; reg <= 0x22; reg++)
    WriteRegister(i2c_master_, (Register)reg, &int_status);
  for (uint8_t reg = 0x2C; reg <= 0x2E; reg++)
    WriteRegister(i2c_master_, (Register)reg, &int_status);
#else
  err = WriteRegister(i2c_master_, Register::GPIO_INT_EN1, &int_status);
  if (err != ESP_OK)
    return err;
  err = WriteRegister(i2c_master_, Register::GPIO_INT_EN2, &int_status);
  if (err != ESP_OK)
    return err;
  err = WriteRegister(i2c_master_, Register::GPIO_INT_EN3, &int_status);
  if (err != ESP_OK)
    return err;
#endif
#endif

  return ESP_OK;
}

esp_err_t Keyboard::LogEventCount() {
  Register_INT_STAT int_stat;
  esp_err_t err = ReadRegister(i2c_master_, Register::INT_STAT, &int_stat);
  if (err != ESP_OK)
    return err;
  Register_KEY_LCK_EC key_lck_ec;
  err = ReadRegister(i2c_master_, Register::KEY_LCK_EC, &key_lck_ec);
  if (err != ESP_OK)
    return err;
  const bool had_keyboard_event = int_stat.K_INT || int_stat.GPI_INT;
  if (had_keyboard_event) {
    ESP_LOGI(TAG, "Had keyboard event");
  } else {
    ESP_LOGI(TAG, "No keyboard event");
  }
  ESP_LOGI(TAG, "Key event count: %u", key_lck_ec.KEC);

  int_stat.K_INT = false;
  WriteRegister(i2c_master_, Register::INT_STAT, &int_stat);

  return ESP_OK;
}