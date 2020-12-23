#include "https_client.h"

// Needs to be included early.
#include <mbedtls/esp_config.h>

#include <esp_crt_bundle.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_tls.h>

namespace {
constexpr char TAG[] = "kbd_https";
}

HTTPSClient::HTTPSClient(std::string user_agent)
    : user_agent_(std::move(user_agent)) {}

HTTPSClient::~HTTPSClient() = default;

std::string HTTPSClient::CreateRequest(
    const std::string& request_type,
    const std::string& host,
    const std::string& resource,
    const std::vector<std::string>& header_values) const {
  std::string request = request_type + " https://" + host + resource +
                        " HTTP/1.0\r\nHost: " + host +
                        "\r\nUser-Agent: " + user_agent_ + "\r\n";

  for (const std::string& value : header_values)
    request += value + "\r\n";

  request += "\r\n";
  return request;
}

esp_err_t HTTPSClient::DoRequest(
    const std::string& host,
    const std::string& resource,
    const std::vector<std::string>& header_values) {
  const std::string request =
      CreateRequest("GET", host, resource, header_values);
  if (request.empty())
    return ESP_ERR_NO_MEM;

  ESP_LOGI(TAG, "REQUEST: %s", request.c_str());
  ESP_LOGI(TAG, "Spotify::DoTest: %p", esp_crt_bundle_attach);
  const esp_tls_cfg_t cfg = {
      .alpn_protos = nullptr,
      .cacert_buf = nullptr,
      .cacert_bytes = 0,
      .clientcert_buf = nullptr,
      .clientcert_bytes = 0,
      .clientkey_buf = nullptr,
      .clientkey_bytes = 0,
      .clientkey_password = nullptr,
      .clientkey_password_len = 0,
      .non_block = false,
      .use_secure_element = false,
      .timeout_ms = 0,
      .use_global_ca_store = false,
      .common_name = nullptr,
      .skip_common_name = false,
      .psk_hint_key = nullptr,
      .crt_bundle_attach = esp_crt_bundle_attach,
      .ds_data = nullptr,
  };

  const std::string url = "https://" + host + resource;
  esp_tls_t* tls = esp_tls_conn_http_new(url.c_str(), &cfg);
  if (!tls) {
    ESP_LOGE(TAG, "Connection failed...");
    return ESP_FAIL;
  }

  ESP_LOGI(TAG, "Connection established, sending request...");
  char buffer[512];
  int ret, len;
  size_t bytes_written = 0;

  do {
    ret = esp_tls_conn_write(tls, request.c_str() + bytes_written,
                             request.length() - bytes_written);
    if (ret >= 0) {
      ESP_LOGI(TAG, "%d bytes written", ret);
      bytes_written += ret;
    } else if (ret != ESP_TLS_ERR_SSL_WANT_READ &&
               ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
      ESP_LOGE(TAG, "esp_tls_conn_write  returned 0x%x", ret);
      goto exit;
    }
  } while (bytes_written < request.length());

  ESP_LOGI(TAG, "Reading HTTP response...");

  do {
    len = sizeof(buffer) - 1;
    bzero(buffer, sizeof(buffer));
    ret = esp_tls_conn_read(tls, buffer, len);

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
  putchar('\n');  // JSON output doesn't have a newline at end.

  static int request_count;
  ESP_LOGI(TAG, "Completed %d requests", ++request_count);
  return ESP_OK;
}