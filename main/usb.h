
#pragma once

#include <Adafruit_USBD_HID.h>
#include <esp_err.h>

class USB {
 public:
  USB();
  ~USB();

  esp_err_t Initialize();

 private:
  Adafruit_USBD_HID usb_hid;
};
