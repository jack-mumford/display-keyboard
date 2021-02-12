
#include <esp_log.h>
#include <lv_core/lv_disp.h>
#include <lv_widgets/lv_img.h>
#include <lv_widgets/lv_label.h>

#include "display.h"
#include "main_screen.h"

namespace {
constexpr char TAG[] = "kbd_screen";
}

MainScreen::MainScreen(Display& display) : Screen(display) {
  lbl_test_ = lv_label_create(display.screen(), nullptr);
  lv_label_set_text(lbl_test_, "Hello World");
  lv_obj_set_pos(lbl_test_, 0, 0);

  img_test_ = lv_img_create(display.screen(), nullptr);
  if (img_test_) {
    constexpr char fname[] = "S:/spiffs/album_2_cover.jpg";
    ESP_LOGI(TAG, "Loading image \"%s\".", fname);
    lv_img_set_src(img_test_, fname);
    lv_obj_set_pos(img_test_, 20, 0);
  }
}

MainScreen::~MainScreen() = default;

void MainScreen::Update() {}
