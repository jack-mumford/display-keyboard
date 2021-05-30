#include "keyboard.h"

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

#include "adp5589_registers.h"
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

}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)), key_states_(0xFF, false) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Reset() {
  ESP_LOGD(TAG, "Resetting keyboard");

  Register_ID reg_id;
  esp_err_t err = ReadByte(Register::ID, &reg_id);
  if (err != ESP_OK)
    return err;
  ESP_LOGI(TAG, "Keyboard mfr: %u, rev: %u", reg_id.MAN, reg_id.REV);

  return ESP_OK;
}

esp_err_t Keyboard::Initialize() {
  ESP_LOGD(TAG, "Initializing keyboard");

  i2c::Operation op = i2c_master_.CreateWriteOp(
      kSlaveAddress, kI2CAddressSize,
      static_cast<uint8_t>(Register::GENERAL_CFG_B), "kbd-init");
  if (!op.ready())
    return ESP_FAIL;

  {
    Register_GENERAL_CFG_B reg;
    reg.OSC_EN = true;      // Enable oscillator.
    reg.CORE_FREQ = 0b01;   // 100MHz.
    reg.LCK_TRK_LOGIC = 1;  // do not track.
    reg.LCK_TRK_GPI = 1;    // do not track.
    reg.Unused = 0;         // Clear unused.
    reg.INT_CFG = 0;  // INT pin remains asserted if an interrupt is pending.
    reg.RST_CFG = 1;  // ADP5589 does not reset if RST is low.
    if (!op.WriteByte(reg))
      return ESP_FAIL;
  }

  {
    if (!op.RestartReg(static_cast<uint8_t>(Register::INT_EN),
                       i2c::Address::Mode::WRITE))
      return ESP_FAIL;
    Register_INT_EN reg;
    reg.Reserved = 0;
    reg.LOGIC2_IEN = false;
    reg.LOGIC1_IEN = false;
    reg.EVENT_IEN = true;
    reg.GPI_IEN = false;
    reg.OVRFLOW_IEN = true;
    if (!op.WriteByte(reg))
      return ESP_FAIL;
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

void Keyboard::LogEvents() {
  Register_Status status;
  i2c::Operation op = i2c_master_.CreateReadOp(
      kSlaveAddress, kI2CAddressSize, static_cast<uint8_t>(Register::Status),
      "evt-log");
  if (!op.ready())
    return;
  if (!op.Read(&status, sizeof(status)))
    return;
  if (!op.Execute(i2c::Operation::ExecuteEnd::NoStop))
    return;
  if (!status.EC) {
    ESP_LOGI(TAG, "No keyboard events.");
    return;
  }

  const size_t kMaxFIFOSize = 16;
  Register_FIFO fifo[kMaxFIFOSize];
  if (!op.RestartReg(static_cast<uint8_t>(Register::FIFO_1),
                     i2c::Address::Mode::READ)) {
    return;
  }
  if (!op.Read(&fifo, status.EC * sizeof(Register_FIFO)))
    return;
  if (!op.Execute())
    return;
  for (uint8_t i = 0; i < status.EC; i++) {
    ESP_LOGI(TAG, "Got keyboard event id %u:%c",
             static_cast<uint8_t>(fifo[i].IDENTIFIER),
             fifo[i].Event_State ? 'U' : 'D');
  }
}

esp_err_t Keyboard::HandleEvents() {
  esp_err_t err;
  event_number_++;

  Register_INT_STATUS interrupt_status;
  err = ReadByte(Register::INT_STATUS, &interrupt_status);
  if (err != ESP_OK)
    return err;
  // Write back INT status register to clear.
  WriteByte(Register::INT_STATUS, interrupt_status);
  if (interrupt_status.OVRFLOW_INT) {
    ESP_LOGE(TAG, "Keyboard FIFO overflow.");
    return ESP_FAIL;
  }

  Register_Status status;
  err = ReadByte(Register::Status, &status);
  if (err != ESP_OK)
    return err;

  if (!status.EC)
    return ESP_OK;

  i2c::Operation fifo_op = i2c_master_.CreateReadOp(
      kSlaveAddress, kI2CAddressSize, static_cast<uint8_t>(Register::FIFO_1),
      "FIFO-read");
  if (!fifo_op.ready())
    return ESP_FAIL;

  for (uint8_t i = 0; i < status.EC; i++) {
    Register_FIFO reg_fifo;
    if (i > 0 && !fifo_op.Restart(i2c::Address::Mode::READ))
      return ESP_FAIL;
    if (!fifo_op.Read(reinterpret_cast<uint8_t*>(&reg_fifo),
                      sizeof(reg_fifo))) {
      return ESP_FAIL;
    }

    const uint8_t hid_keycode = KeyboardKeyIDToHIDKeyCode(reg_fifo.IDENTIFIER);
    if (hid_keycode < key_states_.size()) {
      const bool key_pressed = reg_fifo.Event_State & 0b10000000;
      key_states_[hid_keycode] = key_pressed;
    }
  }

  return ReportHIDEvents();
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
