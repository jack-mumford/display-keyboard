#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include "https_client.h"

#include <esp_crt_bundle.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_tls.h>

namespace {
constexpr char TAG[] = "kbd_httpc";
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
  return DoGET("https://www.howsmyssl.com/a/check", std::vector<HeaderValue>(),
               &response);
}

esp_err_t HTTPSClient::DoGET(const std::string& url,
                             const std::vector<HeaderValue>& header_values,
                             std::string* response) {
  const esp_http_client_config_t config = {
      .url = url.c_str(),
      .auth_type = HTTP_AUTH_TYPE_BASIC,
      //.cert_pem = howsmyssl_com_root_cert_pem_start,
      .method = HTTP_METHOD_GET,
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

esp_err_t HTTPSClient::DoPOST(const std::string& url,
                              const std::string& content,
                              const std::vector<HeaderValue>& header_values,
                              std::string* response) {
  const esp_http_client_config_t config = {
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
