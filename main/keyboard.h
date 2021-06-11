#pragma once

#include <vector>

#include <esp_err.h>
#include <i2clib/master.h>
#include <i2clib/operation.h>

namespace adp5589 {
enum class Register : uint8_t;

struct Register_FIFO;
struct Register_ID;
struct Register_Status;
struct Register_INT_STATUS;
struct PIN_CONFIG_A;
struct PIN_CONFIG_B;
struct PIN_CONFIG_C;
}  // namespace adp5589

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
  esp_err_t ReadByte(adp5589::Register reg, uint8_t* value);
  esp_err_t Read(adp5589::Register_FIFO* reg);
  esp_err_t Read(adp5589::Register_ID* reg);
  esp_err_t Read(adp5589::Register_INT_STATUS* reg);
  esp_err_t Read(adp5589::Register_Status* reg);
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
