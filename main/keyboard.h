#pragma once

#include <vector>

#include <esp_err.h>
#include <i2clib/master.h>

class Keyboard {
 public:
  Keyboard(i2c::Master i2c_master);
  ~Keyboard();

  esp_err_t Initialize();

  /**
   * Handle any keyboard events.
   *
   * Call this function, either polled or when interrupt pin indicates, to
   * handle any queued keyboard events.
   *
   * @return ESP_OK when successful.
   */
  esp_err_t HandleEvents();

  esp_err_t LogEventCount();

 private:
  /**
   * Report all HID events to the host via TinyUSB.
   *
   * @return esp_err_t
   */
  esp_err_t ReportHIDEvents();

  i2c::Master i2c_master_;

  /**
   * Array used to map TinyUSB's HID KEYCODE value to the button
   * pressed/depressed state. true=pressed.
   */
  std::vector<bool> key_states_;
};