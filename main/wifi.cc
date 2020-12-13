#include "wifi.h"

#include <cstring>

#include <esp_log.h>
#include <esp_wifi.h>

namespace {
constexpr char TAG[] = "wifi";
constexpr size_t kMaxSSIDLen = 31;
constexpr size_t kMaxKeyLen = 63;
constexpr int kMaxNumConnectRetry = 10;
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

}  // namespace

void WiFi::EventHandler(esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data) {
  if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
    esp_wifi_connect();
  } else if (event_base == WIFI_EVENT &&
             event_id == WIFI_EVENT_STA_DISCONNECTED) {
    if (retry_num_ < kMaxNumConnectRetry) {
      esp_wifi_connect();
      retry_num_++;
      ESP_LOGI(TAG, "retry to connect to the AP");
    } else {
      xEventGroupSetBits(wifi_event_group_, WIFI_FAIL_BIT);
    }
    ESP_LOGI(TAG, "connect to the AP fail");
  } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
    ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    retry_num_ = 0;
    xEventGroupSetBits(wifi_event_group_, WIFI_CONNECTED_BIT);
  }
}

// static
void WiFi::EventHandler(void* arg,
                        esp_event_base_t event_base,
                        int32_t event_id,
                        void* event_data) {
  static_cast<WiFi*>(arg)->EventHandler(event_base, event_id, event_data);
}

WiFi::WiFi()
    : wifi_event_group_(nullptr),
      instance_any_id_(nullptr),
      instance_got_ip_(nullptr),
      retry_num_(0) {}

WiFi::~WiFi() {
  /* The event will not be processed after unregister */
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      IP_EVENT, IP_EVENT_STA_GOT_IP, instance_got_ip_));
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(
      WIFI_EVENT, ESP_EVENT_ANY_ID, instance_any_id_));
  vEventGroupDelete(wifi_event_group_);
}

esp_err_t WiFi::Inititialize() {
  wifi_event_group_ = xEventGroupCreate();

  esp_err_t err = esp_netif_init();
  if (err != ESP_OK)
    return err;

  esp_netif_create_default_wifi_sta();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  err = esp_wifi_init(&cfg);
  if (err != ESP_OK)
    return err;

  err = esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &EventHandler, this, &instance_any_id_);
  if (err != ESP_OK)
    return err;
  err = esp_event_handler_instance_register(
      IP_EVENT, IP_EVENT_STA_GOT_IP, &EventHandler, this, &instance_got_ip_);

  return err;
}

esp_err_t WiFi::Connect(const std::string& ssid, const std::string& key) {
  ESP_LOGI(TAG, "Connecting to WiFi network: \"%s\"", ssid.c_str());

  retry_num_ = 0;

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
