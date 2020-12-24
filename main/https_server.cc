#include "https_server.h"

HTTPSServer::HTTPSServer() : server_(nullptr) {}

HTTPSServer::~HTTPSServer() {
  if (server_)
    httpd_stop(server_);
}

esp_err_t HTTPSServer::Inilialize() {
  if (server_)
    return ESP_FAIL;
  const httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  return httpd_start(&server_, &config);
}

esp_err_t HTTPSServer::RegisterURIHandler(const httpd_uri_t* handler_info) {
  return httpd_register_uri_handler(server_, handler_info);
}

esp_err_t HTTPSServer::UnregisterURIHandler(const char* uri,
                                            httpd_method_t method) {
  return httpd_unregister_uri_handler(server_, uri, method);
}