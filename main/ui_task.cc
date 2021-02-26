#include "ui_task.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lv_lib_png/lv_png.h>
#include <lv_lib_split_jpg/lv_sjpg.h>
#include <lvgl.h>
#include <lvgl_helpers.h>

#include "gpio_pins.h"
#include "main_display.h"
#include "volume_display.h"

#ifndef DRAW_VOLUME_DISPLAY
#error Define DRAW_VOLUME_DISPLAY
#endif

namespace {

constexpr uint32_t kStackDepthWords = 16 * 1024;
constexpr uint64_t kMaxMainLoopWaitMSecs = 100;
constexpr uint32_t kMinMainLoopWaitMSecs = 10;
constexpr uint64_t kTickTimerPeriodUsec = 1000;
constexpr char TAG[] = "UITask";

// Make sure min wait time is at least one tick.
static_assert((kMinMainLoopWaitMSecs / portTICK_PERIOD_MS) > 0);
static_assert(kMaxMainLoopWaitMSecs > kMinMainLoopWaitMSecs);

UITask* g_ui_task = nullptr;

}  // namespace

UITask::UITask() : mutex_(xSemaphoreCreateMutex()) {}

// static
esp_err_t UITask::Start() {
  if (g_ui_task)
    return ESP_FAIL;
  ESP_LOGD(TAG, "Starting UI task");
  g_ui_task = new UITask();
  return g_ui_task->Initialize();
}

void UITask::Tick() {
  int64_t now = esp_timer_get_time();
  uint32_t tick_period = last_tick_time_ == -1 ? 0 : now - last_tick_time_;
  lv_tick_inc(tick_period);
  last_tick_time_ = now;
}

void UITask::TickTimerCb(void* arg) {
  static_cast<UITask*>(arg)->Tick();
}

esp_err_t UITask::CreateTickTimer() {
  const esp_timer_create_args_t timer_args = {
    .callback = TickTimerCb,
    .arg = this,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "UITask::TickTimer",
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 3, 0)
    .skip_unhandled_events = true,
#endif
  };
  esp_err_t err = esp_timer_create(&timer_args, &tick_timer_);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Unable to create the periodic tick timer");
    return err;
  }
  err = esp_timer_start_periodic(tick_timer_, kTickTimerPeriodUsec);
  if (err != ESP_OK)
    ESP_LOGE(TAG, "Unable to start the periodic tick timer");
  return err;
}

esp_err_t UITask::Initialize() {
  ESP_LOGD(TAG, "Initializing UI task");

  // Create a new task for LVGL drawing. Don't believe this needs to
  // be pinned to a single core, but doing so on a dual-core MCU
  // will reserve the other core for WiFi and other activities.
  return pdPASS == xTaskCreatePinnedToCore(TaskFunc, "UI", kStackDepthWords,
                                           this, tskIDLE_PRIORITY, &task_,
                                           tskNO_AFFINITY)
             ? ESP_OK
             : ESP_FAIL;
}

void IRAM_ATTR UITask::Run() {
  ESP_LOGD(TAG, "Running.");
  lv_init();
  lvgl_driver_init();
  lv_png_init();
  lv_split_jpeg_init();

  ESP_ERROR_CHECK(main_display_.Initialize());
#if DRAW_VOLUME_DISPLAY == 1
  int vol = 0;
  int vol_increment = 1;
  uint32_t loop_count = 0;
  ESP_ERROR_CHECK(volume_display_.Initialize());
#endif
  ESP_ERROR_CHECK(CreateTickTimer());

  while (true) {
#if DRAW_VOLUME_DISPLAY == 1
    loop_count++;
    // Simple test to bounce volume up and down.
    if ((loop_count % 10) == 0) {
      vol += vol_increment;
      if (vol < 0) {
        vol = 1;
        vol_increment = 1;
      } else if (vol > 100) {
        vol = 99;
        vol_increment = -1;
      }

      volume_display_.SetVolume(vol);
    }
#endif

#if 0
    if (uptate_display_time_) {
      struct tm now_local;
      {
        time_t now_epoch_coordinated_universal = 0;
        time(&now_epoch_coordinated_universal);
        localtime_r(&now_epoch_coordinated_universal, &now_local);
      }
      char tmbuf[64];
      asctime_r(&now_local, tmbuf);
      ESP_LOGI(TAG, "Current time: %s", tmbuf);
      // TODO: Actually update the display.
      uptate_display_time_ = false;
    }
#endif

    bool release_mutex = xSemaphoreTake(mutex_, portMAX_DELAY);
    uint32_t wait_msecs = lv_task_handler() / 1000;
    if (release_mutex)
      xSemaphoreGive(mutex_);
    if (wait_msecs < kMinMainLoopWaitMSecs)
      wait_msecs = kMinMainLoopWaitMSecs;
    else if (wait_msecs > kMaxMainLoopWaitMSecs)
      wait_msecs = kMaxMainLoopWaitMSecs;

    // Need to use vTaskDelay to avoid triggering the task WDT.
    vTaskDelay(pdMS_TO_TICKS(wait_msecs));
    taskYIELD();  // Not sure if this is necessary.
  }
}

// static
void IRAM_ATTR UITask::TaskFunc(void* arg) {
  static_cast<UITask*>(arg)->Run();
}

// static
void UITask::SetWiFiStatus(WiFiStatus status) {
  const bool release_mutex = xSemaphoreTake(g_ui_task->mutex_, portMAX_DELAY);
  g_ui_task->wifi_status_ = status;
  g_ui_task->main_display_.SetWiFiStatus(status);
  if (release_mutex)
    xSemaphoreGive(g_ui_task->mutex_);
}
