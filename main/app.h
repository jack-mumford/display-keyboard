#pragma once

#include <memory>

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

class Config;
class Display;
class Filesystem;
class USB;
class WiFi;

class App {
 public:
  App();
  ~App();

  esp_err_t Initialize();
  void Run();

  bool is_initialized() const { return wifi_event_group_ != nullptr; }

 private:
  static void IRAM_ATTR WiFiTaskHandler(void*);
  static void IRAM_ATTR USBTestTaskHandler(void* arg);

  esp_err_t CreateWiFiStatusTask();
  esp_err_t CreateUSBTestTask();

  std::unique_ptr<Config> config_;
  std::unique_ptr<Display> display_;
  std::unique_ptr<Filesystem> fs_;
  std::unique_ptr<USB> usb_;
  std::unique_ptr<WiFi> wifi_;
  EventGroupHandle_t wifi_event_group_;
  TaskHandle_t main_task_;
};