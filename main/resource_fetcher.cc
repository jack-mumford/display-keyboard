#include "resource_fetcher.h"

#include <memory>

#ifdef LOG_LOCAL_LEVEL
// <ssd1306.h> defines this (incorrectly), so undefine.
#undef LOG_LOCAL_LEVEL
#endif
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>

#include "http_client.h"

namespace {
constexpr char TAG[] = "Fetcher";

constexpr EventBits_t FETCH_EVENT = BIT0;
constexpr EventBits_t EVENT_ALL = BIT0;
}  // namespace

ResourceFetcher::ResourceFetcher(ResourceFetchClient* fetch_client)
    : mutex_(xSemaphoreCreateMutex()),
      event_group_(xEventGroupCreate()),
      fetch_client_(fetch_client) {
  configASSERT(fetch_client);
}

// static
ResourceFetcher* ResourceFetcher::Start(ResourceFetchClient* fetch_client) {
  static ResourceFetcher* task;

  if (task)
    return task;

  ESP_LOGD(TAG, "Starting Fetcher task");
  task = new ResourceFetcher(fetch_client);
  if (!task)
    return nullptr;
  esp_err_t err = task->Initialize();
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Cannot initialize: %s", esp_err_to_name(err));
    delete task;
    task = nullptr;
    return nullptr;
  }
  return task;
}

esp_err_t ResourceFetcher::Initialize() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 4096;

  if (!event_group_)
    return ESP_FAIL;

  return xTaskCreate(TaskFunc, TAG, kStackDepthWords, this,
                     tskIDLE_PRIORITY + 1, &task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

void ResourceFetcher::QueueFetch(uint32_t request_id, std::string url) {
  if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE)
    return;
  requests_.emplace(request_id, url);
  xSemaphoreGive(mutex_);
  xEventGroupSetBits(event_group_, FETCH_EVENT);
}

void ResourceFetcher::DownloadResource(RequestData resource) {
  std::string response;
  HTTPClient https_client;
  int status_code(0);
  const std::vector<HTTPClient::HeaderValue> header_values;
  ESP_LOGD(TAG, "GETting %s", resource.url.c_str());
  esp_err_t err = https_client.DoGET(
      resource.url, header_values,
      [&response](const void* data, int data_len) {
        response.append(static_cast<const char*>(data), data_len);
        return ESP_OK;
      },
      &status_code);

  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error GETting %s: %s", resource.url.c_str(),
             esp_err_to_name(err));
    fetch_client_->FetchError(resource.request_id, err);
    return;
  }

  fetch_client_->FetchResult(resource.request_id, status_code,
                             std::move(response));
}

void IRAM_ATTR ResourceFetcher::Run() {
  while (true) {
    EventBits_t bits =
        xEventGroupWaitBits(event_group_, EVENT_ALL, /*xClearOnExit=*/pdFALSE,
                            /*xWaitForAllBits=*/pdFALSE, portMAX_DELAY);
    if (bits & FETCH_EVENT) {
      if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE)
        return;
      configASSERT(!requests_.empty());
      RequestData resource = requests_.front();
      requests_.pop();
      xSemaphoreGive(mutex_);
      DownloadResource(std::move(resource));

      if (xSemaphoreTake(mutex_, portMAX_DELAY) != pdTRUE)
        return;
      if (requests_.empty())
        xEventGroupClearBits(event_group_, FETCH_EVENT);
      xSemaphoreGive(mutex_);
    }
  }
}

// static
void IRAM_ATTR ResourceFetcher::TaskFunc(void* arg) {
  static_cast<ResourceFetcher*>(arg)->Run();
}
