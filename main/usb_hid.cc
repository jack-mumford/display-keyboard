#include "usb_hid.h"

#include <class/hid/hid.h>
#include <class/hid/hid_device.h>
#include <esp_log.h>
#include <freertos/task.h>
#include <tusb.h>

namespace usb {

namespace {

constexpr uint8_t desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_PROTOCOL_KEYBOARD))};
constexpr char TAG[] = "kbd_hid";
constexpr char kUSBDescriptor[] = "Test Keyboard";

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void hid_set_report_cb(TU_ATTR_UNUSED uint8_t report_id,
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

}  // namespace

HID::HID() = default;

HID::~HID() = default;

esp_err_t HID::Initialize() {
  constexpr uint8_t kPollIntervalMs = 2;

  usb_hid_.setPollInterval(kPollIntervalMs);
  usb_hid_.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid_.setReportCallback(nullptr, hid_set_report_cb);
  usb_hid_.setStringDescriptor(kUSBDescriptor);

  if (!usb_hid_.begin())
    return ESP_FAIL;

  return ESP_OK;
}

esp_err_t HID::KeyboardReport(uint8_t report_id,
                              uint8_t modifier,
                              uint8_t keycode[6]) {
  return usb_hid_.keyboardReport(report_id, modifier, keycode) ? ESP_OK
                                                               : ESP_FAIL;
}

esp_err_t HID::KeyboardPress(uint8_t report_id, char ch) {
  return usb_hid_.keyboardPress(report_id, ch) ? ESP_OK : ESP_FAIL;
}

esp_err_t HID::KeyboardRelease(uint8_t report_id) {
  return usb_hid_.keyboardRelease(report_id) ? ESP_OK : ESP_FAIL;
}

bool HID::Ready() {
  return usb_hid_.ready();
}

}  // namespace usb