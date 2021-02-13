#pragma once

#include <esp_err.h>
#include <esp_http_server.h>

class HTTPServer {
 public:
  HTTPServer();
  ~HTTPServer();

  esp_err_t Initialize();
  esp_err_t RegisterURIHandler(const httpd_uri_t* handler_info);
  esp_err_t UnregisterURIHandler(const char* uri, httpd_method_t method);

 private:
  httpd_handle_t server_;  // ESP-IDF server object.
};
