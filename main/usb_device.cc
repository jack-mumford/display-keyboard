#include "usb_device.h"

#include <esp_log.h>
#include <freertos/task.h>
#include <tusb.h>
#include "usb_board.h"
#include "usb_hid.h"
#include "usb_misc.h"

namespace usb {

namespace {

enum { STRID_LANGUAGE = 0, STRID_MANUFACTURER, STRID_PRODUCT, STRID_SERIAL };

constexpr char TAG[] = "kbd_usb";
// TODO: These are from random.org. Need to get actual VID/PID numbers to
//       avoid conflicts with other products.
constexpr uint16_t kVendorID = 44699;
constexpr uint16_t kProductID = 59002;
constexpr uint16_t kDeviceVersion = 0x0100;  // BCD.
constexpr uint16_t kVersion = 0x0101;        // BCD.
constexpr char kDeviceSerialNumber[] = "00001A";
constexpr char USB_MANUFACTURER[] = "Awesome Keyboard Co.";
constexpr char USB_PRODUCT[] = "Super Display Keyboard";
constexpr uint16_t USB_LANGUAGE = 0x0409;  // default is English
constexpr uint8_t USB_CONFIG_POWER = 100;
constexpr tusb_desc_device_t desc_dev = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = kVersion,

    // Use Interface Association Descriptor (IAD) for CDC
    // As required by USB Specs IAD's subclass must be common class (2) and
    // protocol must be IAD (1)
    .bDeviceClass = TUSB_CLASS_MISC,
    .bDeviceSubClass = MISC_SUBCLASS_COMMON,
    .bDeviceProtocol = MISC_PROTOCOL_IAD,

    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor = kVendorID,
    .idProduct = kProductID,
    .bcdDevice = kDeviceVersion,
    .iManufacturer = STRID_MANUFACTURER,
    .iProduct = STRID_PRODUCT,
    .iSerialNumber = STRID_SERIAL,
    .bNumConfigurations = 0x01};
constexpr tusb_desc_configuration_t dev_cfg = {
    .bLength = sizeof(tusb_desc_configuration_t),
    .bDescriptorType = TUSB_DESC_CONFIGURATION,
    .wTotalLength =
        sizeof(tusb_desc_configuration_t) + sizeof(HID::kHIDDescriptor),
    .bNumInterfaces = 1,
    .bConfigurationValue = 1,
    .iConfiguration = 0x00,
    .bmAttributes = TU_BIT(7) | TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP,
    .bMaxPower = TUSB_DESC_CONFIG_POWER_MA(USB_CONFIG_POWER)};

extern "C" {

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const* tud_descriptor_device_cb(void) {
  return reinterpret_cast<uint8_t const*>(&desc_dev);
}

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete
uint8_t const* tud_descriptor_configuration_cb(uint8_t /*index*/) {
  return reinterpret_cast<uint8_t const*>(&dev_cfg);
}

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete Note: the 0xEE index string is a Microsoft
// OS 1.0 Descriptors.
// https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t langid) {
  (void)langid;

  // up to 32 unicode characters (header make it 33)
  static uint16_t _desc_str[33];
  uint8_t chr_count;
  constexpr uint8_t kMaxDescriptorLen =
      (sizeof(_desc_str) / sizeof(_desc_str[0])) - 1;

  switch (index) {
    case STRID_LANGUAGE:
      _desc_str[1] = ((uint16_t)((uint32_t)USB_LANGUAGE));
      chr_count = 1;
      break;

    case STRID_MANUFACTURER:
      chr_count =
          strcpy_utf16(_desc_str + 1, USB_MANUFACTURER, kMaxDescriptorLen);
      break;
    case STRID_PRODUCT:
      chr_count = strcpy_utf16(_desc_str + 1, USB_PRODUCT, kMaxDescriptorLen);
      break;
    case STRID_SERIAL:
      chr_count =
          strcpy_utf16(_desc_str + 1, kDeviceSerialNumber, kMaxDescriptorLen);
      break;

    default:
      chr_count = strcpy_utf16(_desc_str + 1, "<Unknown>", kMaxDescriptorLen);
      break;
  }

  // first byte is length (including header), second byte is string type
  _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

  return _desc_str;
}

}  // extern C

}  // namespace

Device::Device() = default;

Device::~Device() = default;

bool Device::Mounted() {
  return tud_mounted();
  ;
}

esp_err_t Device::Initialize() {
  board_init();

  if (!tusb_init())
    return ESP_FAIL;

  ESP_LOGI(TAG, "USB device is initialized");
  return ESP_OK;
}

esp_err_t Device::RemoteWakup() {
  return tud_remote_wakeup() ? ESP_OK : ESP_FAIL;
}

bool Device::Suspended() {
  return tud_suspended();
  ;
}

void Device::Tick() {
  tud_task();
}

}  // namespace usb