#include "usb.h"

#include <class/hid/hid.h>
#include <class/hid/hid_device.h>
#include <tusb.h>

extern "C" {

namespace {
USB* g_usb;
}

enum { REPORT_ID_KEYBOARD = 1, REPORT_ID_MOUSE };

constexpr uint8_t desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(REPORT_ID_MOUSE))};

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
uint16_t tud_hid_get_report_cb(uint8_t report_id,
                               hid_report_type_t report_type,
                               uint8_t* buffer,
                               uint16_t reqlen) {
  if (!g_usb)
    return 0;
  // TODO not Implemented
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)reqlen;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void tud_hid_set_report_cb(uint8_t report_id,
                           hid_report_type_t report_type,
                           uint8_t const* buffer,
                           uint16_t bufsize) {
  if (!g_usb)
    return;
  // TODO set LED based on CAPLOCK, NUMLOCK etc...
  (void)report_id;
  (void)report_type;
  (void)buffer;
  (void)bufsize;
}

// Invoked when received GET HID REPORT DESCRIPTOR
// Application return pointer to descriptor
// Descriptor contents must exist long enough for transfer to complete
uint8_t const* tud_hid_descriptor_report_cb(void) {
  return desc_hid_report;
}

}  // extern "C"

USB::USB() {
  g_usb = this;
  tusb_init();
}

USB::~USB() {
  g_usb = nullptr;
}
