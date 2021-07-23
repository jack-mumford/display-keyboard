#include "http_client.h"

#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#include <esp-tls/esp_tls.h>
#include <esp_log.h>
#include <mbedtls/esp_crt_bundle/include/esp_crt_bundle.h>

namespace {
constexpr char TAG[] = "HTTPClient";
}  // namespace

esp_http_client_config_t HTTPClient::CreateClientConfig(
    const std::string& url,
    esp_http_client_method_t method) {
  esp_http_client_config_t config = {
    .url = url.c_str(),
    .host = 0,
    .port = 0,
    .username = nullptr,
    .password = nullptr,
    .auth_type = HTTP_AUTH_TYPE_NONE,
    .path = nullptr,
    .query = nullptr,
    .cert_pem = nullptr,
    .client_cert_pem = nullptr,
    .client_key_pem = nullptr,
    .user_agent = nullptr,
    .method = method,
    .timeout_ms = 0,
    .disable_auto_redirect = false,
    .max_redirection_count = 0,
    .max_authorization_retries = 0,
    .event_handler = HTTPClient::EventHandler,
    .transport_type = HTTP_TRANSPORT_UNKNOWN,
    .buffer_size = 0,
    .buffer_size_tx = 0,
    .user_data = this,
    .is_async = false,
    .use_global_ca_store = false,
    .skip_cert_common_name_check = false,
    .keep_alive_enable = false,
    .keep_alive_idle = false,
    .keep_alive_interval = 0,
    .keep_alive_count = 0,
#if ESP_IDF_VERSION > ESP_IDF_VERSION_VAL(4, 3, 0)
    .if_name = nullptr,
#endif  // ESP-IDF 4.3.0
  };
  return config;
}

// static
esp_err_t HTTPClient::EventHandler(esp_http_client_event_t* evt) {
  HTTPClient* client = static_cast<HTTPClient*>(evt->user_data);

  switch (evt->event_id) {
    case HTTP_EVENT_ERROR:
      break;
    case HTTP_EVENT_ON_CONNECTED:
      break;
    case HTTP_EVENT_ON_DATA:
      ESP_LOGV(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
      if (client->data_callback_)
        return client->data_callback_(evt->data, evt->data_len);
      break;
    default:
      // fallthrough
      break;
  }
  return ESP_OK;
}

HTTPClient::HTTPClient() = default;

HTTPClient::~HTTPClient() = default;

esp_err_t HTTPClient::DoSSLCheck() {
  int status;
  return DoGET(
      "https://www.howsmyssl.com/a/check", std::vector<HeaderValue>(),
      [](const void*, int) { return ESP_OK; }, &status);
}

esp_err_t HTTPClient::DoGET(const std::string& url,
                            const std::vector<HeaderValue>& header_values,
                            DataCallback data_callback,
                            int* status_code) {
  const esp_http_client_config_t config =
      CreateClientConfig(url, HTTP_METHOD_GET);
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
  data_callback_ = data_callback;
  err = esp_http_client_perform(client);
  data_callback_ = nullptr;

exit:
  if (err == ESP_OK)
    *status_code = esp_http_client_get_status_code(client);
  esp_err_t e = esp_http_client_cleanup(client);
  if (err == ESP_OK)
    err = e;
  return err;
}

esp_err_t HTTPClient::DoPOST(const std::string& url,
                             const std::string& content,
                             const std::vector<HeaderValue>& header_values,
                             DataCallback data_callback,
                             int* status_code) {
  data_callback_ = data_callback;
  const esp_http_client_config_t config =
      CreateClientConfig(url, HTTP_METHOD_POST);
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
  data_callback_ = nullptr;
  esp_http_client_cleanup(client);
  if (err == ESP_OK)
    *status_code = esp_http_client_get_status_code(client);
  return err;
}
