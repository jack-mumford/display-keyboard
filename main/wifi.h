#pragma once

#include <string>

#include <esp_err.h>
class WiFi {
 public:
  WiFi();
  ~WiFi();

  esp_err_t Connect(const std::string& ssid, const std::string& key);
};