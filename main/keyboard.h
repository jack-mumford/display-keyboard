#pragma once

#include <esp_err.h>

#include <i2clib/master.h>

enum class Register : uint8_t;

class Keyboard {
 public:
  Keyboard(i2c::Master i2c_master);
  ~Keyboard();

  esp_err_t Initialize();
  esp_err_t LogEventCount();

 private:
  esp_err_t WriteByte(Register reg, uint8_t value);
  esp_err_t WriteWord(Register reg, uint16_t value);

  i2c::Master i2c_master_;
};