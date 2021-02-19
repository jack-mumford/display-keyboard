#pragma once

#include <memory>

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

#include "config.h"
#include "filesystem.h"
#include "http_server.h"

class HTTPServer;
class Keyboard;
class LEDController;
class Spotify;
class WiFi;

class App {
 public:
  App();
  ~App();

  esp_err_t Initialize();
  void Run();

 private:
  static void IRAM_ATTR AppEventTask(void*);
  static void IRAM_ATTR SNTPSyncEventHandler(struct timeval* tv);

  esp_err_t CreateAppEventTask();
  esp_err_t InitializSNTP();
  esp_err_t InitializeI2C();
  esp_err_t SetTimezone();

  Config config_;                                  // Application config data.
  Filesystem filesystem_;                          // Filesystem object.
  HTTPServer https_server_;                        // Local HTTPS server.
  std::unique_ptr<LEDController> led_controller_;  // Set all LED's.
  EventGroupHandle_t event_group_;                 // Application events.
  std::unique_ptr<WiFi> wifi_;                     // Controls WiFi.
  std::unique_ptr<Spotify> spotify_;               // All Spotify interracitons.
  TaskHandle_t main_task_ = nullptr;               // Event task.
  bool online_ = false;                            // Is device on the network?
  bool started_spotify_currently_playing_ = false;
  bool spotify_need_access_token_refresh_ = false;
  bool sntp_initialized_ = false;
};
