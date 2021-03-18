#include "ui_task.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lv_lib_png/lv_png.h>
#include <lvgl.h>
#include <lvgl_helpers.h>

#include "gpio_pins.h"
#include "main_display.h"
#include "main_screen.h"
#include "resource_fetcher.h"

namespace {

constexpr uint32_t kStackDepthWords = 4 * 1024;
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

void UITask::UpdateTime() {
  constexpr TickType_t kUpdateTimeTimeout = 2000 / portTICK_PERIOD_MS;
  if (xSemaphoreTake(mutex_, kUpdateTimeTimeout) == pdTRUE) {
    if (main_display_.screen())
      main_display_.screen()->UpdateTime();
    xSemaphoreGive(mutex_);
  }
}

// static
void IRAM_ATTR UITask::UpdateTimeCb(void* arg) {
  static_cast<UITask*>(arg)->UpdateTime();
}

esp_err_t UITask::CreateUpdateTimeTimer() {
  const esp_timer_create_args_t timer_args = {
    .callback = UpdateTimeCb,
    .arg = this,
    .dispatch_method = ESP_TIMER_TASK,
    .name = "UITask::UpdateTimeCb",
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 3, 0)
    .skip_unhandled_events = true,
#endif
  };
  esp_err_t err = esp_timer_create(&timer_args, &time_update_timer_);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Unable to create the periodic tick timer");
    return err;
  }
  constexpr uint64_t kUpdateTimePeriodUsec = 1000000;  // every second.
  err = esp_timer_start_periodic(time_update_timer_, kUpdateTimePeriodUsec);
  if (err != ESP_OK)
    ESP_LOGE(TAG, "Unable to start the time update timer");
  return err;
}

esp_err_t UITask::Initialize() {
  ESP_LOGD(TAG, "Initializing UI task");

  if (!mutex_)
    return ESP_FAIL;

  const esp_timer_create_args_t cover_art_timer_args = {
      .callback = TestCoverArtTimerCb,
      .arg = this,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "Cover Art",
      .skip_unhandled_events = true,
  };
  esp_err_t err =
      esp_timer_create(&cover_art_timer_args, &test_cover_art_timer_);
  if (err != ESP_OK)
    return err;

  fetcher_ = ResourceFetcher::Start(this);
  if (!fetcher_)
    return ESP_FAIL;

  // Create a new task for LVGL drawing. Don't believe this needs to
  // be pinned to a single core, but doing so on a dual-core MCU
  // will reserve the other core for WiFi and other activities.
  return pdPASS == xTaskCreatePinnedToCore(TaskFunc, "UI", kStackDepthWords,
                                           this, tskIDLE_PRIORITY + 1, &task_,
                                           tskNO_AFFINITY)
             ? ESP_OK
             : ESP_FAIL;
}

void UITask::SetDarkMode() {
  // TODO: Set dark mode and background color at compile-time.
  // This should be do-able, but the theme in sdkconfig seems to
  // be ignored. Maybe using lv_conf.h will work.
  lv_theme_material_init(
      lv_theme_get_color_primary(), lv_theme_get_color_secondary(),
      LV_THEME_MATERIAL_FLAG_DARK, lv_theme_get_font_small(),
      lv_theme_get_font_normal(), lv_theme_get_font_subtitle(),
      lv_theme_get_font_title());

  static lv_style_t style;
  lv_style_init(&style);
  lv_style_set_bg_color(&style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
  lv_obj_add_style(main_display_.lv_screen(), LV_OBJ_PART_MAIN, &style);
}

void IRAM_ATTR UITask::Run() {
  ESP_LOGD(TAG, "Running.");
  if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE)
    return;
  lv_init();
  lvgl_driver_init();

  ESP_ERROR_CHECK(main_display_.Initialize());
  SetDarkMode();
  ESP_ERROR_CHECK(CreateTickTimer());
  ESP_ERROR_CHECK(CreateUpdateTimeTimer());
  ESP_ERROR_CHECK(StartTestCoverArtTimer(2));
  xSemaphoreGive(mutex_);

  while (true) {
    uint32_t wait_msecs = kMinMainLoopWaitMSecs;
    if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE) {
      wait_msecs = lv_task_handler() / 1000;
      xSemaphoreGive(mutex_);
      if (wait_msecs < kMinMainLoopWaitMSecs)
        wait_msecs = kMinMainLoopWaitMSecs;
      else if (wait_msecs > kMaxMainLoopWaitMSecs)
        wait_msecs = kMaxMainLoopWaitMSecs;
    }
    vTaskDelay(pdMS_TO_TICKS(wait_msecs));
  }
}

// static
void IRAM_ATTR UITask::TaskFunc(void* arg) {
  static_cast<UITask*>(arg)->Run();
}

// static
void UITask::SetWiFiStatus(WiFiStatus status) {
  configASSERT(g_ui_task);
  if (xSemaphoreTake(g_ui_task->mutex_, portMAX_DELAY) != pdTRUE)
    return;
  g_ui_task->wifi_status_ = status;
  if (g_ui_task->main_display_.screen())
    g_ui_task->main_display_.screen()->SetWiFiStatus(status);
  xSemaphoreGive(g_ui_task->mutex_);
}

std::string UITask::GetTestCoverArtURL() const {
  char* tmp(nullptr);
  if (asprintf(&tmp, "http://10.0.9.120/album_covers/album_%u_cover.jpg",
               test_cover_art_img_idx_) < 0) {
    return std::string();
  }

  std::string url(tmp);
  free(tmp);
  return url;
}

// static
void UITask::TestCoverArtTimerCb(void* arg) {
  UITask* task = static_cast<UITask*>(arg);

  if (xSemaphoreTake(task->mutex_, portMAX_DELAY) != pdTRUE)
    return;

  if (task->wifi_status_ != WiFiStatus::Online) {
    task->StartTestCoverArtTimer(1);
    xSemaphoreGive(task->mutex_);
    return;
  }

  char buf[20];
  snprintf(buf, sizeof(buf), "Fetching cover %d",
           task->test_cover_art_img_idx_);
  task->main_display_.screen()->SetDebugString(buf);
  task->fetcher_->QueueFetch(task->next_fetch_id_, task->GetTestCoverArtURL());
  xSemaphoreGive(task->mutex_);
}

esp_err_t UITask::StartTestCoverArtTimer(uint32_t timer_seconds) {
  const uint64_t timeout_us = timer_seconds * 1000 * 1000;
  return esp_timer_start_once(test_cover_art_timer_, timeout_us);
}

void UITask::FetchImageResult(uint32_t request_id, lv_img_dsc_t image) {
  configASSERT(main_display_.screen());
  if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE)
    return;
  char msg[20];
  snprintf(msg, sizeof(msg), "Got cover %u (#%u).", test_cover_art_img_idx_,
           next_fetch_id_);
  main_display_.screen()->SetDebugString(msg);
  main_display_.screen()->SetAlbumArtwork(std::move(image));
  if (++test_cover_art_img_idx_ > 9)
    test_cover_art_img_idx_ = 1;
  StartTestCoverArtTimer(8);
  xSemaphoreGive(mutex_);
}

void UITask::FetchResult(uint32_t request_id,
                         int http_status_code,
                         std::vector<uint8_t> resource_data,
                         std::string mime_type) {
  if (xSemaphoreTake(mutex_, portMAX_DELAY) == pdTRUE)
    return;

  char msg[20];
  if (http_status_code == HttpStatus_Ok) {
    snprintf(msg, sizeof(msg), "Unexpected fetch: %zu", resource_data.size());
  } else {
    ESP_LOGW(TAG, "Unable to fetch resource: status_code: %d",
             http_status_code);
    snprintf(msg, sizeof(msg), "Fetch code: %d.", http_status_code);
  }
  msg[sizeof(msg) - 1] = '\0';
  main_display_.screen()->SetDebugString(msg);
  if (++test_cover_art_img_idx_ > 9)
    test_cover_art_img_idx_ = 1;
  StartTestCoverArtTimer(5);
  xSemaphoreGive(mutex_);
}

void UITask::FetchError(uint32_t request_id, esp_err_t err) {
  ESP_LOGE(TAG, "Error fetching resource: %s", esp_err_to_name(err));
  if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE)
    return;

  char msg[40];
  snprintf(msg, sizeof(msg), "Fetch %u error: %s.", test_cover_art_img_idx_,
           esp_err_to_name(err));
  msg[sizeof(msg) - 1] = '\0';
  main_display_.screen()->SetDebugString(msg);
  StartTestCoverArtTimer(3);
  xSemaphoreGive(mutex_);
}
