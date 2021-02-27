#include "usb_task.h"

#include <esp_log.h>

#include "usb_device.h"
#include "usb_hid.h"

namespace {
constexpr char TAG[] = "USBTask";
}

USBTask::USBTask() = default;

// static
esp_err_t USBTask::Start() {
  static USBTask* task = nullptr;

  ESP_LOGD(TAG, "Starting USB task");
  if (task)
    return ESP_FAIL;

  task = new USBTask();
  return task->Initialize();
}

esp_err_t USBTask::Initialize() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  esp_err_t err = usb::Device::Initialize();
  if (err != ESP_OK)
    return err;

  return xTaskCreate(TaskFunc, "usb-tick", kStackDepthWords, this,
                     tskIDLE_PRIORITY + 1, &task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

// static
void IRAM_ATTR USBTask::TaskFunc(void* arg) {
  while (true) {
    // Doing this at such a high frequency causes problems.
    // Also, not sure it is necessary, keyboard simulator seems to run
    // fine without it.
#if 0
    if (usb::Device::Suspended()) {
      if (usb::Device::RemoteWakup() != ESP_OK)
        ESP_LOGE(TAG, "Error waking up");
      vTaskDelay(pdMS_TO_TICKS(500));
      continue;
    }
    if (!usb::HID::Ready()) {
      ESP_LOGV(TAG, "USB not ready.");
      vTaskDelay(pdMS_TO_TICKS(1000));
      continue;
    }
#endif
    usb::Device::Tick();
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}
