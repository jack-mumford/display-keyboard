#pragma once

#include <freertos/include/freertos/FreeRTOS.h>
#include <freertos/include/freertos/event_groups.h>
#include <freertos/include/freertos/semphr.h>
#include <freertos/include/freertos/task.h>

#include <esp_err.h>

#include "keyboard.h"

// Keyboard polling is for testing - otherwise interrupts
// are used.
#undef KEYBOARD_POLLING

/**
 * Task responsible for detecting keyboard events from the IC
 * and dispatching them to the USB HID.
 */
class KeyboardTask {
 public:
  static esp_err_t Start();

 private:
  static void IRAM_ATTR TaskFunc(void* arg);

  KeyboardTask();
  ~KeyboardTask();

#ifdef KEYBOARD_POLLING
  static void IRAM_ATTR LogKeysCb(void* arg);
  void LogKeys();
  esp_err_t CreateKeyLogTimer();
#else
  static void IRAM_ATTR KeyboardISR(void* arg);
  esp_err_t InstallKeyboardISR();
#endif
  esp_err_t Initialize();
  void IRAM_ATTR Run();

  EventGroupHandle_t event_group_;  // Application events.
  TaskHandle_t task_ = nullptr;     // This task.
  Keyboard keyboard_;               // All interaction with keyboard.
#ifdef KEYBOARD_POLLING
  esp_timer_handle_t time_update_timer_;
#endif
  SemaphoreHandle_t mutex_;
};
