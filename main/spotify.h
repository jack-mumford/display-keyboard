#pragma once

#include <cstdint>
#include <string>

#include <esp_http_server.h>
#include <esp_timer.h>
#include <event_groups.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class Config;
class HTTPServer;
class WiFi;

class Spotify {
 public:
  Spotify(const Config* config,
          HTTPServer* https_server,
          WiFi* wifi,
          EventGroupHandle_t event_group);
  ~Spotify();

  /**
   * Initialize this instance.
   *
   * Must be done before using any other method.
   */
  esp_err_t Initialize();

  /**
   * Request the access token.
   *
   * Call this after the one-time code has been retrieved.
   */
  esp_err_t RequestAccessToken();

  /**
   * Retrieve the Spotify currently playing track information.
   */
  esp_err_t GetCurrentlyPlaying();

  // Was this instance *successfully* initialized?
  bool initialized() const { return initialized_; }

  // The the URL (hosted by this device) to start the Spotify authentication
  // process.
  std::string GetAuthStartURL() const;

  bool HaveOneTimeCode() const;

  bool HaveAccessToken() const;

 private:
  enum class TokenGrantType {
    Refresh,
    OneTime,
  };

  /**
   * Contains authentication values for the current Spotify user.
   */
  struct AuthData {
    std::string access_token;   // Access token used for all API calls.
    std::string token_type;     // Access token type. Always "Bearer".
    uint32_t expires_in_secs;   // Number of seconds until access_token expires.
    std::string refresh_token;  // The refresh token used to get access_token.
    std::string scope;          // Privilege scope.
    std::string one_time_code;  // Code used when fully authenticating.
  };

  static esp_err_t RootHandler(httpd_req_t* request);
  static esp_err_t CallbackHandler(httpd_req_t* request);
  static void TokenRenewCb(void* arg);

  /**
   * HTTPD request handler for "/".
   *
   * @param request
   * @return ESP_OK if successful.
   */
  esp_err_t HandleRootRequest(httpd_req_t* request);

  /**
   * HTTPD request handler for "/callback/" resource.
   *
   * @param request
   * @return ESP_OK if successful.
   */
  esp_err_t HandleCallbackRequest(httpd_req_t* request);

  /**
   * Get or renew the Spotify API access token.
   *
   * @note Can be called on any task.
   *
   * @param grant_type The type of code grant being renewed.
   * @param code The code (one-time or renew) used.
   */
  esp_err_t GetAccessToken(TokenGrantType grant_type, std::string code);

  /**
   * Renew the access token.
   *
   * Called on another task.
   */
  esp_err_t RenewAccessToken();

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

  const Config* config_;            // Application config data.
  HTTPServer* https_server_;        // Accept inbound requests for auth.
  EventGroupHandle_t event_group_;  // Used to inform owner of events.
  WiFi* wifi_;                      // Object used to controll Wi-Fi network.
  bool initialized_;                // Is this instance initialized?
  esp_timer_handle_t token_renew_timer_;  // Used to renew timer.
  SemaphoreHandle_t mutex_;  // Synchronize access to following members.
  AuthData auth_data_;       // Current user auth data.
};
