
#include <esp_log.h>
#include <lv_core/lv_disp.h>
#include <lv_widgets/lv_img.h>
#include <lv_widgets/lv_label.h>

#include "display.h"
#include "main_screen.h"

namespace {
constexpr char TAG[] = "kbd_screen";
}

MainScreen::MainScreen(Display& display) : Screen(display) {}

esp_err_t MainScreen::Initialize() {
  constexpr int kStatusBarHeight = 13;
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

  img_test_ = lv_img_create(screen, nullptr);
  if (img_test_) {
    constexpr char fname[] = "S:/spiffs/album_2_cover.jpg";
    ESP_LOGI(TAG, "Loading image \"%s\".", fname);
    constexpr int kImageWidth = 320 / 2;
    constexpr int kImageHeight = 240 / 2;
    constexpr int kImageLeft = (320 - kImageWidth) / 2;
    lv_img_set_src(img_test_, fname);
    lv_obj_set_pos(img_test_, kImageLeft, top + 26);
    lv_obj_set_size(img_test_, kImageWidth, kImageHeight);
  }

  return ESP_OK;
}

MainScreen::~MainScreen() = default;

void MainScreen::Update() {}

void MainScreen::SetWiFiStatus(WiFiStatus status) {
  if (wifi_status_ == status)
    return;
  wifi_status_ = status;
}
