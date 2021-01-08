#include "keyboard.h"

#include <cstdint>
#include <cstring>
#include <utility>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <class/hid/hid.h>
#include <esp_err.h>
#include <esp_log.h>

#include "usb_hid.h"

#if !defined(SET_BITS)
#define SET_BITS(value, bits) (value |= (bits))
#endif

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#endif

namespace {
constexpr char TAG[] = "kbd_kbd";
constexpr uint8_t kSlaveAddress = 0x34;  // I2C address of TCA8418 IC.

constexpr uint8_t kActiveRows7_0 = 0b00000011;
constexpr uint8_t kActiveCols7_0 = 0b00001111;
constexpr uint8_t kActiveCols9_8 = 0b00000000;

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
  KP_GPIO2       = 0x1E,
  KP_GPIO3       = 0x1F,
  GPI_EM1        = 0x20,
  GPI_EM2        = 0x21,
  GPI_EM3        = 0x22,
  GPIO_DIR1      = 0x23,
  GPIO_DIR2      = 0x24,
  GPIO_DIR3      = 0x25,
  GPIO_INT_LVL1  = 0x26,
  GPIO_INT_LVL2  = 0x27,
  GPIO_INT_LVL3  = 0x28,
  DEBOUNCE_DIS1  = 0x29,
  DEBOUNCE_DIS2  = 0x2A,
  DEBOUNCE_DIS3  = 0x2B,
  GPIO_PULL1     = 0x2C,
  GPIO_PULL2     = 0x2D,
  GPIO_PULL3     = 0x2E,
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

// Used for GPIO_INT_STAT1, GPIO_DAT_STAT1, GPIO_DAT_OUT1,
// and GPIO_INT_EN1.
struct Register_GPIO_INT_STAT1 {
  uint8_t R0 : 1;
  uint8_t R1 : 1;
  uint8_t R2 : 1;
  uint8_t R3 : 1;
  uint8_t R4 : 1;
  uint8_t R5 : 1;
  uint8_t R6 : 1;
  uint8_t R7 : 1;
};

/**
 * Used for:
 *
 * * GPIO_INT_STAT2
 * * GPIO_DAT_STAT2
 * * GPIO_DAT_OUT2
 * * GPIO_INT_EN2
 */
struct Register_GPIO_INT_STAT2 {
  uint8_t C0 : 1;
  uint8_t C1 : 1;
  uint8_t C2 : 1;
  uint8_t C3 : 1;
  uint8_t C4 : 1;
  uint8_t C5 : 1;
  uint8_t C6 : 1;
  uint8_t C7 : 1;
};

// Used for GPIO_INT_STAT3, GPIO_DAT_STAT3, GPIO_DAT_OUT3,
// and GPIO_INT_EN3.
struct Register_GPIO_INT_STAT3 {
  uint8_t C8 : 1;
  uint8_t C9 : 1;
  uint8_t Unused : 6;
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

/**
 * @brief Convert a Keyboard code to a HID keycode.
 *
 * @param key_code The code returned by the keyboard IC.
 *
 * @return The TinyUSB HID keycode. HID_KEY_NONE if no mapping.
 */
uint8_t KeyboardKeyCodeToHIDKeyCode(uint8_t key_code) {
  // Hard-coded. Will replace with a configuration which comes from
  // a config file at some point.
  switch (key_code) {
    case 14:
      return HID_KEY_A;
    case 13:
      return HID_KEY_S;
    case 12:
      return HID_KEY_D;
    case 11:
      return HID_KEY_F;
    case 04:
      return HID_KEY_Q;
    case 03:
      return HID_KEY_W;
    case 02:
      return HID_KEY_E;
    case 01:
      return HID_KEY_R;
    default:
      break;
  }
  return HID_KEY_NONE;
}

}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)), key_states_(0xFF, false) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Initialize() {
  constexpr Register_CFG reg_config = {
      .KE_IEN = true,
      .GPI_IEN = false,
      .K_LCK_IEN = false,
      .OVR_FLOW_IEN = false,
      .INT_CFG = false,
      .OVR_FLOW_M = false,
      .GPI_E_CFG = false,
      .AI = true,
  };

  esp_err_t err = WriteRegister(i2c_master_, Register::CFG, &reg_config);
  if (err != ESP_OK)
    return err;

  if (!i2c_master_.WriteRegister(kSlaveAddress,
                                 static_cast<uint8_t>(Register::KP_GPIO1),
                                 kActiveRows7_0)) {
    return ESP_FAIL;
  }
  if (!i2c_master_.WriteRegister(kSlaveAddress,
                                 static_cast<uint8_t>(Register::KP_GPIO2),
                                 kActiveCols7_0)) {
    return ESP_FAIL;
  }
  if (!i2c_master_.WriteRegister(kSlaveAddress,
                                 static_cast<uint8_t>(Register::KP_GPIO3),
                                 kActiveCols9_8)) {
    return ESP_FAIL;
  }

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
  // return usb::HID::KeyboardReport(usb::REPORT_ID_KEYBOARD, modifier,
  // keycodes);
  return ESP_OK;
}

esp_err_t Keyboard::HandleEvents() {
  event_number_++;
  Register_INT_STAT int_stat;
  esp_err_t err = ReadRegister(i2c_master_, Register::INT_STAT, &int_stat);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Can't read from kbd IC.");
    return err;
  }
  const bool had_keyboard_event = int_stat.K_INT || int_stat.GPI_INT;
  if (!had_keyboard_event) {
    // TODO: Investigate triggering. We call this too often.
    return ESP_OK;
  }

  Register_KEY_LCK_EC key_lck_ec;
  err = ReadRegister(i2c_master_, Register::KEY_LCK_EC, &key_lck_ec);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Can't read KEY_LCK_EC");
    return err;
  }
  ESP_LOGI(TAG, "Had keyboard event %u. count: %u", event_number_,
           key_lck_ec.KEC);
  for (uint8_t i = 0; i < key_lck_ec.KEC; i++) {
    Register_KEY_EVENT_A key_event_a;
    err = ReadRegister(i2c_master_, Register::KEY_EVENT_A, &key_event_a);
    if (err != ESP_OK) {
      ESP_LOGE(TAG, "Can't read KEY_EVENT_A");
      return err;
    }
    const uint8_t key_code = key_event_a.key_code & 0b01111111;
    if (key_code <= 80) {
      const uint8_t hid_keycode = KeyboardKeyCodeToHIDKeyCode(key_code);
      if (hid_keycode < key_states_.size()) {
        const bool key_pressed = key_event_a.key_code & 0b10000000;
        key_states_[hid_keycode] = key_pressed;
      }
    } else {
      // Don't register for GPIO events, so shouldn't occur.
    }
  }

  err = WriteRegister(i2c_master_, Register::INT_STAT, &int_stat);
  if (err == ESP_OK)
    err = ReportHIDEvents();
  else
    ReportHIDEvents();

  ESP_LOGI(TAG, "  Handled %u events, %s", key_lck_ec.KEC,
           esp_err_to_name(err));
  return err;
}