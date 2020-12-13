#pragma once

#include <string>

#include <esp_err.h>
#include <esp_event.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>


class WiFi {
 public:
  WiFi();
  ~WiFi();

  esp_err_t Inititialize();
  esp_err_t Connect(const std::string& ssid, const std::string& key);

 private:
static void EventHandler(void* arg,
                   esp_event_base_t event_base,
                   int32_t event_id,
                   void* event_data);
void EventHandler(
                   esp_event_base_t event_base,
                   int32_t event_id,
                   void* event_data);

  EventGroupHandle_t wifi_event_group_;
  esp_event_handler_instance_t instance_any_id_;
  esp_event_handler_instance_t instance_got_ip_;
  int retry_num_;
};
