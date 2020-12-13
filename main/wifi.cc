#include "wifi.h"

#include <cstring>

#include <esp_log.h>
#include <esp_wifi.h>

namespace {
constexpr char TAG[] = "wifi";
constexpr size_t kMaxSSIDLen = 31;
constexpr size_t kMaxKeyLen = 63;
}  // namespace

WiFi::WiFi() = default;

WiFi::~WiFi() = default;

esp_err_t WiFi::Connect(const std::string& ssid, const std::string& key) {
  ESP_LOGI(TAG, "Connecting to WiFi network: \"%s\"", ssid.c_str());

  if (ssid.length() > kMaxSSIDLen || key.length() > kMaxKeyLen)
    return ESP_ERR_INVALID_SIZE;

  esp_err_t err = esp_wifi_set_mode(WIFI_MODE_STA);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failure to set mode: %s", esp_err_to_name(err));
    return err;
  }

  wifi_config_t wifi_config = {
      .sta =
          {
              .ssid = {0},
              .password = {0},
              .scan_method = WIFI_FAST_SCAN,
              .bssid_set = false,
              .bssid = {0},
              .channel = 0,
              .listen_interval = 0,
              .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
              .threshold{
                  .rssi = 0,
                  .authmode = WIFI_AUTH_WPA2_PSK,
              },
              .pmf_cfg = {.capable = true, .required = false},
          },
  };
  std::strcpy(reinterpret_cast<char*>(wifi_config.sta.ssid), ssid.c_str());
  std::strcpy(reinterpret_cast<char*>(wifi_config.sta.password), key.c_str());
  err = esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Failure to set config: %s", esp_err_to_name(err));
    return err;
  }

  err = esp_wifi_start();
  if (err != ESP_OK)
    ESP_LOGE(TAG, "Start failed: %s", esp_err_to_name(err));

  return err;
}
