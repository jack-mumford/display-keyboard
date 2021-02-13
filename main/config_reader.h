#pragma once

#include <esp_err.h>

class Config;

class ConfigReader {
 public:
  ConfigReader();
  ~ConfigReader();

  /**
   * @brief Load program configuration data from filesystem.
   *
   * @param config location to write config data.
   * @return esp_err_t Read status.
   */
  esp_err_t Read(Config* config);
};
