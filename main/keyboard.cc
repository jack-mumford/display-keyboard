#include "keyboard.h"

#include <cstdint>
#include <cstring>
#include <utility>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <class/hid/hid.h>
#include <esp_err.h>
#include <esp_log.h>
#include <i2clib/operation.h>

#include "gpio_pins.h"
#include "lm8330_registers.h"

namespace {
constexpr char TAG[] = "Keyboard";
constexpr uint8_t kSlaveAddress = 0x44;  // I2C address of LM8330 IC.
constexpr i2c::AddressMode kI2CAddressMode = i2c::AddressMode::bit7;
constexpr uint8_t k12msec = 0x80;
constexpr uint8_t kInvalidEventCode = 0x7F;
}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)), key_states_(0xFF, false) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Initialize() {
  ESP_LOGD(TAG, "Initializing keyboard");

#if 1
  constexpr gpio_config_t config = {
      .pin_bit_mask = (1UL << kKeyboardResetGPIO),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_ENABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  esp_err_t err = gpio_config(&config);
  if (err != ESP_OK)
    return err;

  // Reset is active low.
  ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_level(kKeyboardResetGPIO, 1));
  vTaskDelay(pdMS_TO_TICKS(200));
  ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_level(kKeyboardResetGPIO, 0));
  vTaskDelay(pdMS_TO_TICKS(200));
  ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_set_level(kKeyboardResetGPIO, 1));
  vTaskDelay(pdMS_TO_TICKS(200));
#endif

#if 0
for (int i = 0; i < 0x89; i++) {
  if (i2c_master_.Ping(i))
    ESP_LOGW(TAG, "Echo from 0x%x", i);
}
#endif

  err = WriteByte(Register::KBDSETTLE, k12msec);
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::KBDBOUNCE, k12msec);
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::KBDSIZE, Register_KBDSIZE{
                                         .ROWSIZE = 8,
                                         .COLSIZE = 8,
                                     });
  if (err != ESP_OK)
    return err;
  err = WriteWord(Register::KBDDEDCFG0, Register_KBDDEDCFG{
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
  err = WriteByte(Register::IOCFG, Register_IOCFG{
                                       .Reserved1 = 0,
                                       .GPIOSEL = 1,
                                       .Reserved2 = 0,
                                       .BALLCFG = 1,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteWord(Register::IOPC0, Register_IOCFG{
                                       .Reserved1 = 0,
                                       .GPIOSEL = 0,
                                       .Reserved2 = 0,
                                       .BALLCFG = 0,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteWord(Register::IOPC1, Register_IOPC1{
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
  err = WriteByte(Register::KBDIC, Register_KBDIC{
                                       .SFOFF = false,
                                       .Reserved = 0,
                                       .EVTIC = true,
                                       .KBDIC = true,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::KBDMSK, Register_KBDMSK{
                                        .Reserved = 0,
                                        .MSKELINT = 0,
                                        .MSKEINT = 0,
                                        .MSKLINT = 1,
                                        .MSKSINT = 1,
                                    });
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::CLKEN, Register_CLKEN{
                                       .Reserved1 = 0,
                                       .TIMEN = false,
                                       .Reserved2 = 0,
                                       .KBDEN = true,

                                   });
  if (err != ESP_OK)
    return err;

  ESP_LOGD(TAG, "keyboard successfully initialized");
  return ESP_OK;
}

esp_err_t Keyboard::ReportHIDEvents() {
  return ESP_OK;
}

esp_err_t Keyboard::HandleEvents() {
  event_number_++;
  return ESP_OK;
}

esp_err_t Keyboard::ReadByte(Register reg, void* value) {
  return i2c_master_.ReadRegister(kSlaveAddress, kI2CAddressMode,
                                  static_cast<uint8_t>(reg),
                                  reinterpret_cast<uint8_t*>(value))
             ? ESP_OK
             : ESP_FAIL;
}

esp_err_t Keyboard::WriteByte(Register reg, uint8_t value) {
  return i2c_master_.WriteRegister(kSlaveAddress, kI2CAddressMode,
                                   static_cast<uint8_t>(reg), value)
             ? ESP_OK
             : ESP_FAIL;
}

esp_err_t Keyboard::WriteWord(Register reg, uint16_t value) {
  i2c::Operation op =
      i2c_master_.CreateWriteOp(kSlaveAddress, kI2CAddressMode,
                                static_cast<uint8_t>(reg), "Kbd::WriteWord");
  if (!op.ready())
    return ESP_FAIL;
  return op.Write(&value, sizeof(value)) ? ESP_OK : ESP_FAIL;
}
