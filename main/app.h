#pragma once

#include <memory>

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>

class Display;
class Filesystem;
class USB;

class App {
 public:
  App();
  ~App();

  esp_err_t Initialize();
  void Run();

 private:
  std::unique_ptr<Display> display_;
  std::unique_ptr<Filesystem> fs_;
  std::unique_ptr<USB> usb_;
  EventGroupHandle_t wifi_event_group_;
};