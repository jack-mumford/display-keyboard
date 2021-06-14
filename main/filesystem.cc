#include "filesystem.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#include <esp_spiffs.h>

namespace {
constexpr char TAG[] = "FS";
}

Filesystem::Filesystem() = default;

Filesystem::~Filesystem() = default;

esp_err_t Filesystem::Initialize() {
  constexpr esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = nullptr,
      .max_files = 5,  // Max number open at a time.
      .format_if_mount_failed = true,
  };

  esp_err_t err = esp_vfs_spiffs_register(&conf);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Error registering filesystem: %s", esp_err_to_name(err));
    return err;
  }

  size_t total = 0, used = 0;
  err = esp_spiffs_info(conf.partition_label, &total, &used);
  if (err != ESP_OK)
    ESP_LOGE(TAG, "Failed to get partition info: %s", esp_err_to_name(err));
  else
    ESP_LOGI(TAG, "Partition size: total: %zu, used: %zu", total, used);

  return err;
}
