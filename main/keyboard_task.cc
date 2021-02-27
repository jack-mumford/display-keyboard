#include "keyboard_task.h"

#include <string>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>

#include "gpio_pins.h"
#include "usb_device.h"
#include "usb_hid.h"

namespace {
constexpr char TAG[] = "KbdTask";

// Interrupt allocation flags.
// Combination of  ESP_INTR_FLAG_* flags.
constexpr int ESP_INTR_FLAG_DEFAULT = 0x0;  // No flags set.

constexpr EventBits_t EVENT_KEYBOARD_EVENT = BIT0;
constexpr EventBits_t EVENT_ALL = BIT0;
}  // namespace

KeyboardTask::KeyboardTask()
    : event_group_(xEventGroupCreate()),
      keyboard_(i2c::Master(kKeyboardPort, /*mutex=*/nullptr)) {}

KeyboardTask::~KeyboardTask() {
  if (event_group_)
    vEventGroupDelete(event_group_);
}

// static
esp_err_t KeyboardTask::Start() {
  static KeyboardTask* task = nullptr;

  ESP_LOGD(TAG, "Starting Keyboard task");
  if (task)
    return ESP_FAIL;

  task = new KeyboardTask();
  return task->Initialize();
}

esp_err_t KeyboardTask::Initialize() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  if (!event_group_)
    return ESP_FAIL;

  esp_err_t err = InstallKeyboardISR();
  if (err != ESP_OK)
    return err;

  err = keyboard_.Initialize();
  if (err != ESP_OK)
    return err;

  return xTaskCreate(TaskFunc, TAG, kStackDepthWords, this,
                     tskIDLE_PRIORITY + 1, &task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

void IRAM_ATTR KeyboardTask::Run() {
  ESP_LOGW(TAG, "In keyboard task.");
  while (true) {
    EventBits_t bits =
        xEventGroupWaitBits(event_group_, EVENT_ALL, /*xClearOnExit=*/pdTRUE,
                            /*xWaitForAllBits=*/pdFALSE, portMAX_DELAY);
    if (bits & EVENT_KEYBOARD_EVENT) {
      keyboard_.HandleEvents();
    }
  }
}

// static
void IRAM_ATTR KeyboardTask::TaskFunc(void* arg) {
  static_cast<KeyboardTask*>(arg)->Run();
}

// static
void IRAM_ATTR KeyboardTask::KeyboardISR(void* arg) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  BaseType_t xResult = xEventGroupSetBitsFromISR(
      static_cast<KeyboardTask*>(arg)->event_group_, EVENT_KEYBOARD_EVENT,
      &xHigherPriorityTaskWoken);

  if (xResult != pdFAIL) {
    // See https://www.freertos.org/xEventGroupSetBitsFromISR.html
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

esp_err_t KeyboardTask::InstallKeyboardISR() {
  constexpr uint64_t pin_mask = 1ULL << kKeyboardINTGPIO;
  constexpr gpio_config_t io_conf = {
      .pin_bit_mask = pin_mask,
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_ANYEDGE,
  };

  esp_err_t err = gpio_config(&io_conf);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Unable to config keyboard INT pin: %s.",
             esp_err_to_name(err));
    return err;
  }

  err = gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "gpio_install_isr_service failure: %s.",
             esp_err_to_name(err));
    return err;
  }

  err = gpio_isr_handler_add(kKeyboardINTGPIO, KeyboardISR, this);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "gpio_isr_handler_add failure: %s.", esp_err_to_name(err));
    return err;
  }
  ESP_LOGD(TAG, "Keyboard interrupt handler installed on GPIO %u.",
           kKeyboardINTGPIO);
  return ESP_OK;
}
