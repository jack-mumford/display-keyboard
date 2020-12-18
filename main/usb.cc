#include "usb.h"

#include <Adafruit_USBD_Device.h>
#include <class/hid/hid.h>
#include <class/hid/hid_device.h>
#include <esp_log.h>
#include <freertos/task.h>
#include <tusb.h>

namespace usb {

namespace {
constexpr char TAG[] = "kbd_usb";
}

// static
bool USB::Mounted() {
  return USBDevice.mounted();
}

// static
esp_err_t USB::Initialize() {
  if (!tusb_init())
    return ESP_FAIL;
  ESP_LOGI(TAG, "USB device is initialized");
  return ESP_OK;
}

// static
esp_err_t USB::RemoteWakup() {
  return USBDevice.remoteWakeup() ? ESP_OK : ESP_FAIL;
}

// static
bool USB::Suspended() {
  return USBDevice.suspended();
}

}  // namespace usb