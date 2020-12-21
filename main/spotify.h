#pragma once

#include <esp_err.h>

class Spotify {
 public:
  Spotify();
  ~Spotify();

  esp_err_t DoTest();
};