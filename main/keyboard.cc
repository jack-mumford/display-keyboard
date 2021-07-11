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

/**
 * @brief Convert an LM8330 Keyboard event code to a TinyUSB HID keycode.
 *
 * @param event_code The code returned by the keyboard IC.
 *
 * @return The TinyUSB HID keycode. HID_KEY_NONE if no mapping.
 */
uint8_t KeyboardKeyIDToHIDKeyCode(kbd::lm8330::reg::EVTCODE event_code) {
  switch (event_code.KEYROW) {
    case 0:
      switch (event_code.KEYCOL) {
        case 0:
          return HID_KEY_SHIFT_LEFT;
        case 1:
          return HID_KEY_Q;
        case 2:
          return HID_KEY_W;
        case 3:
          return HID_KEY_E;
        default:
          return HID_KEY_NONE;
      }
      break;
    case 1:
      switch (event_code.KEYCOL) {
        case 0:
          return HID_KEY_CONTROL_LEFT;
        case 1:
          return HID_KEY_A;
        case 2:
          return HID_KEY_S;
        case 3:
          return HID_KEY_D;
        default:
          return HID_KEY_NONE;
      }
      break;
    default:
      return HID_KEY_NONE;
  }
}

esp_err_t ResetKeyboard(gpio_num_t reset_pin) {
  esp_err_t err = gpio_set_level(reset_pin, 0);
  if (err != ESP_OK)
    return err;

  // Typical RESETN glith filter is 100 nS, so this delay
  // is more than sufficient.
  constexpr uint32_t kResetDelayMs = 2;
  vTaskDelay(pdMS_TO_TICKS(kResetDelayMs));
  err = gpio_set_level(reset_pin, 1);
  if (err != ESP_OK)
    return err;

  // Not sure how long the chip takes after reset to be ready.
  // Choose some reasonably long time.
  constexpr uint32_t kStartupTimeMs = 50;
  vTaskDelay(pdMS_TO_TICKS(kStartupTimeMs));
  return ESP_OK;
}

uint8_t GetModifierFlag(uint8_t keycode) {
  switch (keycode) {
    case HID_KEY_SHIFT_LEFT:
      return KEYBOARD_MODIFIER_LEFTSHIFT;
    case HID_KEY_SHIFT_RIGHT:
      return KEYBOARD_MODIFIER_RIGHTSHIFT;
    case HID_KEY_CONTROL_LEFT:
      return KEYBOARD_MODIFIER_LEFTCTRL;
    case HID_KEY_CONTROL_RIGHT:
      return KEYBOARD_MODIFIER_RIGHTCTRL;
    case HID_KEY_ALT_LEFT:
      return KEYBOARD_MODIFIER_LEFTALT;
    case HID_KEY_ALT_RIGHT:
      return KEYBOARD_MODIFIER_RIGHTALT;
    case HID_KEY_GUI_LEFT:
      return KEYBOARD_MODIFIER_LEFTGUI;
    case HID_KEY_GUI_RIGHT:
      return KEYBOARD_MODIFIER_RIGHTGUI;
  }
  return 0x0;
}
}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)), key_states_({false}) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Reset() {
  ESP_LOGD(TAG, "Resetting...");

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

  // MFGCODE and SWREV are adjacent, so read both in a single operation.
  uint8_t values[2];
  Operation op = i2c_master_.CreateReadOp(kSlaveAddress, kI2CAddressSize,
                                          static_cast<uint8_t>(RegNum::MFGCODE),
                                          "mfg-swrev");
  if (!op.ready() || !op.Read(values, sizeof(values)) || !op.Execute())
    return ESP_FAIL;

  ESP_LOGI(TAG, "Reset complete, mfgcode: 0x%x, swrev: 0x%x", values[0],
           values[1]);

  return ESP_OK;
}

esp_err_t Keyboard::EnableInterrupts() {
  Operation op = i2c_master_.CreateWriteOp(kSlaveAddress, kI2CAddressSize,
                                           static_cast<uint8_t>(RegNum::KBDIC),
                                           "init-int");
  if (!op.ready())
    return ESP_FAIL;
  {
    constexpr kbd::lm8330::reg::KBDIC value = {
        .SFOFF = 0,  // 0: keyboard layout and SF keys are scanned
        .Reserved = 0,
        .EVTIC = 1,  // 1: Clear EVTCODE FIFO and corresponding interrupts
                     // REVTINT and RELINT.
        .KBDIC = 1,  // 1: Clear RSINT and RKLINT
    };

    if (!op.WriteByte(value))
      return ESP_FAIL;
  }

  {
    constexpr kbd::lm8330::reg::KBDMSK value = {
        .Reserved = 0,
        .MSKELINT = 0,  // 0: kbd event lost interrupt RELINT triggers IRQ line.
        .MSKEINT = 0,   // 0: kbd event interrupt REVINT triggers IRQ line.
        .MSKLINT = 1,   // 1: kbd lost interrupt RKLINT is masked.
        .MSKSINT = 1,   // 1: kbd status interrupt RSINT is masked.
    };
    if (!op.RestartReg(static_cast<uint8_t>(RegNum::KBDMSK),
                       Address::Mode::WRITE)) {
      return ESP_FAIL;
    }
    if (!op.WriteByte(value))
      return ESP_FAIL;
  }

  return op.Execute() ? ESP_OK : ESP_FAIL;
}

esp_err_t Keyboard::InitializeKeys() {
  Operation op = i2c_master_.CreateWriteOp(
      kSlaveAddress, kI2CAddressSize, static_cast<uint8_t>(RegNum::KBDSIZE),
      "initkeys");
  if (!op.ready())
    return ESP_FAIL;

  {
    constexpr kbd::lm8330::reg::KBDSIZE value = {
        .ROWSIZE = 2,
        .COLSIZE = 4,
    };
    if (!op.WriteByte(value))
      return ESP_FAIL;
  }

  {
    if (!op.RestartReg(static_cast<uint8_t>(RegNum::KBDDEDCFG0),
                       Address::Mode::WRITE)) {
      return ESP_FAIL;
    }
    constexpr uint16_t value = 0xFC3F;
    if (!op.Write(&value, sizeof(value)))
      return ESP_FAIL;
  }

  {
    if (!op.RestartReg(static_cast<uint8_t>(RegNum::IOCFG),
                       Address::Mode::WRITE)) {
      return ESP_FAIL;
    }
    constexpr uint8_t value =
        0xF8;  // Write default value to enable all pins as keyboard matrix
    if (!op.WriteByte(value))
      return ESP_FAIL;
  }

  {
    if (!op.RestartReg(static_cast<uint8_t>(RegNum::IOPC0),
                       Address::Mode::WRITE)) {
      return ESP_FAIL;
    }
    constexpr uint16_t value =
        0xAAAA;  // Configure pullup resistors for KPX[7:0].
    if (!op.Write(&value, sizeof(value)))
      return ESP_FAIL;
  }

  {
    if (!op.RestartReg(static_cast<uint8_t>(RegNum::IOPC1),
                       Address::Mode::WRITE)) {
      return ESP_FAIL;
    }
    constexpr uint16_t value =
        0x5555;  // Configure pulldown resistors for KPY[7:0].
    if (!op.Write(&value, sizeof(value)))
      return ESP_FAIL;
  }

  return op.Execute() ? ESP_OK : ESP_FAIL;
}

esp_err_t Keyboard::EnableClock() {
  constexpr kbd::lm8330::reg::CLKEN clken = {
      .Reserved1 = 0,
      .TIMEN = 0,
      .Reserved2 = 0,
      .KBDEN = 1,
  };
  return WriteByte(RegNum::CLKEN, clken);
}

// Initialize the keyboard. See KEYSCAN INITIALIZATION in
// LM8330 datasheet.
esp_err_t Keyboard::Initialize() {
  ESP_LOGD(TAG, "Initializing keyboard");

  esp_err_t err = InitializeKeys();
  if (err != ESP_OK)
    return err;

  err = EnableInterrupts();
  if (err != ESP_OK)
    return err;

  err = EnableClock();
  if (err != ESP_OK)
    return err;

  ESP_LOGI(TAG, "Keyboard initialized.");
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
    uint8_t modifier_bit = GetModifierFlag(keycode);
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
  esp_err_t err;
  uint8_t reg;

  err = ReadByte(RegNum::IRQST, &reg);
  if (err != ESP_OK)
    return err;
  const kbd::lm8330::reg::IRQST intr_status = reg;
  if (!intr_status.KBDIRQ) {
    ESP_LOGV(TAG, "Read: keyboard interrupt inactive.");
    return ESP_OK;
  }

  err = ReadByte(RegNum::KBDMIS, &reg);
  if (err != ESP_OK)
    return err;
  const kbd::lm8330::reg::KBDIS masked_intr_status = reg;
  if (!masked_intr_status.EVTINT) {
    ESP_LOGV(TAG, "Read: No EVTINT.");
    return ESP_OK;
  }

  uint8_t num_events = 0;
#if 1
  constexpr size_t kKeyboardFIFOSize = 15;
  uint8_t events[kKeyboardFIFOSize];
  if (!i2c_master_.Read(kSlaveAddress, events, sizeof(events),
                        /*send_start=*/true)) {
    return ESP_FAIL;
  }
  for (size_t i = 0; i < kKeyboardFIFOSize; i++) {
    if (events[i] == 0x7f)  // If event is empty.
      break;
    num_events++;
    const kbd::lm8330::reg::EVTCODE event_code = events[i];
    const uint8_t hid_keycode = KeyboardKeyIDToHIDKeyCode(event_code);
    if (hid_keycode < key_states_.size()) {
      const bool key_pressed = !event_code.RELEASE;
      key_states_[hid_keycode] = key_pressed;
    }
  }
#else
  err = ReadByte(RegNum::KBDCODE0, &reg);
  if (err != ESP_OK)
    return err;
  if (reg != 0x7f) {  // If not empty (see datasheet).
    const kbd::lm8330::reg::KBDCODE keyboard_code = reg;
  }
#endif

  ESP_LOGV(TAG, "Read: Read %u keyboard events.", num_events);
  return ReportHIDEvents();
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
