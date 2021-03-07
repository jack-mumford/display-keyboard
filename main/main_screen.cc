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
constexpr lv_coord_t kSpotifyIconWidth = 66;
constexpr lv_coord_t kSpotifyIconHeight = 20;
constexpr int kStatusBarHeight = 20;
constexpr lv_coord_t kRatingImageWidth = 38;
constexpr lv_coord_t kRatingImageHeight = 42;
constexpr lv_coord_t kAlbumArtworkWidth = 130;
constexpr lv_coord_t kAlbumArtworkHeight = 130;
constexpr lv_coord_t kAlbumArtworkLeft =
    (kScreenWidth - kAlbumArtworkWidth) / 2;
constexpr lv_coord_t kAlbumArtworkTop =
    kScreenHeight - kAlbumArtworkHeight - 20;
}  // namespace

MainScreen::MainScreen(MainDisplay& display) : Screen(display) {}

void MainScreen::UpdateTime() {
  if (!lbl_time_)
    return;
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
  if (img_wifi_online_)
    lv_obj_set_hidden(img_wifi_online_, wifi_status_ != WiFiStatus::Online);
  if (img_wifi_offline_)
    lv_obj_set_hidden(img_wifi_offline_, wifi_status_ != WiFiStatus::Offline);
}

void MainScreen::UpdateRating() {
  if (img_thumbs_up_)
    lv_obj_set_hidden(img_thumbs_up_, true);
  if (img_thumbs_none_)
    lv_obj_set_hidden(img_thumbs_none_, false);
}

esp_err_t MainScreen::LoadWiFiImages() {
  lv_obj_t* screen = disp().lv_screen();
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

esp_err_t MainScreen::LoadRatingImages() {
  lv_obj_t* screen = disp().lv_screen();

  constexpr lv_coord_t kImageTop =
      kAlbumArtworkTop + (kAlbumArtworkHeight - kRatingImageHeight) / 2;
  constexpr lv_coord_t kImageLeft = kScreenWidth - kRatingImageWidth - 30;

  img_thumbs_up_ = lv_img_create(screen, nullptr);
  if (!img_thumbs_up_)
    return ESP_FAIL;
  lv_obj_set_pos(img_thumbs_up_, kImageLeft, kImageTop);
  lv_obj_set_size(img_thumbs_up_, kRatingImageWidth, kRatingImageHeight);
  lv_img_set_src(img_thumbs_up_, "S:/spiffs/thumbs-up.png");

  img_thumbs_none_ = lv_img_create(screen, nullptr);
  if (!img_thumbs_none_)
    return ESP_FAIL;
  lv_obj_set_pos(img_thumbs_none_, kImageLeft, kImageTop);
  lv_obj_set_size(img_thumbs_none_, kRatingImageWidth, kRatingImageHeight);
  lv_img_set_src(img_thumbs_none_, "S:/spiffs/thumbs-up-none.png");

  return ESP_OK;
}

esp_err_t MainScreen::LoadGearImage() {
  img_gear_ = lv_img_create(disp().lv_screen(), nullptr);
  if (!img_gear_)
    return ESP_FAIL;
  lv_img_set_src(img_gear_, "S:/spiffs/gear_icon.png");
  lv_obj_set_pos(img_gear_, 2, 0);
  lv_obj_set_size(img_gear_, kStatusBarIconWidth, kStatusBarIconHeight);
  return ESP_OK;
}

esp_err_t MainScreen::LoadSpotifyImage() {
  img_spotify_ = lv_img_create(disp().lv_screen(), nullptr);
  if (!img_spotify_)
    return ESP_FAIL;
  lv_img_set_src(img_spotify_, "S:/spiffs/spotify-norm.png");
  lv_obj_set_pos(img_spotify_, (kScreenWidth - kSpotifyIconWidth) / 2 - 10, 0);
  lv_obj_set_size(img_spotify_, kSpotifyIconWidth, kSpotifyIconHeight);
  return ESP_OK;
}

esp_err_t MainScreen::CreateTimeLabel() {
  constexpr lv_coord_t kTimeWidth = 105;

  lbl_time_ = lv_label_create(disp().lv_screen(), nullptr);
  if (!lbl_time_)
    return ESP_FAIL;
  lv_obj_set_pos(lbl_time_, kScreenWidth - kStatusBarIconWidth - kTimeWidth, 2);
  return ESP_OK;
}

esp_err_t MainScreen::InitializeStatusBar() {
  esp_err_t err;

  err = LoadGearImage();
  if (err != ESP_OK)
    return err;

  err = LoadSpotifyImage();
  if (err != ESP_OK)
    return err;

  err = LoadWiFiImages();
  if (err != ESP_OK)
    return err;
  UpdateWiFi();

  err = CreateTimeLabel();
  if (err != ESP_OK)
    return err;
  UpdateTime();

  return ESP_OK;
}

esp_err_t MainScreen::CreateSongDataLabels() {
  constexpr int kLineHeight = 18;
  constexpr int kMargin = 10;

  lv_coord_t top = kStatusBarHeight - 12;
  lv_obj_t* screen = disp().lv_screen();

  lbl_test_ = lv_label_create(screen, nullptr);
  if (!lbl_test_)
    return ESP_FAIL;
  lv_label_set_text(lbl_test_, "Artist: <Name of artist>");
  lv_obj_set_pos(lbl_test_, kMargin, top += kLineHeight);

  lv_obj_t* album = lv_label_create(screen, nullptr);
  if (!album)
    return ESP_FAIL;
  lv_label_set_text(album, "Album: <Name of album>");
  lv_obj_set_pos(album, kMargin, top += kLineHeight);

  lv_obj_t* song = lv_label_create(screen, nullptr);
  if (!song)
    return ESP_FAIL;
  lv_label_set_text(song, "Song: <Name of song>");
  lv_obj_set_pos(song, kMargin, top += kLineHeight);
  return ESP_OK;
}

void MainScreen::SetAlbumArtwork(std::string image_src) {
  // We could handle empty in the future, but don't at present.
  configASSERT(!image_src.empty());
  album_cover_img_src_ = std::move(image_src);
  if (!img_album_)
    return;
  // This doesn't work (at least for JPEG images).
  // lv_img_set_src(img_album_, album_cover_img_src_.c_str());
}

esp_err_t MainScreen::CreateAlbumArtwork() {
  img_album_ = lv_img_create(disp().lv_screen(), nullptr);
  if (!img_album_)
    return ESP_FAIL;

  constexpr char fname[] = "S:/spiffs/album_2_cover.jpg";
  ESP_LOGI(TAG, "Loading image \"%s\".", fname);
  lv_img_set_src(img_album_, fname);
  lv_obj_set_pos(img_album_, kAlbumArtworkLeft, kAlbumArtworkTop);
  lv_obj_set_size(img_album_, kAlbumArtworkWidth, kAlbumArtworkHeight);
  // lv_img_set_zoom(img_album_, /*50%=*/128);
  return ESP_OK;
}

esp_err_t MainScreen::Initialize() {
  esp_err_t err;

  err = InitializeStatusBar();
  if (err != ESP_OK)
    return err;

  err = CreateSongDataLabels();
  if (err != ESP_OK)
    return err;

  err = CreateAlbumArtwork();
  if (err != ESP_OK)
    return err;

  err = LoadRatingImages();
  if (err != ESP_OK)
    return err;
  UpdateRating();

  return ESP_OK;
}

MainScreen::~MainScreen() = default;

void MainScreen::SetWiFiStatus(WiFiStatus status) {
  if (wifi_status_ == status)
    return;
  wifi_status_ = status;
  UpdateWiFi();
}
