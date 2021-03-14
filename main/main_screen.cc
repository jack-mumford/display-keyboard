#include "main_screen.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#include <lv_core/lv_disp.h>
#include <lv_widgets/lv_img.h>
#include <lv_widgets/lv_label.h>

#include "images/gear.h"
#include "images/generic_cover.h"
#include "images/rating_none.h"
#include "images/rating_positive.h"
#include "images/spotify.h"
#include "images/wifi_offline.h"
#include "images/wifi_online.h"
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
constexpr lv_coord_t kAlbumArtworkTop = 20;
// kScreenHeight - kAlbumArtworkHeight - 20;

#ifdef DISPLAY_MEMORY
std::string DisplayMem(size_t bytes) {
  char buff[20];

  if (bytes > 1000000) {
    snprintf(buff, sizeof(buff), "%0.1f MB",
             static_cast<float>(bytes) / 1024.0f / 1024.0f);
  } else if (bytes > 10000) {
    snprintf(buff, sizeof(buff), "%zu KB", bytes / 1024);
  } else {
    snprintf(buff, sizeof(buff), "%zu B", bytes);
  }

  buff[sizeof(buff) - 1] = '\0';
  return std::string(buff);
}
#endif
}  // namespace

MainScreen::MainScreen(MainDisplay& display) : Screen(display) {
  bzero(&album_cover_image_, sizeof(album_cover_image_));
}

void MainScreen::UpdateTime() {
  char tmbuf[40];

#ifdef DISPLAY_MEMORY
  if (lbl_memory_spiram_) {
    snprintf(
        tmbuf, sizeof(tmbuf), "SPIRAM: %s, min:%s (%s)",
        DisplayMem(heap_caps_get_free_size(MALLOC_CAP_SPIRAM)).c_str(),
        DisplayMem(heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM)).c_str(),
        DisplayMem(heap_caps_get_total_size(MALLOC_CAP_SPIRAM)).c_str());
    lv_label_set_text(lbl_memory_spiram_, tmbuf);
  }
  if (lbl_memory_internal_) {
    snprintf(tmbuf, sizeof(tmbuf), "INT: %s, min:%s (%s)",
             DisplayMem(heap_caps_get_free_size(MALLOC_CAP_INTERNAL)).c_str(),
             DisplayMem(heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL))
                 .c_str(),
             DisplayMem(heap_caps_get_total_size(MALLOC_CAP_INTERNAL)).c_str());
    lv_label_set_text(lbl_memory_internal_, tmbuf);
  }
#endif
  if (!lbl_time_)
    return;
  struct tm now_local;
  {
    time_t now_epoch_coordinated_universal = 0;
    time(&now_epoch_coordinated_universal);
    localtime_r(&now_epoch_coordinated_universal, &now_local);
  }
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
  lv_img_set_src(img_wifi_online_, &wifi_online);

  img_wifi_offline_ = lv_img_create(screen, nullptr);
  if (!img_wifi_offline_)
    return ESP_FAIL;
  lv_obj_set_pos(img_wifi_offline_, kScreenWidth - kStatusBarIconWidth, 0);
  lv_obj_set_size(img_wifi_offline_, kStatusBarIconWidth, kStatusBarIconHeight);
  lv_img_set_src(img_wifi_offline_, &wifi_offline);

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
  lv_img_set_src(img_thumbs_up_, &rating_positive);

  img_thumbs_none_ = lv_img_create(screen, nullptr);
  if (!img_thumbs_none_)
    return ESP_FAIL;
  lv_obj_set_pos(img_thumbs_none_, kImageLeft, kImageTop);
  lv_obj_set_size(img_thumbs_none_, kRatingImageWidth, kRatingImageHeight);
  lv_img_set_src(img_thumbs_none_, &rating_none);

  return ESP_OK;
}

esp_err_t MainScreen::LoadGearImage() {
  img_gear_ = lv_img_create(disp().lv_screen(), nullptr);
  if (!img_gear_)
    return ESP_FAIL;
  lv_img_set_src(img_gear_, &gear);
  lv_obj_set_pos(img_gear_, 2, 0);
  lv_obj_set_size(img_gear_, kStatusBarIconWidth, kStatusBarIconHeight);
  return ESP_OK;
}

esp_err_t MainScreen::LoadSpotifyImage() {
  img_spotify_ = lv_img_create(disp().lv_screen(), nullptr);
  if (!img_spotify_)
    return ESP_FAIL;
  lv_img_set_src(img_spotify_, &spotify);
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

  constexpr lv_coord_t kLineHeight = 15;
  constexpr lv_coord_t kBottomMargin = 2;

#ifdef DEBUG_STRING
  lbl_debug_msg_ = lv_label_create(disp().lv_screen(), nullptr);
  if (!lbl_debug_msg_)
    return ESP_FAIL;
  lv_label_set_text(lbl_debug_msg_, "");
  lv_obj_set_pos(lbl_debug_msg_, 0,
                 kScreenHeight - 3 * kLineHeight - kBottomMargin);
#endif

#ifdef DISPLAY_MEMORY
  lbl_memory_spiram_ = lv_label_create(disp().lv_screen(), nullptr);
  if (!lbl_memory_spiram_)
    return ESP_FAIL;
  lv_obj_set_pos(lbl_memory_spiram_, 0,
                 kScreenHeight - 2 * kLineHeight - kBottomMargin);
  lbl_memory_internal_ = lv_label_create(disp().lv_screen(), nullptr);
  if (!lbl_memory_internal_)
    return ESP_FAIL;
  lv_obj_set_pos(lbl_memory_internal_, 0,
                 kScreenHeight - 1 * kLineHeight - kBottomMargin);
#endif

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

#ifdef DEBUG_STRING
void MainScreen::SetDebugString(const char* str) {
  lv_label_set_text(lbl_debug_msg_, str);
}
#endif

void MainScreen::SetAlbumArtwork(lv_img_dsc_t image) {
  if (!img_album_)
    return;
  delete[] album_cover_image_.data;
  album_cover_image_ = image;
  lv_img_set_src(img_album_, &album_cover_image_);
  lv_obj_set_size(img_album_, album_cover_image_.header.w,
                  album_cover_image_.header.h);
}

esp_err_t MainScreen::CreateAlbumArtwork() {
  img_album_ = lv_img_create(disp().lv_screen(), nullptr);
  if (!img_album_)
    return ESP_FAIL;

  lv_img_set_src(img_album_, &generic_cover);
  lv_obj_set_pos(img_album_, kAlbumArtworkLeft, kAlbumArtworkTop);
  lv_obj_set_size(img_album_, kAlbumArtworkWidth, kAlbumArtworkHeight);
  return ESP_OK;
}

esp_err_t MainScreen::Initialize() {
  esp_err_t err;

  err = CreateAlbumArtwork();
  if (err != ESP_OK)
    return err;

  err = InitializeStatusBar();
  if (err != ESP_OK)
    return err;

  err = CreateSongDataLabels();
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
