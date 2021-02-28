#pragma once

#include <string>

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
  void UpdateTime();

 private:
  esp_err_t InitializeStatusBar();

  lv_obj_t* lbl_test_ = nullptr;
  lv_obj_t* lbl_time_ = nullptr;
  lv_obj_t* img_album_ = nullptr;
  lv_obj_t* img_wifi_ = nullptr;
  std::string last_time_;
  WiFiStatus wifi_status_ = WiFiStatus::Offline;
};
