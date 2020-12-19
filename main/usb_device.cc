#include "usb_device.h"

#include <Adafruit_USBD_Device.h>
#include "hardware.h"
#include <freertos/task.h>
#include <esp_log.h>
#include <tusb.h>

namespace usb {

namespace {
constexpr char TAG[] = "kbd_usb";

class Port : public Adafruit_USBD_Device_Port {
 public:
  uint8_t getSerialDescriptor(uint16_t* serial_str,
                              uint8_t max_num_chars) override {
    constexpr char kSerialDescriptor[] = "Display Keyboard";
    if (!max_num_chars)
      return 0;
    uint8_t i = 0;
    for (; i < sizeof(kSerialDescriptor) && i < max_num_chars; i++)
      serial_str[i] = kSerialDescriptor[i];
    return i - 1;
  }
};

}  // namespace

Device::Device() : device_(std::unique_ptr<Port>(new Port())) {}

Device::~Device() = default;

bool Device::Mounted() {
  return device_.mounted();
}

esp_err_t Device::Initialize() {
  // device_.addInterface(); // Serial
  // TODO: These are from random.org. Need to get actual VID/PID numbers to
  //       avoid conflicts with other products.
  constexpr uint16_t kVendorID = 44699;
  constexpr uint16_t kProductID = 59002;

  device_.setID(kVendorID, kProductID);
  if (!device_.begin())
    return ESP_OK;

  board_init();

  if (!tusb_init())
    return ESP_FAIL;

  ESP_LOGI(TAG, "USB device is initialized");
  return ESP_OK;
}

esp_err_t Device::RemoteWakup() {
  return device_.remoteWakeup() ? ESP_OK : ESP_FAIL;
}

bool Device::Suspended() {
  return device_.suspended();
}

void Device::Tick() {
  tud_task();
}

}  // namespace usb