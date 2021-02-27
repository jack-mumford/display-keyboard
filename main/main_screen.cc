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
constexpr lv_coord_t kWiFiWidth = 16;
constexpr lv_coord_t kWiFiHeight = 16;
constexpr int kStatusBarHeight = 16;
}  // namespace

MainScreen::MainScreen(MainDisplay& display) : Screen(display) {}

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

  img_wifi_ = lv_img_create(screen, nullptr);
  if (img_wifi_) {
    constexpr char fname[] = "S:/spiffs/wifi-online.png";
    ESP_LOGI(TAG, "Loading image \"%s\".", fname);
    lv_img_set_src(img_wifi_, fname);
    // For some reason this image can't be put higher than y=40 - LVGL will
    // hang.
    lv_obj_set_pos(img_wifi_, kScreenWidth - kWiFiWidth, 40);
    lv_obj_set_size(img_wifi_, kWiFiWidth, kWiFiHeight);
  }

  return ESP_OK;
}

MainScreen::~MainScreen() = default;

void MainScreen::SetWiFiStatus(WiFiStatus status) {
  if (wifi_status_ == status)
    return;
  wifi_status_ = status;
}
