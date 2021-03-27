#pragma once

#include <functional>
#include <string>
#include <tuple>
#include <vector>

#include <esp_err.h>
#include <esp_http_client/include/esp_http_client.h>

class HTTPClient {
 public:
  using HeaderValue = std::pair<std::string, std::string>;
  using DataCallback = std::function<esp_err_t(const void*, int)>;

  HTTPClient();
  ~HTTPClient();

  esp_err_t DoGET(const std::string& url,
                  const std::vector<HeaderValue>& header_values,
                  DataCallback data_callback,
                  int* status_code);

  esp_err_t DoPOST(const std::string& url,
                   const std::string& content,
                   const std::vector<HeaderValue>& header_values,
                   DataCallback data_callback,
                   int* status_code);

  esp_err_t DoSSLCheck();

 private:
  static esp_err_t EventHandler(esp_http_client_event_t* evt);

  esp_http_client_config_t CreateClientConfig(const std::string& url,
                                              esp_http_client_method_t method);

  DataCallback data_callback_;
};
