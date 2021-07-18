#pragma once

#include <array>

#include <esp_err.h>
#include <i2clib/simple_master.h>

namespace kbd {
namespace lm8330 {
enum class RegNum : uint8_t;
}  // namespace lm8330
}  // namespace kbd

// Maximum number of HID_KEY_* key codes.
constexpr size_t kNumHIDKeyCodes = 0xff;

class Keyboard {
 public:
  explicit Keyboard(i2c::SimpleMaster i2c_master);
  ~Keyboard();

  /**
   * Reset the keyboard IC to manufacturer default values.
   */
  esp_err_t Reset();

  /**
   * Initialize the keyboard.
   *
   * Call after reset to configure the keyboard IC.
   */
  esp_err_t Initialize();

  /**
   * Handle any keyboard events.
   *
   * Call this function, either polled or when interrupt pin indicates, to
   * handle any queued keyboard events.
   */
  esp_err_t HandleEvents();

 private:
  /**
   * Report all HID events to the USB host.
   */
  esp_err_t ReportHIDEvents();
  esp_err_t WriteByte(kbd::lm8330::RegNum reg, uint8_t value);
  esp_err_t ReadByte(kbd::lm8330::RegNum reg, uint8_t* value);
  esp_err_t EnableInterrupts();
  esp_err_t InitializeKeys();
  esp_err_t EnableClock();

  i2c::SimpleMaster i2c_master_;

  /**
   * Array used to map TinyUSB's HID KEYCODE value to the button
   * pressed/depressed state.
   */
  std::array<bool, kNumHIDKeyCodes> key_states_;  // true if depressed.
  uint32_t event_number_ = 0;
};
