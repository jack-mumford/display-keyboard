#pragma once

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

 private:
  lv_obj_t* lbl_test_ = nullptr;
  lv_obj_t* img_test_ = nullptr;
  WiFiStatus wifi_status_ = WiFiStatus::Offline;
};
