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

using namespace adp5589;

namespace {
constexpr char TAG[] = "Keyboard";
constexpr uint8_t kSlaveAddress = 0x34;  // I2C address of ADP5589 IC.
constexpr i2c::Address::Size kI2CAddressSize = i2c::Address::Size::bit7;
}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)), key_states_(0xFF, false) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Reset() {
  ESP_LOGD(TAG, "Resetting keyboard");

  Register_ID reg_id;
  esp_err_t err = ReadByte(Register::GENERAL_CFG_B, &reg_id);
  if (err != ESP_OK)
    return err;
  ESP_LOGI(TAG, "Keyboard mfr: %u, rev: %u", reg_id.MAN, reg_id.REV);

  return ESP_OK;
}

esp_err_t Keyboard::Initialize() {
  ESP_LOGD(TAG, "Initializing keyboard");

  esp_err_t err;

  Register_GENERAL_CFG_B general_config;
  err = ReadByte(Register::GENERAL_CFG_B, &general_config);
  if (err != ESP_OK)
    return err;
  general_config.LCK_TRK_LOGIC = false;
  general_config.LCK_TRK_GPI = false;
  err = WriteByte(Register::GENERAL_CFG_B, general_config);
  if (err != ESP_OK)
    return err;

  Register_INT_EN int_en;
  int_en.LOGIC2_IEN = 0;
  int_en.LOGIC1_IEN = 0;
  int_en.EVENT_IEN = 1;
  int_en.GPI_IEN = 0;
  int_en.OVRFLOW_IEN = 0;
  err = WriteByte(Register::INT_EN, int_en);
  if (err != ESP_OK)
    return err;

  ESP_LOGI(TAG, "Keyboard initialized.");
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
  return i2c_master_.ReadRegister(kSlaveAddress, kI2CAddressSize,
                                  static_cast<uint8_t>(reg),
                                  reinterpret_cast<uint8_t*>(value))
             ? ESP_OK
             : ESP_FAIL;
}

esp_err_t Keyboard::WriteByte(Register reg, uint8_t value) {
  return i2c_master_.WriteRegister(kSlaveAddress, kI2CAddressSize,
                                   static_cast<uint8_t>(reg), value)
             ? ESP_OK
             : ESP_FAIL;
}
