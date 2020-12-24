#include "spotify.h"

#include <cstring>
#include <memory>
#include <string>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_err.h>
#include <esp_log.h>
#include <mbedtls/base64.h>

#include "config.h"
#include "event_ids.h"
#include "https_server.h"

using std::string;

namespace {

struct RequestData {
  struct {
    uint32_t progress_ms;
    uint32_t duration_ms;
  } times;
  bool is_playing;
  bool is_player_active;
  string artist_name;
  string song_title;
  struct {
    string url_640;
    string url_300;
    string url_64;
  } image;
};

constexpr char TAG[] = "kbd_spotify";
constexpr char kApiHost[] = "api.spotify.com";
constexpr char kCurrentlyPlayingResource[] = "/v1/me/player/currently-playing";
constexpr char kUserAgent[] = "esp-idf/1.0 esp32";
constexpr char kRootURI[] = "/";
constexpr char kCallbackURI[] = "/callback/";

string Base64Encode(const string& str) {
  size_t dest_buff_size(0);

  // First get the necessary destination buffer size.
  mbedtls_base64_encode(nullptr, 0, &dest_buff_size,
                        reinterpret_cast<const unsigned char*>(str.c_str()),
                        str.length());

  std::unique_ptr<unsigned char> dst(new unsigned char[dest_buff_size]);
  if (!dst)
    return string();

  int res = mbedtls_base64_encode(
      dst.get(), dest_buff_size, &dest_buff_size,
      reinterpret_cast<const unsigned char*>(str.c_str()), str.length());
  if (res)
    return string();

  return string(reinterpret_cast<char*>(dst.get()), dest_buff_size);
}

string EntityEncode(const string& str) {
  // TODO: Surely there is an ESP-IDF/FreeRTOS function to do this.
  string encoded;
  for (size_t i = 0; i < str.length(); i++) {
    char c = str[i];
    if (std::isalnum(c)) {
      encoded += c;
    } else {
      char code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9)
        code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      char code0 = c + '0';
      if (c > 9)
        code0 = c - 10 + 'A';
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
    taskYIELD();
  }
  return encoded;
}

string GetQueryString(httpd_req_t* request, const char* key) {
  std::unique_ptr<char> query(new char[HTTPD_MAX_URI_LEN + 1]);
  if (httpd_req_get_url_query_str(request, query.get(), HTTPD_MAX_URI_LEN) !=
      ESP_OK) {
    return string();
  }
  std::unique_ptr<char> value(new char[HTTPD_MAX_URI_LEN + 1]);
  if (httpd_query_key_value(query.get(), "code", value.get(),
                            HTTPD_MAX_URI_LEN) != ESP_OK) {
    return string();
  }
  query.reset();
  return string(value.get());
}

}  // namespace

// static
esp_err_t Spotify::RootHandler(httpd_req_t* request) {
  return static_cast<Spotify*>(request->user_ctx)->HandleRootRequest(request);
}

// static
esp_err_t Spotify::CallbackHandler(httpd_req_t* request) {
  return static_cast<Spotify*>(request->user_ctx)
      ->HandleCallbackRequest(request);
}

Spotify::Spotify(const Config* config,
                 HTTPSServer* https_server,
                 EventGroupHandle_t event_group)
    : https_client_(kUserAgent),
      config_(config),
      https_server_(https_server),
      event_group_(event_group),
      initialized_(false) {
  assert(config != nullptr);
  assert(https_server != nullptr);
}

Spotify::~Spotify() {
  ESP_ERROR_CHECK_WITHOUT_ABORT(
      https_server_->UnregisterURIHandler(kCallbackURI, HTTP_GET));
  ESP_ERROR_CHECK_WITHOUT_ABORT(
      https_server_->UnregisterURIHandler(kRootURI, HTTP_GET));
};

esp_err_t Spotify::Initialize() {
  const httpd_uri_t root_handler_info{
      .uri = kRootURI,
      .method = HTTP_GET,
      .handler = Spotify::RootHandler,
      .user_ctx = this,
  };
  esp_err_t err = https_server_->RegisterURIHandler(&root_handler_info);
  if (err != ESP_OK)
    return err;
  const httpd_uri_t callback_handler_info{
      .uri = kCallbackURI,
      .method = HTTP_GET,
      .handler = Spotify::CallbackHandler,
      .user_ctx = this,
  };
  err = https_server_->RegisterURIHandler(&callback_handler_info);
  if (err != ESP_OK)
    return err;

  initialized_ = true;
  return ESP_OK;
}

// Redirect the user agent to Spotify to authenticate. After successful
// authentication Spotify will redirect the user agent to a second URL
// that will contain our one-way code.
esp_err_t Spotify::HandleRootRequest(httpd_req_t* request) {
  ESP_LOGD(TAG, "In HandleRootRequest");
  httpd_resp_set_status(request, "302 Found");
  httpd_resp_set_type(request, "text/plain");

  constexpr char kScopes[] =
      "user-read-private%20"
      "user-read-currently-playing%20"
      "user-read-playback-state%20"
      "user-modify-playback-state";

  const string location = "https://accounts.spotify.com/authorize/?client_id=" +
                          config_->spotify.client_id + "&response_type=code" +
                          "&redirect_uri=" + GetRedirectURL() +
                          "&scope=" + kScopes;

  httpd_resp_set_hdr(request, "Location", location.c_str());

  constexpr char kRedirectText[] =
      "<html><head></head><body>Please authenticate on Spotify.</body></html>";
  return httpd_send(request, kRedirectText, sizeof(kRedirectText) - 1);
}

// The handler for the second part of the user authenticaton where
// Spotify delivers the one-way code.
esp_err_t Spotify::HandleCallbackRequest(httpd_req_t* request) {
  ESP_LOGD(TAG, "In HandleCallbackRequest");
  auth_data_.one_way_code = GetQueryString(request, "code");
  if (auth_data_.one_way_code.empty())
    return httpd_resp_send_500(request);
  ESP_LOGD(TAG, "One-way code \"%s\"", auth_data_.one_way_code.c_str());

  // Now that we have the one-way code, notify the application so that
  // it can update any UI (if desired) and continue the process of connecting
  // to Spotify.
  xEventGroupSetBits(event_group_, EVENT_SPOTIFY_GOT_ONE_WAY_CODE);

  constexpr char kCallbackSuccess[] =
      "<html><head></head><body>Succesfully authentiated this device with "
      "Spotify.</body></html>";
  constexpr size_t kCallbackSuccessLen = sizeof(kCallbackSuccess) - 1;
  return httpd_resp_send(request, kCallbackSuccess, kCallbackSuccessLen);
}

esp_err_t Spotify::RequestAuthorization(AuthData* auth) {
  return ESP_OK;
}

string Spotify::GetRedirectURL() const {
  const std::string hostname = "10.0.9.30";
  return "http://" + hostname + kCallbackURI;
}

esp_err_t Spotify::RequestAuthToken() {
  return GetToken("authorization_code", std::move(auth_data_.one_way_code));
}

esp_err_t Spotify::GetToken(const string& grant_type, const string& code) {
  if (grant_type != "authorization_code" && grant_type != "refresh_token")
    return ESP_ERR_INVALID_ARG;
  const std::vector<string> header_values = {
      "Authorization: Basic " + Base64Encode(config_->spotify.client_id + ":" +
                                             config_->spotify.client_secret),
      "Content-Type: application/x-www-form-urlencoded", "Connection: close"};

  const string code_param =
      grant_type == "refresh_token" ? "refresh_token" : "code";

  const string content = "grant_type=" + grant_type + "&" + code_param + "=" +
                         code +
                         "&redirect_uri=" + EntityEncode(GetRedirectURL());

  ESP_LOGD(TAG, "content: \"%s\"", content.c_str());
  esp_err_t err = https_client_.DoPOST("accounts.spotify.com", "/api/token",
                                       content, header_values);
  return err;
}