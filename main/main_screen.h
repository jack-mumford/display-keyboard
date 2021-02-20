#pragma once

#include <esp_err.h>
#include <lv_core/lv_obj.h>

#include "screen.h"

class MainScreen : public Screen {
 public:
  MainScreen(Display& display);
  ~MainScreen();

  esp_err_t Initialize();
  void Update() override;

 private:
  lv_obj_t* lbl_test_ = nullptr;
  lv_obj_t* img_test_ = nullptr;
};
