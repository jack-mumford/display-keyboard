#include "resource_fetcher.h"

#include <cstring>
#include <memory>

#ifdef LOG_LOCAL_LEVEL
// <ssd1306.h> defines this (incorrectly), so undefine.
#undef LOG_LOCAL_LEVEL
#endif
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#include <lv_lib_split_jpg/tjpgd.h>

#include "http_client.h"

namespace {

/**
 * The JPEG decoding "device" - used when decoding.
 */
struct IODEV {
  size_t image_data_pos;   // Read position within |image_data|.
  std::string image_data;  // Full compressed image data being decompressed.
};

constexpr char TAG[] = "Fetcher";

constexpr EventBits_t FETCH_EVENT = BIT0;
constexpr EventBits_t EVENT_ALL = BIT0;

bool is_jpeg(const std::string& mime_type) {
  return mime_type == "image/jpeg";
}

/**
 * JPEG decompressor input callback function.
 *
 * @param jd Decompression object.
 * @param buff Pointer to the read buffer (null to remove data).
 * @param buff Number of bytes to read/skip.
 *
 * @return number of bytes read (zero on error).
 */
unsigned int input_func(JDEC* jd, uint8_t* buff, unsigned int ndata) {
  IODEV* dev = static_cast<IODEV*>(jd->device);

  size_t bytes_left = dev->image_data.size() - dev->image_data_pos;
  if (!bytes_left)
    return 0;
  if (ndata < bytes_left)
    ndata = bytes_left;

  if (buff) {
    // Read bytes from current position.
    std::memcpy(buff, dev->image_data.c_str() + dev->image_data_pos, ndata);
  }

  // Advance current position.
  dev->image_data_pos += ndata;
  return ndata;
}

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
  std::string mime_type;
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

  constexpr unsigned int kWorkPoolSize = 4096;
  std::unique_ptr<uint8_t[]> work_pool(new uint8_t[kWorkPoolSize]);
  std::unique_ptr<JDEC> jd(new JDEC);
  bzero(jd.get(), sizeof(JDEC));
  IODEV iodev;
  JRESULT res;

  if (!is_jpeg(mime_type))
    goto decode_error;

  iodev.image_data = std::move(response);
  iodev.image_data_pos = 0;

  res =
      jd_prepare(jd.get(), input_func, work_pool.get(), kWorkPoolSize, &iodev);
  if (res != JDR_OK)
    goto decode_error;

decode_error:
  fetch_client_->FetchResult(resource.request_id, status_code,
                             std::move(response), std::move(mime_type));
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
