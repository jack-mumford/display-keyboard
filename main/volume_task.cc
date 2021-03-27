#include "volume_task.h"

#ifdef LOG_LOCAL_LEVEL
// <ssd1306.h> defines this (incorrectly), so undefine.
#undef LOG_LOCAL_LEVEL
#endif
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>

#include "led_controller.h"

namespace {
constexpr char TAG[] = "VolTask";
}

VolumeTask::VolumeTask() = default;

// static
esp_err_t VolumeTask::Start() {
  static VolumeTask* task = nullptr;

  ESP_LOGD(TAG, "Starting Volume task");
  if (task)
    return ESP_FAIL;

  task = new VolumeTask();
  return task->Initialize();
}

esp_err_t VolumeTask::Initialize() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  esp_err_t err = volume_display_.Initialize();
  if (err != ESP_OK)
    return err;

  return xTaskCreate(TaskFunc, TAG, kStackDepthWords, this,
                     tskIDLE_PRIORITY + 1, &task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

void VolumeTask::Run() {
  int vol = 0;
  int vol_increment = 1;
  uint8_t color_idx = 0;  // r/g/b

  LEDController* led_controller = nullptr;
  while (true) {
    // Simple test to bounce volume up and down.
    vol += vol_increment;
    if (vol < 0) {
      vol = 1;
      vol_increment = 1;
      color_idx++;
    } else if (vol > 100) {
      vol = 99;
      vol_increment = -1;
    }
    volume_display_.SetVolume(vol);

    if (led_controller) {
      uint8_t color_val = (vol * 255) / 100;
      constexpr uint8_t intensity = 2 << 3;
      switch (color_idx % 3) {
        case 0:
          led_controller->SetRGBLED(color_val, 0, 0, intensity);
          break;
        case 1:
          led_controller->SetRGBLED(0, color_val, 0, intensity);
          break;
        case 2:
          led_controller->SetRGBLED(0, 0, color_val, intensity);
          break;
      }
    } else {
      led_controller = LEDController::GetForTesting();
    }
  }
}

// static
void IRAM_ATTR VolumeTask::TaskFunc(void* arg) {
  static_cast<VolumeTask*>(arg)->Run();
}
