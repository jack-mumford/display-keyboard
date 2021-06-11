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

#include "adp5589.h"
#include "gpio_pins.h"
#include "usb_hid.h"

using namespace adp5589;

namespace {
constexpr char TAG[] = "Keyboard";
constexpr uint8_t kSlaveAddress = 0x34;  // I2C address of ADP5589 IC.
constexpr i2c::Address::Size kI2CAddressSize = i2c::Address::Size::bit7;

static_assert(sizeof(Register_FIFO) == sizeof(uint8_t));
static_assert(sizeof(Register_INT_EN) == sizeof(uint8_t));

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/**
 * @brief Convert an ADP5589 Keyboard event ID to a TinyUSB HID keycode.
 *
 * See ADP5589 datasheet table 11 for description of EventID values.
 *
 * @param event_id The code returned by the keyboard IC.
 *
 * @return The TinyUSB HID keycode. HID_KEY_NONE if no mapping.
 */
uint8_t KeyboardKeyIDToHIDKeyCode(EventID event_id) {
  // Hard-coded. Will replace with a configuration which comes from
  // a config file at some point.
  switch (event_id) {
    case EventID::R0_C3:
      return HID_KEY_SHIFT_LEFT;
    case EventID::R0_C2:
      return HID_KEY_Q;
    case EventID::R0_C1:
      return HID_KEY_W;
    case EventID::R0_C0:
      return HID_KEY_E;
    case EventID::R1_C3:
      return HID_KEY_CONTROL_LEFT;
    case EventID::R1_C2:
      return HID_KEY_A;
    case EventID::R1_C1:
      return HID_KEY_S;
    case EventID::R1_C0:
      return HID_KEY_D;
    default:
      break;
  }
  return HID_KEY_NONE;
}

esp_err_t ResetKeyboard(gpio_num_t reset_pin) {
  esp_err_t err = gpio_set_level(reset_pin, 0);
  if (err != ESP_OK)
    return err;

  vTaskDelay(pdMS_TO_TICKS(30));
  err = gpio_set_level(reset_pin, 1);
  if (err != ESP_OK)
    return err;

  vTaskDelay(pdMS_TO_TICKS(30));
  return ESP_OK;
}

}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)), key_states_(0xFF, false) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Reset() {
  ESP_LOGD(TAG, "Resetting keyboard");

  const gpio_config_t config = {
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

  Register_ID reg_id;
  err = Read(&reg_id);
  if (err != ESP_OK)
    return err;
  ESP_LOGI(TAG, "Keyboard reset, mfr: %u, rev: %u", reg_id.MAN, reg_id.REV);

  return ESP_OK;
}

esp_err_t Keyboard::InitializeKeys(i2c::Operation& op) {
  constexpr Register_PIN_CONFIG_A reg_a = {
      .R7_CONFIG = 0,
      .R6_CONFIG = 0,
      .R5_CONFIG = 0,
      .R4_CONFIG = 0,
      .R3_CONFIG = 0,
      .R2_CONFIG = 0,
      .R1_CONFIG = 1,
      .R0_CONFIG = 1,
  };
  if (!op.RestartReg(static_cast<uint8_t>(Register::PIN_CONFIG_A),
                     i2c::Address::Mode::WRITE) ||
      !op.WriteByte(reg_a)) {
    return ESP_FAIL;
  }

  constexpr Register_PIN_CONFIG_B reg_b = {
      .C7_CONFIG = 0,
      .C6_CONFIG = 0,
      .C5_CONFIG = 0,
      .C4_CONFIG = 0,
      .C3_CONFIG = 1,
      .C2_CONFIG = 1,
      .C1_CONFIG = 1,
      .C0_CONFIG = 1,
  };
  if (!op.RestartReg(static_cast<uint8_t>(Register::PIN_CONFIG_B),
                     i2c::Address::Mode::WRITE) ||
      !op.WriteByte(reg_b)) {
    return ESP_FAIL;
  }

  constexpr Register_PIN_CONFIG_C reg_c = {
      .Reserved = 0,
      .C10_CONFIG = 0,
      .C9_CONFIG = 0,
      .C8_CONFIG = 0,
  };
  if (!op.RestartReg(static_cast<uint8_t>(Register::PIN_CONFIG_C),
                     i2c::Address::Mode::WRITE) ||
      !op.WriteByte(reg_c)) {
    return ESP_FAIL;
  }

  constexpr Register_PIN_CONFIG_D reg_d = {
      .PULL_SELECT = 0,    // 300 kΩ used for row pull-up during key scanning.
      .C4_EXTEND_CFG = 0,  // C4 remains configured as GPIO 13.
      .R4_EXTEND_CFG = 0,  // R4 remains configured as GPIO 5.
      .C6_EXTEND_CFG = 0,  // C6 remains configured as GPIO 15.
      .R3_EXTEND_CFG = 0,  // R3 remains configured as GPIO 4.
      .C9_EXTEND_CFG = 0,  // C9 remains configured as GPIO 18.
      .R0_EXTEND_CFG = 0,  // R0 remains configured as GPIO 1.
  };
  if (!op.RestartReg(static_cast<uint8_t>(Register::PIN_CONFIG_D),
                     i2c::Address::Mode::WRITE) ||
      !op.WriteByte(reg_d)) {
    return ESP_FAIL;
  }

  constexpr std::array<Register, 5> kRegistersToClear = {
      Register::RPULL_CONFIG_A, Register::RPULL_CONFIG_B,
      Register::RPULL_CONFIG_C, Register::RPULL_CONFIG_D,
      Register::RPULL_CONFIG_E,
  };

  for (auto reg : kRegistersToClear) {
    if (!op.RestartReg(static_cast<uint8_t>(reg), i2c::Address::Mode::WRITE) ||
        !op.WriteByte(0x0)) {
      return ESP_FAIL;
    }
  }

  return ESP_OK;
}

esp_err_t Keyboard::InitializeInterrupts(i2c::Operation& op) {
  if (!op.RestartReg(static_cast<uint8_t>(Register::INT_EN),
                     i2c::Address::Mode::WRITE)) {
    return ESP_FAIL;
  }
  constexpr Register_INT_EN reg = {
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
  return ESP_OK;
}

esp_err_t Keyboard::Initialize() {
  esp_err_t err;

  ESP_LOGD(TAG, "Initializing keyboard");

  i2c::Operation op = i2c_master_.CreateWriteOp(
      kSlaveAddress, kI2CAddressSize,
      static_cast<uint8_t>(Register::GENERAL_CFG_B), "kbd-init");
  if (!op.ready())
    return ESP_FAIL;

  {
    constexpr Register_GENERAL_CFG_B reg = {
        .OSC_EN = true,  // Enable oscillator.
        .CORE_FREQ = CoreFrequency::kHz500,
        .LCK_TRK_LOGIC = 1,  // do not track.
        .LCK_TRK_GPI = 1,    // do not track.
        .Unused = 0,
        .INT_CFG = 1,  // deassert for 50 µs, reassert if interrupt is pending.
        .RST_CFG = 0,  // ADP5589 reset if RST is low.
    };
    if (!op.WriteByte(reg))
      return ESP_FAIL;
  }

  err = InitializeInterrupts(op);
  if (err != ESP_OK)
    return err;
  err = InitializeKeys(op);
  if (err != ESP_OK)
    return err;

  constexpr std::array<Register, 5> kRegistersToClear = {
      Register::LOGIC_1_CFG,        Register::LOGIC_2_CFG,
      Register::LOGIC_INT_EVENT_EN, Register::CLOCK_DIV_CFG,
      Register::LOGIC_FF_CFG,
  };

  for (auto reg : kRegistersToClear) {
    if (!op.RestartReg(static_cast<uint8_t>(reg), i2c::Address::Mode::WRITE) ||
        !op.WriteByte(0x0)) {
      return ESP_FAIL;
    }
  }

  if (!op.Execute())
    return ESP_FAIL;

  ESP_LOGI(TAG, "Keyboard initialized.");
  return ESP_OK;
}

esp_err_t Keyboard::ReportHIDEvents() {
  uint8_t next_key_idx = 0;
  uint8_t keycodes[6] = {HID_KEY_NONE};
  uint8_t modifier = 0;
  uint8_t num_pressed_keys = 0;  // non-modifier keys.

  for (uint8_t keycode = 0; keycode < key_states_.size(); keycode++) {
    if (key_states_[keycode]) {
      if (keycode == HID_KEY_SHIFT_LEFT) {
        modifier |= KEYBOARD_MODIFIER_LEFTSHIFT;
      } else if (keycode == HID_KEY_SHIFT_RIGHT) {
        modifier |= KEYBOARD_MODIFIER_RIGHTSHIFT;
      } else if (keycode == HID_KEY_CONTROL_LEFT) {
        modifier |= KEYBOARD_MODIFIER_LEFTCTRL;
      } else if (keycode == HID_KEY_CONTROL_RIGHT) {
        modifier |= KEYBOARD_MODIFIER_RIGHTCTRL;
      } else if (keycode == HID_KEY_ALT_LEFT) {
        modifier |= KEYBOARD_MODIFIER_LEFTALT;
      } else if (keycode == HID_KEY_ALT_RIGHT) {
        modifier |= KEYBOARD_MODIFIER_RIGHTALT;
      } else if (keycode == HID_KEY_GUI_LEFT) {
        modifier |= KEYBOARD_MODIFIER_LEFTGUI;
      } else if (keycode == HID_KEY_GUI_RIGHT) {
        modifier |= KEYBOARD_MODIFIER_RIGHTGUI;
      } else {
        num_pressed_keys++;
        if (next_key_idx < ARRAY_SIZE(keycodes))
          keycodes[next_key_idx++] = keycode;
      }
    }
  }

  if (num_pressed_keys > ARRAY_SIZE(keycodes)) {
    ESP_LOGW(TAG, "Unable to report all keys. %u pressed, max %u",
             num_pressed_keys, ARRAY_SIZE(keycodes));
  }
  return usb::HID::KeyboardReport(usb::REPORT_ID_KEYBOARD, modifier, keycodes);
}

esp_err_t Keyboard::HandleEvents() {
  esp_err_t err;

  event_number_++;

  ESP_LOGV(TAG, "Reading keyboard events.");

  Register_INT_STATUS interrupt_status;
  err = Read(&interrupt_status);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failure reading INT_STATUS");
    return err;
  }
  // Write back the INT_STATUS register to clear the INT flags.
  err = WriteByte(Register::INT_STATUS, interrupt_status);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failure writing INT_STATUS");
  }
  if (interrupt_status.OVRFLOW_INT) {
    ESP_LOGE(TAG, "Keyboard FIFO overflow.");
    return ESP_FAIL;
  }

  Register_Status status_reg;
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
  Register_FIFO fifo[kMaxFIFOEntries];

  for (uint8_t i = 0; i < status_reg.EC; i++) {
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
             adp5589::EventToName(static_cast<EventID>(fifo[i].IDENTIFIER)),
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
}

esp_err_t Keyboard::Read(Register_Status* reg) {
  uint8_t b;
  esp_err_t err = ReadByte(Register::Status, &b);
  if (err != ESP_OK)
    return err;

  reg->LOGIC2_STAT = b & 0b10000000 ? 1 : 0;
  reg->LOGIC1_STAT = b & 0b01000000 ? 1 : 0;
  reg->LOCK_STAT = b & 0b00100000 ? 1 : 0;
  reg->EC = b & 0b00011111;

  return ESP_OK;
}

esp_err_t Keyboard::Read(Register_INT_STATUS* reg) {
  uint8_t b;
  esp_err_t err = ReadByte(Register::Status, &b);
  if (err != ESP_OK)
    return err;

  reg->LOGIC2_INT = b & 0b00100000 ? 1 : 0;
  reg->LOGIC1_INT = b & 0b00010000 ? 1 : 0;
  reg->LOCK_INT = b & 0b00001000 ? 1 : 0;
  reg->OVRFLOW_INT = b & 0b00000100 ? 1 : 0;
  reg->GPI_INT = b & 0b00000010 ? 1 : 0;
  reg->EVENT_INT = b & 0b00000001;

  return ESP_OK;
}

esp_err_t Keyboard::Read(Register_FIFO* reg) {
  uint8_t b;
  esp_err_t err = ReadByte(Register::FIFO_1, &b);
  if (err != ESP_OK)
    return err;

  reg->Event_State = b & 0b10000000 ? 1 : 0;
  reg->IDENTIFIER = static_cast<EventID>(b & 0b01111111);

  return ESP_OK;
}

esp_err_t Keyboard::Read(Register_ID* reg) {
  uint8_t b;
  esp_err_t err = ReadByte(Register::ID, &b);
  if (err != ESP_OK)
    return err;

  reg->MAN = b & 0xF0 >> 4;
  reg->REV = b & 0x0F;

  return ESP_OK;
}

esp_err_t Keyboard::ReadByte(Register reg, uint8_t* value) {
  return i2c_master_.ReadRegister(kSlaveAddress, kI2CAddressSize,
                                  static_cast<uint8_t>(reg), value)
             ? ESP_OK
             : ESP_FAIL;
}

esp_err_t Keyboard::WriteByte(Register reg, uint8_t value) {
  return i2c_master_.WriteRegister(kSlaveAddress, kI2CAddressSize,
                                   static_cast<uint8_t>(reg), value)
             ? ESP_OK
             : ESP_FAIL;
}
