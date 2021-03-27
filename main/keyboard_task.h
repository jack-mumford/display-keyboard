#pragma once

#include <freertos/include/freertos/FreeRTOS.h>
#include <freertos/include/freertos/event_groups.h>
#include <freertos/include/freertos/task.h>

#include <esp_err.h>

#include "keyboard.h"

/**
 * Task responsible for detecting keyboard events from the IC
 * and dispatching them to the USB HID.
 */
class KeyboardTask {
 public:
  static esp_err_t Start();

 private:
  static void IRAM_ATTR TaskFunc(void* arg);
  static void IRAM_ATTR KeyboardISR(void* arg);

  KeyboardTask();
  ~KeyboardTask();

  esp_err_t InstallKeyboardISR();
  esp_err_t Initialize();
  void IRAM_ATTR Run();

  EventGroupHandle_t event_group_;  // Application events.
  TaskHandle_t task_ = nullptr;     // This task.
  Keyboard keyboard_;               // All interaction with keyboard.
};
