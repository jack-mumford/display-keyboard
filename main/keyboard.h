#pragma once

#include <vector>

#include <esp_err.h>
#include <i2clib/master.h>
#include <i2clib/operation.h>

namespace adp5589 {
enum class RegNum : uint8_t;

namespace Register {
struct FIFO;
struct ID;
struct Status;
struct INT_STATUS;
struct PIN_CONFIG_A;
struct PIN_CONFIG_B;
struct PIN_CONFIG_C;
}  // namespace Register
}  // namespace adp5589

class Keyboard {
 public:
  explicit Keyboard(i2c::Master i2c_master);
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
  esp_err_t WriteByte(adp5589::RegNum reg, uint8_t value);
  esp_err_t ReadByte(adp5589::RegNum reg, uint8_t* value);
  esp_err_t Read(adp5589::Register::FIFO* reg);
  esp_err_t Read(adp5589::Register::ID* reg);
  esp_err_t Read(adp5589::Register::INT_STATUS* reg);
  esp_err_t Read(adp5589::Register::Status* reg);
  esp_err_t InitializeKeys(i2c::Operation& op);
  esp_err_t InitializeInterrupts(i2c::Operation& op);

  i2c::Master i2c_master_;

  /**
   * Array used to map TinyUSB's HID KEYCODE value to the button
   * pressed/depressed state. true=pressed.
   */
  std::vector<bool> key_states_;
  uint32_t event_number_ = 0;
};
