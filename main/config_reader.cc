#include "config_reader.h"

#include <cstring>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#include <ini.h>
#include <stdio.h>

#include "config.h"

namespace {

constexpr char TAG[] = "ConfigReader";
constexpr char kConfigPath[] = "/spiffs/config.ini";

bool streq(const char* a, const char* b) {
  return !strcmp(a, b);
}

int handler(void* user,
            const char* section,
            const char* name,
            const char* value) {
  Config* config = static_cast<Config*>(user);
  if (streq(section, "wifi")) {
    if (streq(name, "ssid"))
      config->wifi.ssid = value;
    else if (streq(name, "key"))
      config->wifi.key = value;
    else
      return 1;  // Unknown key.
  }
  if (streq(section, "time")) {
    if (streq(name, "ntp_server"))
      config->time.ntp_server = value;
    else if (streq(name, "timezone"))
      config->time.timezone = value;
  } else {
    return 1;  // Unknown section.
  }

  return 1;
}  // namespace

}  // namespace

ConfigReader::ConfigReader() = default;

ConfigReader::~ConfigReader() = default;

esp_err_t ConfigReader::Read(Config* config) {
  if (ini_parse(kConfigPath, handler, config) < 0) {
    ESP_LOGE(TAG, "Can't load \"%s\".", kConfigPath);
    return ESP_FAIL;
  }

  return ESP_OK;
}
