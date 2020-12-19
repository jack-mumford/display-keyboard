#include "usb_device.h"

#include <esp_log.h>
#include <freertos/task.h>
#include <tusb.h>
#include "usb_board.h"
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

    // Total Length & Interface Number will be updated later
    .wTotalLength = 0,
    .bNumInterfaces = 0,
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
#if 0
  device_.setVersion(1);
  device_.setDeviceVersion(1);
  if (!device_.begin())
    return ESP_OK;
#endif

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

#if 0
// Add interface descriptor
// - Interface number will be updated to match current count
// - Endpoint number is updated to be unique
esp_err_t Device::addInterface(const Interface* ifname) {
  uint8_t* desc = _desc_cfg + _desc_cfg_len;
  uint16_t const len =
      itf->getDescriptor(_itf_count, desc, _desc_cfg_maxlen - _desc_cfg_len);
  uint8_t* desc_end = desc + len;

  const char* desc_str = itf->getStringDescriptor();

  if (!len)
    return ESP_ERR_INVALID_SIZE;

  // Parse interface descriptor to update
  // - Interface Number & string descrioptor
  // - Endpoint address
  while (desc < desc_end) {
    if (desc[1] == TUSB_DESC_INTERFACE) {
      tusb_desc_interface_t* desc_itf = (tusb_desc_interface_t*)desc;
      if (desc_itf->bAlternateSetting == 0) {
        _itf_count++;
        if (desc_str && (_desc_str_count < STRING_DESCRIPTOR_MAX)) {
          _desc_str_arr[_desc_str_count] = desc_str;
          desc_itf->iInterface = _desc_str_count;
          _desc_str_count++;

          // only assign string index to first interface
          desc_str = NULL;
        }
      }
    } else if (desc[1] == TUSB_DESC_ENDPOINT) {
      tusb_desc_endpoint_t* desc_ep = (tusb_desc_endpoint_t*)desc;
      desc_ep->bEndpointAddress |=
          (desc_ep->bEndpointAddress & 0x80) ? _epin_count++ : _epout_count++;
    }

    if (desc[0] == 0)
      return ESP_FAIL;
    desc += desc[0];  // next
  }

  _desc_cfg_len += len;

  // Update configuration descriptor
  tusb_desc_configuration_t* config = (tusb_desc_configuration_t*)_desc_cfg;
  config->wTotalLength = _desc_cfg_len;
  config->bNumInterfaces = _itf_count;

  return ESP_OK;
}

// static
esp_err_t Device::AddInterface(const Interface* ifname) {
  g_device->addInterface(ifname);
}
#endif

}  // namespace usb