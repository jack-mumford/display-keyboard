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
    taskYIELD();
  } while (bytes_written < num_bytes);

  return ESP_OK;
}

esp_err_t ReadData(esp_tls_t* tls) {
  char buffer[512];
  do {
    int len = sizeof(buffer) - 1;
    bzero(buffer, sizeof(buffer));
    len = esp_tls_conn_read(tls, buffer, len);
    if (len == ESP_TLS_ERR_SSL_WANT_WRITE || len == ESP_TLS_ERR_SSL_WANT_READ)
      continue;
    if (len < 0) {
      ESP_LOGE(TAG, "esp_tls_conn_read returned -0x%x", -len);
      break;
    }
    if (len == 0) {
      ESP_LOGD(TAG, "connection closed");
      break;
    }
    ESP_LOGD(TAG, "%d bytes read", len);
    taskYIELD();
  } while (true);
  return ESP_OK;
}

}  // namespace

// static
esp_err_t HTTPSClient::EventHandler(esp_http_client_event_t* evt) {
  HTTPSClient* client = static_cast<HTTPSClient*>(evt->user_data);

  switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
      client->last_request_data_.clear();
      break;
    case HTTP_EVENT_ON_CONNECTED:
      client->last_request_data_.clear();
      break;
    case HTTP_EVENT_ON_DATA:
      ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
      client->last_request_data_.append(static_cast<const char*>(evt->data),
                                        evt->data_len);
      break;
    default:
      // fallthrough
      break;
  }
  return ESP_OK;
}

HTTPSClient::HTTPSClient(std::string user_agent)
    : user_agent_(std::move(user_agent)) {}

HTTPSClient::~HTTPSClient() = default;

esp_err_t HTTPSClient::DoSSLCheck() {
  std::string response;
  return DoGET("www.howsmyssl.com", "/a/check", std::vector<HeaderValue>(),
               &response);
}

std::string HTTPSClient::CreateRequestString(
    const std::string& request_type,
    const std::string& host,
    const std::string& resource,
    const std::string& content,
    const std::vector<HeaderValue>& header_values) const {
  std::string request = request_type + " https://" + host + resource +
                        " HTTP/1.1\r\nHost: " + host +
                        "\r\nUser-Agent: " + user_agent_ + "\r\n";

  for (const HeaderValue& value : header_values)
    request += value.first + ": " + value.second + "\r\n";

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
                             const std::vector<HeaderValue>& header_values,
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

  ESP_LOGD(TAG, "Connection established, sending request...");
  esp_err_t err = WriteData(tls, request.c_str(), request.length());
  if (err != ESP_OK)
    goto exit;

  err = ReadData(tls);
  *response = std::move(last_request_data_);

exit:
  esp_tls_conn_delete(tls);
  return err;
}

esp_err_t HTTPSClient::DoPOST(const std::string& url,
                              const std::string& content,
                              const std::vector<HeaderValue>& header_values,
                              std::string* response) {
  esp_http_client_config_t config = {
      .url = url.c_str(),
      .auth_type = HTTP_AUTH_TYPE_BASIC,
      //.cert_pem = howsmyssl_com_root_cert_pem_start,
      .method = HTTP_METHOD_POST,
      .event_handler = HTTPSClient::EventHandler,
      .user_data = this,
  };
  esp_http_client_handle_t client = esp_http_client_init(&config);
  if (!client)
    return ESP_FAIL;
  esp_err_t err = ESP_OK;
  for (const auto& value : header_values) {
    err = esp_http_client_set_header(client, value.first.c_str(),
                                     value.second.c_str());
    if (err != ESP_OK)
      goto exit;
  }
  err =
      esp_http_client_set_post_field(client, content.data(), content.length());
  if (err != ESP_OK)
    goto exit;
  err = esp_http_client_perform(client);

exit:
  esp_http_client_cleanup(client);
  if (err == ESP_OK) {
    *response = std::move(last_request_data_);
    ESP_LOGI(TAG, "Status = %d, content_length = %d",
             esp_http_client_get_status_code(client),
             esp_http_client_get_content_length(client));
  }
  return err;
}
