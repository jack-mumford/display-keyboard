#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "https_client.h"

#include <cstring>

// Needs to be included early.
#include <mbedtls/esp_config.h>

#include <esp_crt_bundle.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_tls.h>

namespace {
constexpr char TAG[] = "kbd_https";

esp_tls_t* CreateConnection(const std::string& url) {
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

  return esp_tls_conn_http_new(url.c_str(), &cfg);
}

esp_err_t WriteData(esp_tls_t* tls, const void* data, size_t num_bytes) {
  size_t bytes_written = 0;
  do {
    ssize_t ret =
        esp_tls_conn_write(tls, static_cast<const char*>(data) + bytes_written,
                           num_bytes - bytes_written);
    if (ret >= 0) {
      ESP_LOGD(TAG, "%d bytes written", ret);
      bytes_written += ret;
    } else if (ret != ESP_TLS_ERR_SSL_WANT_READ &&
               ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
      ESP_LOGE(TAG, "esp_tls_conn_write  returned 0x%x", ret);
      return ESP_FAIL;
    }
  } while (bytes_written < num_bytes);

  return ESP_OK;
}

esp_err_t ReadData(esp_tls_t* tls, std::string* response) {
  char buffer[512];
  do {
    int len = sizeof(buffer) - 1;
    bzero(buffer, sizeof(buffer));
    len = esp_tls_conn_read(tls, buffer, len);
    if (len == ESP_TLS_ERR_SSL_WANT_WRITE || len == ESP_TLS_ERR_SSL_WANT_READ)
      continue;
    if (len < 0) {
      ESP_LOGE(TAG, "esp_tls_conn_read  returned -0x%x", -len);
      break;
    }
    if (len == 0) {
      ESP_LOGI(TAG, "connection closed");
      break;
    }
    ESP_LOGD(TAG, "%d bytes read", len);
    response->append(buffer, len);
  } while (true);
  return ESP_OK;
}

}  // namespace

HTTPSClient::HTTPSClient(std::string user_agent)
    : user_agent_(std::move(user_agent)) {}

HTTPSClient::~HTTPSClient() = default;

esp_err_t HTTPSClient::DoSSLCheck() {
  std::string response;
  return DoGET("www.howsmyssl.com", "/a/check", std::vector<std::string>(),
               &response);
}

std::string HTTPSClient::CreateRequestString(
    const std::string& request_type,
    const std::string& host,
    const std::string& resource,
    const std::string& content,
    const std::vector<std::string>& header_values) const {
  std::string request = request_type + " https://" + host + resource +
                        " HTTP/1.1\r\nHost: " + host +
                        "\r\nUser-Agent: " + user_agent_ + "\r\n";

  for (const std::string& value : header_values)
    request += value + "\r\n";

  if (!content.empty()) {
    char buff[40];
    std::snprintf(buff, sizeof(buff), "Content-Length: %u\r\n",
                  content.length());
    request += buff;
  }

  request += "\r\n";
  return request;
}

esp_err_t HTTPSClient::DoGET(const std::string& host,
                             const std::string& resource,
                             const std::vector<std::string>& header_values,
                             std::string* response) {
  const std::string request = CreateRequestString(
      "GET", host, resource, /*content=*/std::string(), header_values);
  if (request.empty())
    return ESP_ERR_NO_MEM;

  const std::string url = "https://" + host + resource;
  esp_tls_t* tls = CreateConnection(url);
  if (!tls) {
    ESP_LOGE(TAG, "Connection failed...");
    return ESP_FAIL;
  }

  ESP_LOGI(TAG, "Connection established, sending request...");
  esp_err_t err = WriteData(tls, request.c_str(), request.length());
  if (err != ESP_OK)
    goto exit;

  ESP_LOGI(TAG, "Reading HTTPS response...");
  err = ReadData(tls, response);
  if (err != ESP_OK)
    goto exit;

exit:
  esp_tls_conn_delete(tls);
  return ESP_OK;
}

esp_err_t HTTPSClient::DoPOST(const std::string& host,
                              const std::string& resource,
                              const std::string& content,
                              const std::vector<std::string>& header_values,
                              std::string* response) {
  const std::string request =
      CreateRequestString("POST", host, resource, content, header_values);
  if (request.empty())
    return ESP_ERR_NO_MEM;

  const std::string url = "https://" + host + resource;
  esp_tls_t* tls = CreateConnection(url);
  if (!tls) {
    ESP_LOGE(TAG, "Connection failed...");
    return ESP_FAIL;
  }

  ESP_LOGI(TAG, "Connection established, sending request...");
  ESP_LOGD(TAG, "REQUEST: %s", request.c_str());
  esp_err_t err = WriteData(tls, request.c_str(), request.length());
  if (err != ESP_OK) {
    esp_tls_conn_delete(tls);
    return err;
  }
  err = WriteData(tls, content.c_str(), content.length());
  if (err != ESP_OK) {
    esp_tls_conn_delete(tls);
    return err;
  }

  ESP_LOGI(TAG, "Reading HTTPS response...");
  char buffer[512];
  int ret, len;

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

  esp_tls_conn_delete(tls);
  putchar('\n');  // JSON output doesn't have a newline at end.

  static int request_count;
  ESP_LOGI(TAG, "Completed %d requests", ++request_count);
  return ESP_OK;
}