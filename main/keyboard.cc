#include "keyboard.h"

#include <cstdint>
#include <cstring>
#include <utility>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <class/hid/hid.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_task_wdt.h>
#include <i2clib/operation.h>

#include "adp5589_registers.h"
#include "gpio_pins.h"

namespace {
constexpr char TAG[] = "Keyboard";
constexpr uint8_t kSlaveAddress = 0x34;  // I2C address of ADP5589 IC.
constexpr i2c::Address::Size kI2CAddressSize = i2c::Address::Size::bit7;
}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)), key_states_(0xFF, false) {}

Keyboard::~Keyboard() = default;

// static
esp_err_t Keyboard::Reset() {
#if 1
  return ESP_OK;
#else
  constexpr gpio_config_t reset_pin_config = {
      .pin_bit_mask = (1ULL << kKeyboardResetGPIO),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  esp_err_t err = gpio_config(&reset_pin_config);
  if (err != ESP_OK)
    return err;

  ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_level(kKeyboardResetGPIO, 1));
  vTaskDelay(pdMS_TO_TICKS(100));

  // Reset is active low.
  ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_level(kKeyboardResetGPIO, 0));
  vTaskDelay(pdMS_TO_TICKS(100));
  ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_level(kKeyboardResetGPIO, 1));
  vTaskDelay(pdMS_TO_TICKS(100));

  return ESP_OK;
#endif
}

esp_err_t Keyboard::Initialize() {
  ESP_LOGD(TAG, "Initializing keyboard");

#if 0
  return ESP_OK;
#endif

#if 1
  Register_GENERAL_CFG_B config;
  return ReadByte(ADPRegister::GENERAL_CFG_B, &config);
#endif

#if 0
  for (int i = 1; i < 128; i++) {
    if (i2c_master_.Ping(i, i2c::Address::Size::bit7)) {
      ESP_LOGW(TAG, "Echo from 0x%x", i);
      return ESP_OK;
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
  return ESP_FAIL;
#endif

#if 0
  esp_err_t err = WriteByte(ADPRegister::KBDSETTLE, k12msec);
  if (err != ESP_OK)
    return err;
  err = WriteByte(ADPRegister::KBDBOUNCE, k12msec);
  if (err != ESP_OK)
    return err;
  err = WriteByte(ADPRegister::KBDSIZE, Register_KBDSIZE{
                                         .ROWSIZE = 8,
                                         .COLSIZE = 8,
                                     });
  if (err != ESP_OK)
    return err;
  err = WriteWord(ADPRegister::KBDDEDCFG0, Register_KBDDEDCFG{
                                            .KPX7 = 1,
                                            .KPX6 = 1,
                                            .KPX5 = 1,
                                            .KPX4 = 1,
                                            .KPX3 = 1,
                                            .KPX2 = 1,
                                            .KPY11 = 1,
                                            .KPY10 = 1,
                                            .KPY9 = 1,
                                            .KPY8 = 1,
                                            .KPY7 = 1,
                                            .KPY6 = 1,
                                            .KPY5 = 1,
                                            .KPY4 = 1,
                                            .KPY3 = 1,
                                            .KPY2 = 1,
                                        });
  if (err != ESP_OK)
    return err;
  err = WriteByte(ADPRegister::IOCFG, Register_IOCFG{
                                       .Reserved1 = 0,
                                       .GPIOSEL = 1,
                                       .Reserved2 = 0,
                                       .BALLCFG = 1,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteWord(ADPRegister::IOPC0, Register_IOCFG{
                                       .Reserved1 = 0,
                                       .GPIOSEL = 0,
                                       .Reserved2 = 0,
                                       .BALLCFG = 0,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteWord(ADPRegister::IOPC1, Register_IOPC1{
                                       .KPY7PR = Pull::Down,
                                       .KPY6PR = Pull::Down,
                                       .KPY5PR = Pull::Down,
                                       .KPY4PR = Pull::Down,
                                       .KPY3PR = Pull::Down,
                                       .KPY2PR = Pull::Down,
                                       .KPY1PR = Pull::Down,
                                       .KPY0PR = Pull::Down,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteByte(ADPRegister::KBDIC, Register_KBDIC{
                                       .SFOFF = false,
                                       .Reserved = 0,
                                       .EVTIC = true,
                                       .KBDIC = true,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteByte(ADPRegister::KBDMSK, Register_KBDMSK{
                                        .Reserved = 0,
                                        .MSKELINT = 0,
                                        .MSKEINT = 0,
                                        .MSKLINT = 1,
                                        .MSKSINT = 1,
                                    });
  if (err != ESP_OK)
    return err;
  err = WriteByte(ADPRegister::CLKEN, Register_CLKEN{
                                       .Reserved1 = 0,
                                       .TIMEN = false,
                                       .Reserved2 = 0,
                                       .KBDEN = true,

                                   });
  if (err != ESP_OK)
    return err;

  ESP_LOGD(TAG, "keyboard successfully initialized");
  return ESP_OK;
#endif
}

esp_err_t Keyboard::ReportHIDEvents() {
  return ESP_OK;
}

esp_err_t Keyboard::HandleEvents() {
  event_number_++;
  return ESP_OK;
}

esp_err_t Keyboard::ReadByte(ADPRegister reg, void* value) {
  return i2c_master_.ReadRegister(kSlaveAddress, kI2CAddressSize,
                                  static_cast<uint8_t>(reg),
                                  reinterpret_cast<uint8_t*>(value))
             ? ESP_OK
             : ESP_FAIL;
}

esp_err_t Keyboard::WriteByte(ADPRegister reg, uint8_t value) {
  return i2c_master_.WriteRegister(kSlaveAddress, kI2CAddressSize,
                                   static_cast<uint8_t>(reg), value)
             ? ESP_OK
             : ESP_FAIL;
}

esp_err_t Keyboard::WriteWord(ADPRegister reg, uint16_t value) {
  i2c::Operation op =
      i2c_master_.CreateWriteOp(kSlaveAddress, kI2CAddressSize,
                                static_cast<uint8_t>(reg), "Kbd::WriteWord");
  if (!op.ready())
    return ESP_FAIL;
  return op.Write(&value, sizeof(value)) ? ESP_OK : ESP_FAIL;
}
