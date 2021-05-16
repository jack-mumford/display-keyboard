#pragma once

#include <freertos/include/freertos/FreeRTOS.h>
#include <freertos/include/freertos/event_groups.h>
#include <freertos/include/freertos/task.h>

#include <esp_err.h>

#include "config.h"
#include "filesystem.h"
#include "http_server.h"
#include "led_controller.h"
#include "spotify.h"
#include "wifi.h"

/**
 * The main application task.
 *
 * Responsible for starting all other tasks, maintaining application
 * state, and distributing state to other tasks when necessary.
 */
class MainTask {
 public:
  static esp_err_t Start();

 private:
  static void IRAM_ATTR TaskFunc(void* arg);
  static esp_err_t InitializeI2C();

  MainTask();
  ~MainTask();

  void UpdateSpotify();
  esp_err_t SetTimezone();
  esp_err_t InitializSNTP();
  esp_err_t Initialize();
  void IRAM_ATTR Run();

  Config config_;                   // Application config data.
  Filesystem filesystem_;           // Filesystem object.
  HTTPServer https_server_;         // Local HTTPS server.
  LEDController led_controller_;    // Set all LED's.
  EventGroupHandle_t event_group_;  // Application events.
  WiFi wifi_;                       // Controls WiFi.
  Spotify spotify_;                 // Interface with Spotify.
  TaskHandle_t task_ = nullptr;     // Event task.
  bool online_ = false;             // Is device on the network?
  bool started_spotify_currently_playing_ = false;
  bool spotify_need_access_token_refresh_ = false;
  bool sntp_initialized_ = false;
};
