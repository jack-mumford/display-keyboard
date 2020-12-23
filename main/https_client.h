#pragma once

#include <string>
#include <vector>

class HTTPSClient {
 public:
  HTTPSClient(std::string user_agent);
  ~HTTPSClient();

 private:
  std::string CreateRequest(
      const std::string& request_type,
      const std::string& host,
      const std::string& resource,
      const std::vector<std::string>& header_values) const;
  const std::string user_agent_;
};