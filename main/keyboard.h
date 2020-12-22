#pragma once

#include <esp_err.h>

#include <i2clib/master.h>

class Keyboard {
 public:
  Keyboard(i2c::Master i2c_master);
  ~Keyboard();

  esp_err_t Initialize();

 private:
  i2c::Master i2c_master_;
};