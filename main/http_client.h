#pragma once

#include <string>
#include <tuple>
#include <vector>

#include <esp_err.h>
#include <esp_http_client.h>

class HTTPClient {
 public:
  using HeaderValue = std::pair<std::string, std::string>;

  HTTPClient();
  ~HTTPClient();

  esp_err_t DoGET(const std::string& url,
                  const std::vector<HeaderValue>& header_values,
                  std::string* response);

  esp_err_t DoPOST(const std::string& url,
                   const std::string& content,
                   const std::vector<HeaderValue>& header_values,
                   std::string* response);

  esp_err_t DoSSLCheck();

 private:
  static esp_err_t EventHandler(esp_http_client_event_t* evt);

  std::string last_response_data_;  // contents of the current request data.
};