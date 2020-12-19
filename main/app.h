#pragma once

#include <memory>

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

class Config;
class Display;
class Filesystem;
class WiFi;

namespace usb {
class Device;
class HID;
}  // namespace usb

class App {
 public:
  App();
  ~App();

  esp_err_t Initialize();
  void Run();

  bool is_initialized() const { return wifi_event_group_ != nullptr; }

 private:
  static void IRAM_ATTR WiFiStatusTask(void*);
  static void IRAM_ATTR KeyboardSimulatorTask(void* arg);
  static void IRAM_ATTR USBTask(void* arg);

  esp_err_t CreateWiFiStatusTask();
  esp_err_t CreateKeyboardSimulatorTask();
  esp_err_t CreateUSBTask();

  std::unique_ptr<Config> config_;
  std::unique_ptr<Display> display_;
  std::unique_ptr<Filesystem> fs_;
  std::unique_ptr<usb::Device> usb_device_;
  std::unique_ptr<usb::HID> usb_hid_;
  std::unique_ptr<WiFi> wifi_;
  EventGroupHandle_t wifi_event_group_;
  TaskHandle_t main_task_;
};