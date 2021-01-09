#pragma once

// See https://docs.lvgl.io/latest/en/html/overview/file-system.html

#include <esp_err.h>

namespace lvgl {

class Drive {
 public:
  Drive() = delete;
  ~Drive() = delete;

  static esp_err_t Initialize();
};

}  // namespace lvgl