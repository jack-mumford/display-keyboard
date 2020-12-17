#include "usb.h"

#include <class/hid/hid.h>
#include <class/hid/hid_device.h>
#include <esp_log.h>
#include <freertos/task.h>
#include <tusb.h>

namespace {

enum { REPORT_ID_KEYBOARD = 1, REPORT_ID_MOUSE };

constexpr uint8_t desc_hid_report[] = {
    TUD_HID_REPORT_DESC_KEYBOARD(HID_REPORT_ID(REPORT_ID_KEYBOARD)),
    TUD_HID_REPORT_DESC_MOUSE(HID_REPORT_ID(REPORT_ID_MOUSE))};
constexpr char TAG[] = "kbd_hid";
constexpr char kUSBDescriptor[] = "Test Keyboard";

USB* g_usb;

}  // namespace

// Invoked when received GET_REPORT control request
// Application must fill buffer report's content and return its length.
// Return zero will cause the stack to STALL request
extern "C" uint16_t hid_get_report_cb(TU_ATTR_UNUSED uint8_t report_id,
                                      TU_ATTR_UNUSED hid_report_type_t
                                          report_type,
                                      TU_ATTR_UNUSED uint8_t* buffer,
                                      TU_ATTR_UNUSED uint16_t reqlen) {
  if (!g_usb)
    return 0;

  return 0;
}

// Invoked when received SET_REPORT control request or
// received data on OUT endpoint ( Report ID = 0, Type = 0 )
void hid_set_report_cb(TU_ATTR_UNUSED uint8_t report_id,
                       hid_report_type_t report_type,
                       uint8_t const* buffer,
                       TU_ATTR_UNUSED uint16_t bufsize) {
  if (!g_usb)
    return;

  // LED indicator is output report with only 1 byte length
  if (report_type != HID_REPORT_TYPE_OUTPUT)
    return;

  // The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
  // Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
  const uint8_t ledIndicator = buffer[0];

  ESP_LOGI(TAG, "LED indicator: %c", ledIndicator ? 'Y' : 'N');
}

USB::USB() {
  g_usb = this;
  tusb_init();
}

USB::~USB() {
  g_usb = nullptr;
}

esp_err_t USB::Initialize() {
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.setReportCallback(hid_get_report_cb, hid_set_report_cb);
  usb_hid.setStringDescriptor(kUSBDescriptor);

  if (!usb_hid.begin())
    return ESP_FAIL;

  // wait until device mounted
  while (!USBDevice.mounted())
    vTaskDelay(200 / portTICK_PERIOD_MS);

  return ESP_OK;
}