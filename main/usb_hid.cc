#include "usb_hid.h"

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <freertos/FreeRTOS.h>

#include <class/hid/hid_device.h>
#include <esp_log.h>
#include <tusb.h>
#include "usb_device.h"

namespace usb {

constexpr uint8_t HID::kHIDDescriptorReport[];
constexpr char HID::kInterfaceName[];
constexpr uint8_t HID::kHIDDescriptorConfig[];

namespace {

constexpr char TAG[] = "HID";
constexpr uint8_t kASCII2KeyCode[128][2] = {HID_ASCII_TO_KEYCODE};

extern "C" {

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete
uint8_t const* tud_hid_descriptor_report_cb() {
  return HID::kHIDDescriptorReport;
}

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t report_id,
                               hid_report_type_t report_type,
                               uint8_t* buffer,
                               uint16_t reqlen) {
  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t report_id,
                           hid_report_type_t report_type,
                           uint8_t const* buffer,
                           uint16_t bufsize) {
  if (report_type != HID_REPORT_TYPE_OUTPUT)
    return;
  if (bufsize < 1)
    return;

  const uint8_t num_lock = buffer[KEYBOARD_LED_NUMLOCK];
  const uint8_t caps_lock = buffer[KEYBOARD_LED_CAPSLOCK];

  ESP_LOGI(TAG, "NUM LOCK: %c, CAPS LOCK: %c", num_lock ? 'Y' : 'N',
           caps_lock ? 'Y' : 'N');
}

}  // extern "C"

}  // namespace

// static
esp_err_t HID::KeyboardReport(uint8_t report_id,
                              uint8_t modifier,
                              const uint8_t keycode[6]) {
  return tud_hid_keyboard_report(report_id, modifier,
                                 const_cast<uint8_t*>(keycode))
             ? ESP_OK
             : ESP_FAIL;
}

// static
esp_err_t HID::KeyboardPress(uint8_t report_id, char ch) {
  uint8_t keycode[6] = {HID_KEY_NONE};
  uint8_t modifier = 0;

  const int idx = static_cast<int>(ch);
  if (kASCII2KeyCode[idx][0])
    modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
  keycode[0] = kASCII2KeyCode[idx][1];

  return tud_hid_keyboard_report(report_id, modifier, keycode) ? ESP_OK
                                                               : ESP_FAIL;
}

// static
esp_err_t HID::KeyboardRelease(uint8_t report_id) {
  return tud_hid_keyboard_report(report_id, 0, nullptr) ? ESP_OK : ESP_FAIL;
}

// static
bool HID::Ready() {
  return tud_hid_ready();
}

}  // namespace usb
