
#pragma once

#include <Adafruit_USBD_HID.h>
#include <esp_err.h>

class USB {
 public:
  USB();
  ~USB();

  static bool Mounted();

  /**
   * Initialize the USB instance.
   */
  esp_err_t Initialize();

  static esp_err_t RemoteWakup();

  static bool Suspended();

  /**
   * Standard HID Boot Protocol Keyboard Report.
   *
   * @param report_id ///< Can be zero.
   * @param modifier  ///< Keyboard modifier (KEYBOARD_MODIFIER_* masks).
   * @param keycode   ///< Key codes of the currently pressed keys.
   *
   * @return esp_err_t
   */
  esp_err_t KeyboardReport(uint8_t report_id,
                           uint8_t modifier,
                           uint8_t keycode[6]);

  esp_err_t KeyboardRelease(uint8_t report_id);

  bool Ready();

 private:
  Adafruit_USBD_HID usb_hid_;
};
