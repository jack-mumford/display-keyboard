#include "spotify.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_err.h>

#include <esp_crt_bundle.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_tls.h>

namespace {
constexpr char TAG[] = "kbd_spotify";

#define WEB_SERVER "www.howsmyssl.com"
#define WEB_PORT "443"
#define WEB_URL "https://www.howsmyssl.com/a/check"

constexpr char REQUEST[] = "GET " WEB_URL
                           " HTTP/1.0\r\n"
                           "Host: " WEB_SERVER
                           "\r\n"
                           "User-Agent: esp-idf/1.0 esp32\r\n"
                           "\r\n";

}  // namespace

Spotify::Spotify() = default;

Spotify::~Spotify() = default;

void Spotify::DoTest() {
  char buffer[512];
  int ret, len;
  size_t bytes_written = 0;

  const esp_tls_cfg_t cfg = {
      .crt_bundle_attach = esp_crt_bundle_attach,
  };

  struct esp_tls* tls = esp_tls_conn_http_new(WEB_URL, &cfg);
  if (tls != NULL) {
    ESP_LOGI(TAG, "Connection established...");
  } else {
    ESP_LOGE(TAG, "Connection failed...");
    goto exit;
  }

  do {
    ret = esp_tls_conn_write(tls, REQUEST + bytes_written,
                             strlen(REQUEST) - bytes_written);
    if (ret >= 0) {
      ESP_LOGI(TAG, "%d bytes written", ret);
      bytes_written += ret;
    } else if (ret != ESP_TLS_ERR_SSL_WANT_READ &&
               ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
      ESP_LOGE(TAG, "esp_tls_conn_write  returned 0x%x", ret);
      goto exit;
    }
  } while (bytes_written < strlen(REQUEST));

  ESP_LOGI(TAG, "Reading HTTP response...");

  do {
    len = sizeof(buffer) - 1;
    bzero(buffer, sizeof(buffer));
    ret = esp_tls_conn_read(tls, (char*)buffer, len);

    if (ret == ESP_TLS_ERR_SSL_WANT_WRITE || ret == ESP_TLS_ERR_SSL_WANT_READ)
      continue;

    if (ret < 0) {
      ESP_LOGE(TAG, "esp_tls_conn_read  returned -0x%x", -ret);
      break;
    }

    if (ret == 0) {
      ESP_LOGI(TAG, "connection closed");
      break;
    }

    len = ret;
    ESP_LOGD(TAG, "%d bytes read", len);
    /* Print response directly to stdout as it is read */
    for (int i = 0; i < len; i++) {
      putchar(buffer[i]);
    }
  } while (true);

exit:
  esp_tls_conn_delete(tls);
  putchar('\n');  // JSON output doesn't have a newline at end

  static int request_count;
  ESP_LOGI(TAG, "Completed %d requests", ++request_count);

  for (int countdown = 10; countdown >= 0; countdown--) {
    ESP_LOGI(TAG, "%d...", countdown);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  ESP_LOGI(TAG, "Starting again!");
}