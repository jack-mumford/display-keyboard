#include "keyboard_simulator_task.h"

#include <string>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>

#include "gpio_pins.h"
#include "usb_device.h"
#include "usb_hid.h"

namespace {
constexpr char TAG[] = "KbdSimTask";
}

KeyboardSimulatorTask::KeyboardSimulatorTask() = default;

KeyboardSimulatorTask::~KeyboardSimulatorTask() = default;

// static
esp_err_t KeyboardSimulatorTask::Start() {
  static KeyboardSimulatorTask* task = nullptr;

  ESP_LOGD(TAG, "Starting keyboard simulator task");
  if (task)
    return ESP_FAIL;

  task = new KeyboardSimulatorTask();
  return task->Initialize();
}

esp_err_t KeyboardSimulatorTask::Initialize() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  return xTaskCreate(TaskFunc, TAG, kStackDepthWords, this, tskIDLE_PRIORITY,
                     &task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

void IRAM_ATTR KeyboardSimulatorTask::Run() {
  ESP_LOGW(TAG, "In keyboard simulator task.");

  const std::string kTypedString = "Super Display Keyboard. ";
  int idx = 0;

  // TODO: This should really be thread-safe. Probably OK for testing.
  while (true) {
    if (usb::Device::Mounted()) {
      ESP_LOGD(TAG, "Mounted, sending keyboard event.");

      usb::HID::KeyboardPress(usb::REPORT_ID_KEYBOARD, kTypedString[idx++]);
      if (idx == kTypedString.length())
        idx = 0;

      // For some reason a delay is required. Without one the keyboard release
      // doesn't seem to work and auto-repeats forever. 5ms was impirically
      // found to be short enough to prevent auto-repeat.
      ets_delay_us(5000);
      usb::HID::KeyboardRelease(usb::REPORT_ID_KEYBOARD);
      vTaskDelay(pdMS_TO_TICKS(2000));
    } else {
      ESP_LOGD(TAG, "USB not mounted");
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }
}

// static
void IRAM_ATTR KeyboardSimulatorTask::TaskFunc(void* arg) {
  static_cast<KeyboardSimulatorTask*>(arg)->Run();
}
