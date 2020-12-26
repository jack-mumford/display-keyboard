#pragma once

#include <esp_err.h>

#include <i2clib/master.h>

struct RegisterCFG;

class Keyboard {
 public:
  Keyboard(i2c::Master i2c_master);
  ~Keyboard();

  esp_err_t Initialize();

 private:
  esp_err_t ReadConfig(RegisterCFG* config);
  esp_err_t WriteConfig();

  i2c::Master i2c_master_;
};