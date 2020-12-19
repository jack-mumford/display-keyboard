#include "usb_hid.h"

#include <freertos/FreeRTOS.h>

#include <class/hid/hid_device.h>
#include <esp_log.h>
#include <tusb.h>
#include "usb_device.h"

namespace usb {

namespace {

constexpr uint8_t kEndpointInput = 0x80;
#if 0
constexpr char kUSBDescriptor[] = "Test Keyboard";
constexpr uint8_t desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(HID_PROTOCOL_KEYBOARD))};
constexpr char TAG[] = "kbd_hid";
constexpr uint8_t kASCII2KeyCode[128][2] = {HID_ASCII_TO_KEYCODE};
constexpr uint8_t kEndpointIntervalMs = 2;
constexpr uint8_t kBootProtocol = HID_PROTOCOL_NONE;
#endif

extern "C" {

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete
uint8_t const* tud_hid_descriptor_report_cb(void) {
  return desc_hid_report;
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

HID::HID() = default;

HID::~HID() = default;

esp_err_t HID::Initialize() {
  setStringDescriptor(kUSBDescriptor);
  return Device::AddInterface(this);
}

esp_err_t HID::KeyboardReport(uint8_t report_id,
                              uint8_t modifier,
                              uint8_t keycode[6]) {
  return tud_hid_keyboard_report(report_id, modifier, keycode) ? ESP_OK
                                                               : ESP_FAIL;
}

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

esp_err_t HID::KeyboardRelease(uint8_t report_id) {
  return tud_hid_keyboard_report(report_id, 0, nullptr) ? ESP_OK : ESP_FAIL;
}

bool HID::Ready() {
  return tud_hid_ready();
}

uint16_t HID::getDescriptor(uint8_t itfnum, uint8_t* buf, uint16_t bufsize) {
  // usb core will automatically update endpoint number
  const uint8_t desc_in_only[] = {TUD_HID_DESCRIPTOR(
      itfnum, 0, kBootProtocol, sizeof(desc_hid_report), kEndpointInput,
      CFG_TUD_HID_EP_BUFSIZE, kEndpointIntervalMs)};

  if (bufsize < sizeof(desc_in_only))
    return 0;

  memcpy(buf, desc_in_only, sizeof(desc_in_only));
  return sizeof(desc_in_only);
}

}  // namespace usb