#pragma once

#include <string>

#include <esp_err.h>

#include <https_client.h>

class Config;

class Spotify {
 public:
  Spotify(const Config* config);
  ~Spotify();

  esp_err_t DoSSLCheck();

 private:
  std::string CreateCurrentlyPlayingRequest() const;

  HTTPSClient https_client_;
  const Config* config_;  // Application config data.
};