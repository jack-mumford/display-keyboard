#pragma once

#include <memory>

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

class Config;
class Display;
class Filesystem;
class HTTPServer;
class Keyboard;
class LEDController;
class Spotify;
class VolumeDisplay;
class WiFi;

class App {
 public:
  App();
  ~App();

  esp_err_t Initialize();
  void Run();

  bool is_initialized() const { return event_group_ != nullptr; }

 private:
  static void IRAM_ATTR AppEventTask(void*);
  static void IRAM_ATTR KeyboardSimulatorTask(void* arg);
  static void IRAM_ATTR USBTask(void* arg);
  static void IRAM_ATTR KeyboardISR(void* arg);
  static void IRAM_ATTR SNTPSyncEventHandler(struct timeval* tv);

  esp_err_t CreateAppEventTask();
  esp_err_t CreateKeyboardSimulatorTask();
  esp_err_t CreateUSBTask();
  esp_err_t InitializSNTP();
  esp_err_t InstallKeyboardISR();
  esp_err_t InitializeI2C();
  esp_err_t SetTimezone();

  std::unique_ptr<Config> config_;    // Application config data.
  std::unique_ptr<Display> display_;  // Object owning main display.
  std::unique_ptr<VolumeDisplay> volume_display_;
  std::unique_ptr<Filesystem> fs_;            // Filesystem object.
  std::unique_ptr<HTTPServer> https_server_;  // Local HTTPS server.
  std::unique_ptr<WiFi> wifi_;                // Controls WiFi.
  std::unique_ptr<Spotify> spotify_;          // All interracitons w/Spotify.
  std::unique_ptr<Keyboard> keyboard_;        // All interaction with keyboard.
  std::unique_ptr<LEDController> led_controller_;
  EventGroupHandle_t event_group_ = nullptr;  // Application events.
  TaskHandle_t main_task_ = nullptr;          // Event task.
  bool online_ = false;                       // Is this device on the network?
  bool started_spotify_currently_playing_ = false;
  bool spotify_need_access_token_refresh_ = false;
  bool sntp_initialized_ = false;
  bool uptate_display_time_ = false;
};
