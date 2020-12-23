#pragma once

#include <cstdint>
#include <string>

#include <esp_err.h>
#include <esp_http_server.h>

#include <https_client.h>

class Config;
class HTTPSServer;

class Spotify {
 public:
  Spotify(const Config* config, HTTPSServer* https_server);
  ~Spotify();

  esp_err_t Initialize();

 private:
  struct AuthData {
    std::string access_token;
    std::string token_type;
    uint16_t expires_in;
    std::string refresh_token;
    std::string scope;
  };

  static esp_err_t RootHandler(httpd_req_t* request);
  static esp_err_t CallbackHandler(httpd_req_t* request);

  esp_err_t HandleRootRequest(httpd_req_t* request);
  esp_err_t HandleCallbackRequest(httpd_req_t* request);

  esp_err_t GetToken(AuthData* auth,
                     const std::string& grant_type,
                     std::string code);
  esp_err_t RequestAuthorization(AuthData* auth);

  HTTPSClient https_client_;
  AuthData auth_data_;
  const Config* config_;  // Application config data.
  HTTPSServer* https_server_;
};