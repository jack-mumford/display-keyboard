#pragma once

#include <vector>

#include <esp_err.h>
#include <i2clib/master.h>

namespace adp5589 {
enum class Register : uint8_t;
}

class Keyboard {
 public:
  Keyboard(i2c::Master i2c_master);
  ~Keyboard();

  esp_err_t Initialize();
  esp_err_t Reset();

  /**
   * Handle any keyboard events.
   *
   * Call this function, either polled or when interrupt pin indicates, to
   * handle any queued keyboard events.
   *
   * @return ESP_OK when successful.
   */
  esp_err_t HandleEvents();

 private:
  /**
   * Report all HID events to the USB host.
   *
   * @return esp_err_t
   */
  esp_err_t ReportHIDEvents();
  esp_err_t WriteByte(adp5589::Register reg, uint8_t value);
  esp_err_t ReadByte(adp5589::Register reg, void* value);
  esp_err_t InitializeInterrupts();

  i2c::Master i2c_master_;

  /**
   * Array used to map TinyUSB's HID KEYCODE value to the button
   * pressed/depressed state. true=pressed.
   */
  std::vector<bool> key_states_;
  uint32_t event_number_ = 0;
};
