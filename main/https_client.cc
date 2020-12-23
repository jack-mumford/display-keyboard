#include "https_client.h"

HTTPSClient::HTTPSClient(std::string user_agent)
    : user_agent_(std::move(user_agent)) {}

HTTPSClient::~HTTPSClient() = default;

std::string HTTPSClient::CreateRequest(
    const std::string& request_type,
    const std::string& host,
    const std::string& resource,
    const std::vector<std::string>& header_values) const {
  std::string request = request_type + " https://" + host + resource +
                        "HTTP/1.0/\r\nHost: " + host +
                        "\r\nUser-Agent: " + user_agent_ + "\r\n";

  for (const std::string& value : header_values)
    request += value + "\r\n";

  request += "\r\n";
  return request;
}