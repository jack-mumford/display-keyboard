
#pragma once

#include <esp_err.h>

#include <Adafruit_USBD_Device.h>
#include <class/hid/hid.h>
#include <class/hid/hid_device.h>

namespace usb {

class HID : public Adafruit_USBD_Interface {
 public:
  HID();
  ~HID();

  /**
   * Initialize the USB instance.
   */
  esp_err_t Initialize();

  /**
   * Standard HID Boot Protocol Keyboard Report.
   *
   * @param report_id // Can be zero.
   * @param modifier  // Keyboard modifier (KEYBOARD_MODIFIER_* masks).
   * @param keycode   // Key codes of the currently pressed keys.
   *
   * @return ESP_OK on success, else other error code.
   */
  esp_err_t KeyboardReport(uint8_t report_id,
                           uint8_t modifier,
                           uint8_t keycode[6]);

  esp_err_t KeyboardPress(uint8_t report_id, char ch);

  esp_err_t KeyboardRelease(uint8_t report_id);

  bool Ready();

  // Adafruit_USBD_Interface:
  uint16_t getDescriptor(uint8_t itfnum,
                         uint8_t* buf,
                         uint16_t bufsize) override;
};

}  // namespace usb