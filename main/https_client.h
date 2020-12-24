#pragma once

#include <string>
#include <vector>

#include <esp_err.h>

class HTTPSClient {
 public:
  HTTPSClient(std::string user_agent);
  ~HTTPSClient();

  esp_err_t DoGET(const std::string& host,
                  const std::string& resource,
                  const std::vector<std::string>& header_values,
                  std::string* response);

  esp_err_t DoPOST(const std::string& host,
                   const std::string& resource,
                   const std::string& content,
                   const std::vector<std::string>& header_values,
                   std::string* response);

  esp_err_t DoSSLCheck();

 private:
  std::string CreateRequestString(
      const std::string& request_type,
      const std::string& host,
      const std::string& resource,
      const std::string& content,
      const std::vector<std::string>& header_values) const;
  const std::string user_agent_;
};