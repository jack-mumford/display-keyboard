#pragma once

#include <memory>

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

class Config;
class Display;
class Filesystem;
class Keyboard;
class Spotify;
class WiFi;

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

  std::unique_ptr<Config> config_;       // Application config data.
  std::unique_ptr<Display> display_;     // Object owning main display.
  std::unique_ptr<Filesystem> fs_;       // Filesystem object.
  std::unique_ptr<WiFi> wifi_;           // Controls WiFi.
  std::unique_ptr<Spotify> spotify_;     // All network interraction w/Spotify.
  std::unique_ptr<Keyboard> keyboard_;   // All interaction with keyboard.
  EventGroupHandle_t wifi_event_group_;  // WiFi events.
  TaskHandle_t main_task_;               // Event task.
  bool online_;                          // Is this device on the network?
  bool did_spotify_test_;
};