#pragma once

#include <cstdint>

#include <esp_err.h>

#include <class/hid/hid.h>
#include <class/hid/hid_device.h>
#include <device/usbd.h>
#include "usb_string_ids.h"

namespace usb {

class HID {
 private:
  constexpr static uint8_t kBootProtocol = HID_PROTOCOL_NONE;
  constexpr static uint8_t kEndpointAddress = TUSB_DIR_IN_MASK + 1;
  constexpr static uint8_t kEndpointIntervalMs = 2;
  constexpr static uint8_t kInterfaceNumber = 0;  // IF #'s are zero based.

 public:
  constexpr static char kInterfaceName[] = "Keyboard HID";
  constexpr static uint8_t kHIDDescriptorReport[] = {
      TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_PROTOCOL_KEYBOARD))};
  constexpr static uint8_t kHIDDescriptorConfig[] = {
      TUD_HID_DESCRIPTOR(kInterfaceNumber,
                         STRID_HID,
                         kBootProtocol,
                         sizeof(kHIDDescriptorReport),
                         kEndpointAddress,
                         CFG_TUD_HID_EP_BUFSIZE,
                         kEndpointIntervalMs)};

  HID() = delete;
  ~HID() = delete;

  /**
   * Initialize the USB instance.
   */
  static esp_err_t Initialize();

  /**
   * Standard HID Boot Protocol Keyboard Report.
   *
   * @param report_id // Can be zero.
   * @param modifier  // Keyboard modifier (KEYBOARD_MODIFIER_* masks).
   * @param keycode   // Key codes of the currently pressed keys.
   *
   * @return ESP_OK on success, else other error code.
   */
  static esp_err_t KeyboardReport(uint8_t report_id,
                                  uint8_t modifier,
                                  uint8_t keycode[6]);

  static esp_err_t KeyboardPress(uint8_t report_id, char ch);

  static esp_err_t KeyboardRelease(uint8_t report_id);

  static bool Ready();
};

}  // namespace usb