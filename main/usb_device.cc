#include "usb_device.h"

#include <cstring>

#include <esp_log.h>
#include <freertos/task.h>
#include <tusb.h>
#include "usb_board.h"
#include "usb_hid.h"
#include "usb_misc.h"
#include "usb_string_ids.h"

namespace usb {

namespace {

struct ConfigDescriptors {
  tusb_desc_configuration_t device;
  uint8_t hid[sizeof(HID::kHIDDescriptorConfig)];
};

// Character 1 = (length) + (string type).
// Characters 2..n = descriptor string (not null terminated).
constexpr uint8_t kMaxDescriptorStringLen = 32;
typedef uint16_t DescriptorString[1 + kMaxDescriptorStringLen];

// Make sure compiler didn't pack any space in between the members.
static_assert(sizeof(ConfigDescriptors) ==
              sizeof(tusb_desc_configuration_t) +
                  sizeof(HID::kHIDDescriptorConfig));

constexpr char TAG[] = "kbd_usb";
// TODO: These are from random.org. Need to get actual VID/PID numbers to
//       avoid conflicts with other products.
constexpr uint16_t kVendorID = 0xae9b;
constexpr uint16_t kProductID = 0xe67a;
constexpr uint16_t kDeviceVersion = 0x0100;  // BCD.
constexpr uint16_t kVersion = 0x0100;        // BCD.
constexpr char kDeviceSerialNumber[] = "00001A";
constexpr char kDeviceManufacturer[] = "Awesome Keyboard Co.";
constexpr char kProduct[] = "Super Display Keyboard";
constexpr uint16_t kLanguage = 0x0409;  // = English
constexpr uint8_t kMaxPower = 150;      // 2 mA units (i.e., 50 = 100 mA).
constexpr tusb_desc_device_t kDeviceDescriptor = {
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
    .bNumConfigurations = 0x01,
};
constexpr tusb_desc_configuration_t kDeviceDescriptorConfig = {
    .bLength = sizeof(tusb_desc_configuration_t),
    .bDescriptorType = TUSB_DESC_CONFIGURATION,
    .wTotalLength = sizeof(ConfigDescriptors),
    .bNumInterfaces = 1,
    .bConfigurationValue = 1,
    .iConfiguration = 0x00,
    .bmAttributes = TU_BIT(7) | TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP,
    .bMaxPower = TUSB_DESC_CONFIG_POWER_MA(kMaxPower),
};

static_assert(sizeof(kDeviceManufacturer) <= kMaxDescriptorStringLen);
static_assert(sizeof(kProduct) <= kMaxDescriptorStringLen);
static_assert(sizeof(kDeviceSerialNumber) <= kMaxDescriptorStringLen);
static_assert(sizeof(HID::kInterfaceName) <= kMaxDescriptorStringLen);

ConfigDescriptors g_config_descriptors;
DescriptorString g_descriptor_strings[STRID_NUM];
DescriptorString g_unknown_descriptor_string;

const char* GetDescriptorString(StringID id) {
  switch (id) {
    case STRID_MANUFACTURER:
      return kDeviceManufacturer;
    case STRID_PRODUCT:
      return kProduct;
    case STRID_SERIAL:
      return kDeviceSerialNumber;
    case STRID_HID:
      return HID::kInterfaceName;
    case STRID_LANGUAGE:
      // fallthrough - handled differently.
    case STRID_NUM:
      // fallthrough
      break;
  }
  return nullptr;
}

uint16_t CreateDecStringHeader(uint8_t num_chars) {
  // Byte #1 = length. Byte #2 = string type.
  return (TUSB_DESC_STRING << 8) | (2 + 2 * num_chars);
}

void InitializeDescriptorString(DescriptorString dst, const char* src) {
  const int num_chars = strcpy_utf16(dst + 1, src, kMaxDescriptorStringLen);
  dst[0] = CreateDecStringHeader(num_chars);
}

void InitializeDescriptorString(StringID id) {
  InitializeDescriptorString(g_descriptor_strings[id], GetDescriptorString(id));
}

extern "C" {

// Invoked when received GET DEVICE DESCRIPTOR
// Application return pointer to descriptor
uint8_t const* tud_descriptor_device_cb(void) {
  return reinterpret_cast<uint8_t const*>(&kDeviceDescriptor);
}

// Invoked when received GET CONFIGURATION DESCRIPTOR
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete
uint8_t const* tud_descriptor_configuration_cb(uint8_t /*index*/) {
  return reinterpret_cast<uint8_t const*>(&g_config_descriptors);
}

// Invoked when received GET STRING DESCRIPTOR request
// Application return pointer to descriptor, whose contents must exist long
// enough for transfer to complete Note: the 0xEE index string is a Microsoft
// OS 1.0 Descriptors.
// https://docs.microsoft.com/en-us/windows-hardware/drivers/usbcon/microsoft-defined-usb-descriptors
uint16_t const* tud_descriptor_string_cb(uint8_t index, uint16_t /*langid*/) {
  if (index < STRID_NUM)
    return g_descriptor_strings[index];
  return g_unknown_descriptor_string;
}

}  // extern C

}  // namespace

// static
bool Device::Mounted() {
  return tud_mounted();
}

// static
esp_err_t Device::Initialize() {
  g_config_descriptors.device = kDeviceDescriptorConfig;
  std::memcpy(g_config_descriptors.hid, HID::kHIDDescriptorConfig,
              sizeof(HID::kHIDDescriptorConfig));
  static_assert(HID::kHIDDescriptorConfig[1] == TUSB_DESC_INTERFACE);

  for (int i = 0; i < STRID_NUM; i++) {
    if (i != STRID_LANGUAGE)
      InitializeDescriptorString(static_cast<StringID>(i));
  }
  g_descriptor_strings[STRID_LANGUAGE][1] = ((uint16_t)((uint32_t)kLanguage));
  g_descriptor_strings[STRID_LANGUAGE][0] = CreateDecStringHeader(1);
  InitializeDescriptorString(g_unknown_descriptor_string, "<unknown>");

  board_init();

  if (!tusb_init())
    return ESP_FAIL;

  ESP_LOGI(TAG, "USB device is initialized");
  return ESP_OK;
}

// static
esp_err_t Device::RemoteWakup() {
  return tud_remote_wakeup() ? ESP_OK : ESP_FAIL;
}

// static
bool Device::Suspended() {
  return tud_suspended();
}

// static
void Device::Tick() {
  tud_task();
}

}  // namespace usb