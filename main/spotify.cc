#include "spotify.h"

#include <cstring>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <esp_err.h>

// Needs to be included early.
#include <mbedtls/esp_config.h>

#include <esp_crt_bundle.h>
#include <esp_http_client.h>
#include <esp_log.h>
#include <esp_tls.h>

#include "config.h"

using std::string;

namespace {

struct AuthData {
  string access_token;
  string token_type;
  uint16_t expires_in;
  string refresh_token;
  string scope;
};

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

}  // namespace

Spotify::Spotify(const Config* config)
    : https_client_(kUserAgent), config_(config) {
  assert(config != nullptr);
}

Spotify::~Spotify() = default;

esp_err_t Spotify::DoSSLCheck() {
  return https_client_.DoRequest("www.howsmyssl.com", "/a/check",
                                 std::vector<std::string>());
}