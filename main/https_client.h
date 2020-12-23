#pragma once

#include <string>
#include <vector>

#include <esp_err.h>

class HTTPSClient {
 public:
  HTTPSClient(std::string user_agent);
  ~HTTPSClient();

  esp_err_t DoRequest(const std::string& host,
                      const std::string& resource,
                      const std::vector<std::string>& header_values);

 private:
  std::string CreateRequest(
      const std::string& request_type,
      const std::string& host,
      const std::string& resource,
      const std::vector<std::string>& header_values) const;
  const std::string user_agent_;
};