#include "usb.h"

#include <class/hid/hid.h>
#include <class/hid/hid_device.h>
#include <esp_log.h>
#include <freertos/task.h>
#include <tusb.h>

namespace {

constexpr uint8_t desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_PROTOCOL_KEYBOARD))};
constexpr char TAG[] = "kbd_hid";
constexpr char kUSBDescriptor[] = "Test Keyboard";

USB* g_usb;

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void hid_set_report_cb(TU_ATTR_UNUSED uint8_t report_id,
                       hid_report_type_t report_type,
                       uint8_t const* buffer,
                       uint16_t bufsize) {
  if (!g_usb)
    return;

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

USB::USB() {
  g_usb = this;
}

USB::~USB() {
  g_usb = nullptr;
}

// static
bool USB::Mounted() {
  return USBDevice.mounted();
}

esp_err_t USB::Initialize() {
  if (!tusb_init())
    return ESP_FAIL;

  constexpr uint8_t kPollIntervalMs = 2;

  usb_hid_.setPollInterval(kPollIntervalMs);
  usb_hid_.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid_.setReportCallback(nullptr, hid_set_report_cb);
  usb_hid_.setStringDescriptor(kUSBDescriptor);

  if (!usb_hid_.begin())
    return ESP_FAIL;

  return ESP_OK;
}

esp_err_t USB::KeyboardReport(uint8_t report_id,
                              uint8_t modifier,
                              uint8_t keycode[6]) {
  return usb_hid_.keyboardReport(report_id, modifier, keycode) ? ESP_OK
                                                               : ESP_FAIL;
}

esp_err_t USB::KeyboardRelease(uint8_t report_id) {
  return usb_hid_.keyboardRelease(report_id) ? ESP_OK : ESP_FAIL;
}

bool USB::Ready() {
  return usb_hid_.ready();
}

// static
esp_err_t USB::RemoteWakup() {
  return USBDevice.remoteWakeup() ? ESP_OK : ESP_FAIL;
}

// static
bool USB::Suspended() {
  return USBDevice.suspended();
}