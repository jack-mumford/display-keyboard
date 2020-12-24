#pragma once

#include <esp_err.h>
#include <esp_http_server.h>

class HTTPSServer {
 public:
  HTTPSServer();
  ~HTTPSServer();

  esp_err_t Inilialize();
  esp_err_t RegisterURIHandler(const httpd_uri_t* handler_info);
  esp_err_t UnregisterURIHandler(const char* uri, httpd_method_t method);

 private:
  httpd_handle_t server_;
};