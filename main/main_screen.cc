#include <lv_core/lv_disp.h>
#include <lv_widgets/lv_label.h>

#include "main_screen.h"

MainScreen::MainScreen() {
  lv_obj_t* scr = lv_disp_get_scr_act(nullptr);

  lbl_test_ = lv_label_create(scr, nullptr);
  lv_label_set_text(lbl_test_, "Hello World");
  lv_obj_set_pos(lbl_test_, 0, 0);
}

MainScreen::~MainScreen() = default;

void MainScreen::Update() {}
