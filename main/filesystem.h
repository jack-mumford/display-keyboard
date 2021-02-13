#pragma once

#include <esp_err.h>

class Filesystem {
 public:
  Filesystem();
  ~Filesystem();

  esp_err_t Initialize();
};
