#include "main_screen.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#include <lv_core/lv_disp.h>
#include <lv_widgets/lv_img.h>
#include <lv_widgets/lv_label.h>

#include "main_display.h"

namespace {
constexpr char TAG[] = "MainScreen";
constexpr lv_coord_t kScreenWidth = 320;
constexpr lv_coord_t kScreenHeight = 240;
constexpr lv_coord_t kStatusBarIconWidth = 20;
constexpr lv_coord_t kStatusBarIconHeight = 20;
constexpr int kStatusBarHeight = 20;
}  // namespace

MainScreen::MainScreen(MainDisplay& display) : Screen(display) {}

void MainScreen::UpdateTime() {
  struct tm now_local;
  {
    time_t now_epoch_coordinated_universal = 0;
    time(&now_epoch_coordinated_universal);
    localtime_r(&now_epoch_coordinated_universal, &now_local);
  }
  char tmbuf[32];
  strftime(tmbuf, sizeof(tmbuf), "%r", &now_local);
  lv_label_set_text(lbl_time_, tmbuf);
  last_time_ = tmbuf;
}

void MainScreen::UpdateWiFi() {
  lv_obj_set_hidden(img_wifi_online_, wifi_status_ != WiFiStatus::Online);
  lv_obj_set_hidden(img_wifi_offline_, wifi_status_ != WiFiStatus::Offline);
}

esp_err_t MainScreen::LoadWiFiImages() {
  lv_obj_t* screen = disp().screen();
  img_wifi_online_ = lv_img_create(screen, nullptr);
  if (!img_wifi_online_)
    return ESP_FAIL;
  lv_obj_set_pos(img_wifi_online_, kScreenWidth - kStatusBarIconWidth, 0);
  lv_obj_set_size(img_wifi_online_, kStatusBarIconWidth, kStatusBarIconHeight);
  lv_img_set_src(img_wifi_online_, "S:/spiffs/wifi-online.png");

  img_wifi_offline_ = lv_img_create(screen, nullptr);
  if (!img_wifi_offline_)
    return ESP_FAIL;
  lv_obj_set_pos(img_wifi_offline_, kScreenWidth - kStatusBarIconWidth, 0);
  lv_obj_set_size(img_wifi_offline_, kStatusBarIconWidth, kStatusBarIconHeight);
  lv_img_set_src(img_wifi_offline_, "S:/spiffs/wifi-offline.png");

  return ESP_OK;
}

esp_err_t MainScreen::InitializeStatusBar() {
  constexpr lv_coord_t kTimeWidth = 105;

  lv_obj_t* screen = disp().screen();

  lbl_time_ = lv_label_create(screen, nullptr);
  if (!lbl_time_)
    return ESP_FAIL;
  UpdateTime();
  lv_obj_set_pos(lbl_time_, kScreenWidth - kStatusBarIconWidth - kTimeWidth, 0);

  esp_err_t err = LoadWiFiImages();
  if (err != ESP_OK)
    return err;
  UpdateWiFi();

  img_gear_ = lv_img_create(screen, nullptr);
  if (!img_gear_)
    return ESP_FAIL;
  lv_img_set_src(img_gear_, "S:/spiffs/gear_icon.png");
  lv_obj_set_pos(img_gear_, 2, 0);
  lv_obj_set_size(img_gear_, kStatusBarIconWidth, kStatusBarIconHeight);

  img_spotify_ = lv_img_create(screen, nullptr);
  if (!img_spotify_)
    return ESP_FAIL;
  lv_img_set_src(img_spotify_, "S:/spiffs/spotify-logo.png");
  lv_obj_set_pos(img_spotify_, (kScreenWidth - kStatusBarIconWidth) / 2, 0);
  lv_obj_set_size(img_spotify_, kStatusBarIconWidth, kStatusBarIconHeight);

  return ESP_OK;
}

esp_err_t MainScreen::Initialize() {
  constexpr int kLineHeight = 18;
  constexpr int kMargin = 10;

  int top = kStatusBarHeight;

  lv_obj_t* screen = disp().screen();

  lbl_test_ = lv_label_create(screen, nullptr);
  lv_label_set_text(lbl_test_, "Artist: <Name of artist>");
  lv_obj_set_pos(lbl_test_, kMargin, top += kLineHeight);

  lv_obj_t* album = lv_label_create(screen, nullptr);
  lv_label_set_text(album, "Album: <Name of album>");
  lv_obj_set_pos(album, kMargin, top += kLineHeight);

  lv_obj_t* song = lv_label_create(screen, nullptr);
  lv_label_set_text(song, "Song: <Name of song>");
  lv_obj_set_pos(song, kMargin, top += kLineHeight);

  img_album_ = lv_img_create(screen, nullptr);
  if (img_album_) {
    constexpr char fname[] = "S:/spiffs/album_2_cover.jpg";
    ESP_LOGI(TAG, "Loading image \"%s\".", fname);
    constexpr int kImageWidth = kScreenWidth / 2;
    constexpr int kImageHeight = kScreenHeight / 2;
    constexpr int kImageLeft = (kScreenWidth - kImageWidth) / 2;
    lv_img_set_src(img_album_, fname);
    lv_obj_set_pos(img_album_, kImageLeft, top + 26);
    lv_obj_set_size(img_album_, kImageWidth, kImageHeight);
  }

  return InitializeStatusBar();
  ;
}

MainScreen::~MainScreen() = default;

void MainScreen::SetWiFiStatus(WiFiStatus status) {
  if (wifi_status_ == status)
    return;
  wifi_status_ = status;
  UpdateWiFi();
}
