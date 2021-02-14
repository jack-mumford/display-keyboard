#pragma once

#include <string>

#include <esp_err.h>
#include <esp_event.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

class WiFi {
 public:
  WiFi(EventGroupHandle_t wifi_event_group);
  ~WiFi();

  esp_err_t Inititialize();
  esp_err_t InitiateConnection(const std::string& ssid, const std::string& key);
  esp_err_t GetHostname(std::string* hostname) const;
  esp_err_t GetIPAddress(esp_ip4_addr_t* addr) const;

 private:
  static void EventHandler(void* arg,
                           esp_event_base_t event_base,
                           int32_t event_id,
                           void* event_data);
  void HandleWiFiEvent(wifi_event_t event_id, void* event_data);
  void HandleIPEvent(ip_event_t event_id, void* event_data);

  EventGroupHandle_t wifi_event_group_;
  esp_event_handler_instance_t instance_any_id_;
  esp_event_handler_instance_t instance_got_ip_;
  int retry_num_;
  esp_netif_t* netif_;
};
