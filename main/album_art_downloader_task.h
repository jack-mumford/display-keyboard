#pragma once

#include <string>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_err.h>

class UITask;

/**
 * @brief Download a single album cover image and then self delete.
 */
class AlbumArtDownloaderTask {
 public:
  static esp_err_t Start(std::string url, UITask* ui_task);

 private:
  static void IRAM_ATTR TaskFunc(void* arg);

  AlbumArtDownloaderTask(std::string url, UITask* ui_task);

  esp_err_t Initialize();

  const std::string url_;  // Cover Artwork image URL to download.
  UITask* ui_task_;
  TaskHandle_t task_ = nullptr;
};
