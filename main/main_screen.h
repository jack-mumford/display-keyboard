#pragma once

#include <string>

#include <esp_err.h>
#include <lv_core/lv_obj.h>

#include "event_ids.h"
#include "screen.h"

class MainScreen : public Screen {
 public:
  MainScreen(MainDisplay& display);
  ~MainScreen();

  esp_err_t Initialize();
  void SetWiFiStatus(WiFiStatus status);
  void UpdateTime();

 private:
  esp_err_t InitializeStatusBar();
  void UpdateWiFi();
  esp_err_t LoadWiFiImages();
  esp_err_t LoadGearImage();
  esp_err_t LoadSpotifyImage();
  esp_err_t CreateTimeLabel();
  esp_err_t CreateSongDataLabels();
  esp_err_t CreateAlbumArtwork();
  void UpdateRating();
  esp_err_t LoadRatingImages();

  lv_obj_t* lbl_test_ = nullptr;
  lv_obj_t* lbl_time_ = nullptr;
  lv_obj_t* img_album_ = nullptr;
  lv_obj_t* img_wifi_online_ = nullptr;
  lv_obj_t* img_wifi_offline_ = nullptr;
  lv_obj_t* img_spotify_ = nullptr;
  lv_obj_t* img_gear_ = nullptr;
  lv_obj_t* img_thumbs_none_ = nullptr;
  lv_obj_t* img_thumbs_up_ = nullptr;
  std::string last_time_;
  WiFiStatus wifi_status_ = WiFiStatus::Offline;
};
