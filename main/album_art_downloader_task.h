#pragma once

#include <cstdint>
#include <memory>
#include <queue>
#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/event_groups.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include <esp_err.h>
#include <esp_http_client.h>

class ResourceFetchClient {
 public:
  /**
   * @brief Called when a resource request was completed successfully.
   *
   * @note check \p http_status_code for 200 before using \p resource_data.
   *
   * @warning Can be called on any thread/task.
   */
  virtual void FetchResult(uint32_t request_id,
                           int http_status_code,
                           std::string resource_data) = 0;

  /**
   * @brief Called when an error occurred fetching a resource.
   *
   * @warning Can be called on any thread/task.
   */
  virtual void FetchError(uint32_t request_id, esp_err_t err) = 0;

 protected:
  ResourceFetchClient() = default;
  ~ResourceFetchClient() = default;
};

/**
 * @brief Download a single album cover image and then self delete.
 */
class AlbumArtDownloaderTask {
 public:
  static AlbumArtDownloaderTask* Start(ResourceFetchClient* fetch_client);

  /**
   * @brief Queue a resource to fetch.
   *
   * @note This is threadsafe.
   *
   * @param request_id
   * @param url
   */
  void QueueFetch(uint32_t request_id, std::string url);

 private:
  struct RequestData {
    RequestData(uint32_t request_id, std::string url)
        : request_id(request_id), url(std::move(url)) {}
    uint32_t request_id;
    std::string url;
  };
  static void IRAM_ATTR TaskFunc(void* arg);

  void IRAM_ATTR Run();
  void DownloadResource(RequestData resource);
  AlbumArtDownloaderTask(ResourceFetchClient* fetch_client);
  ~AlbumArtDownloaderTask() = default;

  esp_err_t Initialize();

  SemaphoreHandle_t mutex_;
  EventGroupHandle_t event_group_;    // Fetch events.
  std::queue<RequestData> requests_;  // Outstanding requests.
  ResourceFetchClient* fetch_client_;
  TaskHandle_t task_ = nullptr;
};
