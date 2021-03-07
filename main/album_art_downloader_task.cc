#include "album_art_downloader_task.h"

#include <memory>

#ifdef LOG_LOCAL_LEVEL
// <ssd1306.h> defines this (incorrectly), so undefine.
#undef LOG_LOCAL_LEVEL
#endif
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>

#include "http_client.h"
#include "ui_task.h"

namespace {
constexpr char TAG[] = "CoverTask";
}

AlbumArtDownloaderTask::AlbumArtDownloaderTask(std::string url, UITask* ui_task)
    : url_(std::move(url)), ui_task_(ui_task) {}

// static
esp_err_t AlbumArtDownloaderTask::Start(std::string url, UITask* ui_task) {
  ESP_LOGD(TAG, "Starting AlbumArtDownloaderTask");

  AlbumArtDownloaderTask* task =
      new AlbumArtDownloaderTask(std::move(url), ui_task);
  if (!task)
    return ESP_ERR_NO_MEM;
  esp_err_t err = task->Initialize();
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Cannot initialize: %s", esp_err_to_name(err));
    delete task;
    return err;
  }
  return ESP_OK;
}

esp_err_t AlbumArtDownloaderTask::Initialize() {
  // https://www.freertos.org/FAQMem.html#StackSize
  constexpr uint32_t kStackDepthWords = 2048;

  return xTaskCreate(TaskFunc, TAG, kStackDepthWords, this, tskIDLE_PRIORITY,
                     &task_) == pdPASS
             ? ESP_OK
             : ESP_FAIL;
}

// static
void IRAM_ATTR AlbumArtDownloaderTask::TaskFunc(void* arg) {
  std::unique_ptr<AlbumArtDownloaderTask> task(
      static_cast<AlbumArtDownloaderTask*>(arg));

  std::string response;
  HTTPClient https_client;
  int status_code(0);
  const std::vector<HTTPClient::HeaderValue> header_values;
  ESP_LOGD(TAG, "GETting %s", task->url_.c_str());
  esp_err_t err = https_client.DoGET(
      task->url_, header_values,
      [&response](const void* data, int data_len) {
        response.append(static_cast<const char*>(data), data_len);
        return ESP_OK;
      },
      &status_code);

  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error GETting %s: %s", task->url_.c_str(),
             esp_err_to_name(err));
    task->ui_task_->SetAlbumCoverDownloadError();
    return;
  }

  if (status_code != HttpStatus_Ok) {
    ESP_LOGE(TAG, "Request error: %d", status_code);
    task->ui_task_->SetAlbumCoverDownloadError();
    return;
  }

  if (response.empty()) {
    ESP_LOGE(TAG, "Got empty response.");
    task->ui_task_->SetAlbumCoverDownloadError();
    return;
  }

  task->ui_task_->SetAlbumCoverImage(std::move(response));
}
