#pragma once

#include <lv_core/lv_obj.h>

#include "screen.h"

class MainScreen : public Screen {
 public:
  MainScreen();
  ~MainScreen();

  void Update() override;

 private:
  lv_obj_t* lbl_test_ = nullptr;
};
