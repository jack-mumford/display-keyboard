#pragma once

#include <cstdint>

#include <esp_err.h>

#include <class/hid/hid.h>
#include <class/hid/hid_device.h>
#include <device/usbd.h>
#include "usb_strings.h"

namespace usb {

class HID {
 public:
  constexpr static uint8_t kEndpointInput = 0x80;
  constexpr static uint8_t kBootProtocol = HID_PROTOCOL_NONE;
  constexpr static uint8_t kEndpointIntervalMs = 2;
  constexpr static char kStringDescriptor[] = "Test Keyboard";
  constexpr static uint8_t desc_hid_report[] = {
      TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_PROTOCOL_KEYBOARD))};
  constexpr static uint8_t kHIDDescriptor[] = {
      TUD_HID_DESCRIPTOR(1,
                         STRID_HID,
                         kBootProtocol,
                         sizeof(desc_hid_report),
                         kEndpointInput,
                         CFG_TUD_HID_EP_BUFSIZE,
                         kEndpointIntervalMs)};

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
};

}  // namespace usb