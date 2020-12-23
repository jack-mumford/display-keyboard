#include "spotify.h"

#include <cstring>
#include <memory>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_err.h>
#include <esp_log.h>
#include <mbedtls/base64.h>

#include "config.h"
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
constexpr char kRedirectURL[] = "http://displaykeyboard/local/callback";
constexpr char kUserAgent[] = "esp-idf/1.0 esp32";
constexpr char kRootURI[] = "/";
constexpr char kCallbackURI[] = "/callback/";

std::string Base64Encode(const std::string& str) {
  size_t dest_buff_size(0);

  // First get the necessary destination buffer size.
  mbedtls_base64_encode(nullptr, 0, &dest_buff_size,
                        reinterpret_cast<const unsigned char*>(str.c_str()),
                        str.length());

  std::unique_ptr<unsigned char> dst(new unsigned char[dest_buff_size]);
  if (!dst)
    return std::string();

  int res = mbedtls_base64_encode(
      dst.get(), dest_buff_size, &dest_buff_size,
      reinterpret_cast<const unsigned char*>(str.c_str()), str.length());
  if (res)
    return std::string();

  return std::string(reinterpret_cast<char*>(dst.get()), dest_buff_size);
}

std::string EntityEncode(const std::string& str) {
  // TODO: Surely there is an ESP-IDF/FreeRTOS function to do this.
  std::string encoded;
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

std::string GetQueryString(httpd_req_t* request, const char* key) {
  std::unique_ptr<char> query(new char[HTTPD_MAX_URI_LEN + 1]);
  if (httpd_req_get_url_query_str(request, query.get(), HTTPD_MAX_URI_LEN) !=
      ESP_OK) {
    return std::string();
  }
  std::unique_ptr<char> value(new char[HTTPD_MAX_URI_LEN + 1]);
  if (httpd_query_key_value(query.get(), "code", value.get(),
                            HTTPD_MAX_URI_LEN) != ESP_OK) {
    return std::string();
  }
  query.reset();
  return std::string(value.get());
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

Spotify::Spotify(const Config* config, HTTPSServer* https_server)
    : https_client_(kUserAgent), config_(config), https_server_(https_server) {
  assert(config != nullptr);
  assert(https_server != nullptr);
}

Spotify::~Spotify() {
  https_server_->UnregisterURIHandler(kRootURI, HTTP_GET);
  https_server_->UnregisterURIHandler(kCallbackURI, HTTP_GET);
};

esp_err_t Spotify::Initialize() {
  esp_err_t err = https_client_.DoSSLCheck();
  if (err != ESP_OK)
    return err;

  const httpd_uri_t root_handler_info{
      .uri = kRootURI,
      .method = HTTP_GET,
      .handler = Spotify::RootHandler,
      .user_ctx = this,
  };
  https_server_->RegisterURIHandler(&root_handler_info);

  const httpd_uri_t callback_handler_info{
      .uri = kCallbackURI,
      .method = HTTP_GET,
      .handler = Spotify::CallbackHandler,
      .user_ctx = this,
  };
  https_server_->RegisterURIHandler(&callback_handler_info);

  return ESP_OK;
}

esp_err_t Spotify::HandleRootRequest(httpd_req_t* request) {
  return ESP_OK;
}

esp_err_t Spotify::HandleCallbackRequest(httpd_req_t* request) {
  const std::string one_way_code = GetQueryString(request, "code");
  if (one_way_code.empty())
    return httpd_resp_send_500(request);
  ESP_LOGI(TAG, "One-way code \"%s\"", one_way_code.c_str());

  constexpr char kCallbackSuccess[] =
      "<html><head></head><body>Succesfully authentiated This device with "
      "Spotify. Restart your device now</body></html>";
  constexpr size_t kCallbackSuccessLen = sizeof(kCallbackSuccess) - 1;
  return httpd_resp_send(request, kCallbackSuccess, kCallbackSuccessLen);
}

esp_err_t Spotify::RequestAuthorization(AuthData* auth) {
  return ESP_OK;
}

esp_err_t Spotify::GetToken(AuthData* auth,
                            const std::string& grant_type,
                            std::string code) {
  std::vector<std::string> header_values = {
      "Authorization: Basic " + Base64Encode(config_->spotify.client_id + ":" +
                                             config_->spotify.client_secret),
      "Content-Type: application/x-www-form-urlencoded", "Connection: close"};

  const std::string code_param =
      grant_type == "refresh_token" ? "refresh_token" : "code";

  const std::string redurect_url = EntityEncode(kRedirectURL);
  const std::string content = "grant_type=" + grant_type + "&" + code_param +
                              "=" + code + "&redirect_uri=" + redurect_url;

  ESP_LOGD(TAG, "content: \"%s\"", content.c_str());
  esp_err_t err = https_client_.DoPOST("accounts.spotify.com", "/api/token",
                                       content, header_values);
  return err;
}