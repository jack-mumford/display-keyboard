#include "keyboard.h"

#include <array>
#include <cstdint>
#include <cstring>
#include <utility>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <class/hid/hid.h>
#include <esp_err.h>
#include <esp_log.h>
#include <esp_task_wdt.h>
#include <i2clib/address.h>
#include <i2clib/master.h>
#include <i2clib/operation.h>
#include <kbdlib/lm8330.h>

#include "gpio_pins.h"
#include "usb_hid.h"

using i2c::Address;
using i2c::Operation;
using kbd::lm8330::kSlaveAddress;
using kbd::lm8330::RegNum;

namespace {
constexpr char TAG[] = "Keyboard";
constexpr Address::Size kI2CAddressSize = i2c::Address::Size::bit7;

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

esp_err_t ResetKeyboard(gpio_num_t reset_pin) {
  esp_err_t err = gpio_set_level(reset_pin, 0);
  if (err != ESP_OK)
    return err;

  // Typical RESETN glith filter is 100 nS, so this delay
  // is more than sufficient.
  constexpr uint32_t kResetDelayMs = 2;

  vTaskDelay(pdMS_TO_TICKS(2));
  err = gpio_set_level(reset_pin, kResetDelayMs);
  if (err != ESP_OK)
    return err;

  // Not sure how long the chip takes after reset to be ready.
  // Choose some reasonably long time.
  constexpr uint32_t kStartupTimeMs = 50;
  vTaskDelay(pdMS_TO_TICKS(kStartupTimeMs));
  return ESP_OK;
}
}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)), key_states_({false}) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Reset() {
  ESP_LOGD(TAG, "Resetting keyboard");

  constexpr gpio_config_t config = {
      .pin_bit_mask = (1ULL << kKeyboardResetGPIO),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  esp_err_t err = gpio_config(&config);
  if (err != ESP_OK)
    return err;
  err = ResetKeyboard(kKeyboardResetGPIO);
  if (err != ESP_OK)
    return err;

  return ESP_OK;
}

esp_err_t Keyboard::InitializeInterrupts(i2c::Operation& op) {
#if 0
  if (!op.RestartReg(static_cast<uint8_t>(RegNum::INT_EN),
                     Address::Mode::WRITE)) {
    return ESP_FAIL;
  }
  constexpr kbd::lm8330::reg::INT_EN reg = {
      .Reserved = 0,
      .LOGIC2_IEN = false,
      .LOGIC1_IEN = false,
      .LOCK_IEN = false,
      .OVRFLOW_IEN = true,
      .GPI_IEN = false,
      .EVENT_IEN = true,
  };
  if (!op.WriteByte(reg))
    return ESP_FAIL;
#endif
  return ESP_OK;
}

// Initialize the keyboard. See KEYSCAN INITIALIZATION in
// LM8330 datasheet.
esp_err_t Keyboard::Initialize() {
  ESP_LOGD(TAG, "Initializing keyboard");

  uint8_t reg_mfgcode;

  Operation op = i2c_master_.CreateReadOp(kSlaveAddress, kI2CAddressSize,
                                          static_cast<uint8_t>(RegNum::MFGCODE),
                                          "kbdinit");
  if (!op.ready())
    return ESP_FAIL;

  if (!op.Read(&reg_mfgcode, sizeof(reg_mfgcode)))
    return ESP_FAIL;

  {
    op.RestartReg(static_cast<uint8_t>(RegNum::CLKEN), Address::Mode::WRITE);
    constexpr kbd::lm8330::reg::CLKEN clken = {
        .Reserved1 = 0,
        .TIMEN = 0,
        .Reserved2 = 0,
        .KBDEN = 1,
    };
    if (!op.WriteByte(clken))
      return ESP_FAIL;
  }

  if (!op.Execute())
    return ESP_FAIL;

  ESP_LOGI(TAG, "Keyboard initialized, mfgcode: 0x%x", reg_mfgcode);
  return ESP_OK;
}

esp_err_t Keyboard::ReportHIDEvents() {
  uint8_t next_key_idx = 0;
  uint8_t keycodes[6] = {HID_KEY_NONE};
  uint8_t modifier = 0;
  uint8_t num_pressed_keys = 0;  // number of non-modifier keys.

  for (uint8_t keycode = 0; keycode < key_states_.size(); keycode++) {
    if (!key_states_[keycode])
      continue;
    uint8_t modifier_bit = 0 /*GetModifierFlag(keycode)*/;
    if (modifier_bit) {
      modifier |= modifier_bit;
    } else {
      num_pressed_keys++;
      if (next_key_idx < ARRAY_SIZE(keycodes))
        keycodes[next_key_idx++] = keycode;
    }
  }

  if (num_pressed_keys > ARRAY_SIZE(keycodes)) {
    ESP_LOGW(TAG, "Unable to report all keys. %u pressed, max %u",
             num_pressed_keys, ARRAY_SIZE(keycodes));
  }
  return usb::HID::KeyboardReport(usb::REPORT_ID_KEYBOARD, modifier, keycodes);
}

esp_err_t Keyboard::HandleEvents() {
#if 1
  return ESP_OK;
#else
  esp_err_t err;

  ESP_LOGV(TAG, "Reading keyboard events.");

  kbd::lm8330::reg::INT_STATUS interrupt_status;
  err = Read(&interrupt_status);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failure reading INT_STATUS");
    return err;
  }
  // Write back the INT_STATUS register to clear the INT flags.
  err = WriteByte(RegNum::INT_STATUS, interrupt_status);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failure writing INT_STATUS");
  }
  if (interrupt_status.OVRFLOW_INT) {
    ESP_LOGE(TAG, "Keyboard FIFO overflow.");
    return ESP_FAIL;
  }

  kbd::lm8330::reg::Status status_reg;
  err = Read(&status_reg);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failure reading status register");
    return err;
  }

  if (!status_reg.EC) {
    ESP_LOGV(TAG, "No keyboard events.");
    return ESP_OK;
  }

  constexpr uint8_t kMaxFIFOEntries = 16;
  kbd::lm8330::reg::FIFO fifo[kMaxFIFOEntries];

  for (uint8_t i = 0; i < status_reg.EC; i++) {
    event_number_++;
    err = Read(&fifo[i]);
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "Error reading FIFO entry %u", i);
      return err;
    }
  }

  for (uint8_t i = 0; i < status_reg.EC; i++) {
#ifdef ONLY_LOG_EVENTS
    const char key_state = fifo[i].Event_State ? 'D' : 'U';
    ESP_LOGI(TAG, "keyboard event[%u/%u] (0x%02x) id %s:%c", i + 1,
             status_reg.EC, fifo[i].Event_State,
             lm8330::EventToName(static_cast<EventID>(fifo[i].IDENTIFIER)),
             key_state);
#else
    const uint8_t hid_keycode = KeyboardKeyIDToHIDKeyCode(fifo[i].IDENTIFIER);
    if (hid_keycode < key_states_.size()) {
      const bool key_pressed = fifo[i].Event_State & 0b10000000;
      key_states_[hid_keycode] = key_pressed;
    }
#endif
  }

  ESP_LOGV(TAG, "    Done reading keyboard.");
  return ReportHIDEvents();
#endif
}

esp_err_t Keyboard::ReadByte(RegNum reg, uint8_t* value) {
  return i2c_master_.ReadRegister(kSlaveAddress, kI2CAddressSize,
                                  static_cast<uint8_t>(reg), value)
             ? ESP_OK
             : ESP_FAIL;
}

esp_err_t Keyboard::WriteByte(RegNum reg, uint8_t value) {
  return i2c_master_.WriteRegister(kSlaveAddress, kI2CAddressSize,
                                   static_cast<uint8_t>(reg), value)
             ? ESP_OK
             : ESP_FAIL;
}
