#pragma once

#include <cstdint>
#include <string>

#include <esp_err.h>
#include <esp_http_server.h>
#include <event_groups.h>

#include "https_client.h"

class Config;
class HTTPSServer;
class WiFi;

class Spotify {
 public:
  Spotify(const Config* config,
          HTTPSServer* https_server,
          WiFi* wifi,
          EventGroupHandle_t event_group);
  ~Spotify();

  esp_err_t Initialize();
  esp_err_t RequestAuthToken();

  // Was this instance *successfully* initialized?
  bool initialized() const { return initialized_; }

  // The the URL (hosted by this device) to start the Spotify authentication
  // process.
  std::string GetAuthStartURL() const;

 private:
  struct AuthData {
    std::string access_token;
    std::string token_type;
    uint16_t expires_in;
    std::string refresh_token;
    std::string scope;
    std::string one_time_code;
  };

  static esp_err_t RootHandler(httpd_req_t* request);
  static esp_err_t CallbackHandler(httpd_req_t* request);

  /**
   * HTTPD request handler for "/".
   *
   * @param request
   * @return ESP_OK if successful.
   */
  esp_err_t HandleRootRequest(httpd_req_t* request);

  /**
   * HTTPD request handler for "/callback/".
   *
   * @param request
   * @return ESP_OK if successful.
   */
  esp_err_t HandleCallbackRequest(httpd_req_t* request);

  /**
   * Get or renew the Spotify API access token.
   *
   * @param grant_type Either "refresh_token" or "authorization_code". The
   * latter is used as part of the user authorization flow.
   */
  esp_err_t GetAccessToken(const std::string& grant_type,
                           const std::string& code);

  /**
   * Create the URL to have Spotify redirect to after user successfully
   * authenticates and approves access to this client.
   *
   * @param url Location to write redirect url.
   * @return ESP_OK if successful.
   */
  esp_err_t GetRedirectURL(std::string* url) const;

  /**
   * Retrieve the devices host name or IP address.
   *
   * @param host Location to receive host or IP.
   *
   * @return ESP_OK if successful.
   */
  esp_err_t GetHostname(std::string* host) const;

  HTTPSClient https_client_;
  AuthData auth_data_;
  const Config* config_;  // Application config data.
  HTTPSServer* https_server_;
  EventGroupHandle_t event_group_;
  WiFi* wifi_;
  bool initialized_;
};