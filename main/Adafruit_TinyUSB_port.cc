#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <Adafruit_USBD_Device.h>
#include <esp_log.h>
#include <tusb.h>
namespace {
constexpr char TAG[] = "usb_port";
}

void Adafruit_TinyUSB_Core_init() {
  ESP_LOGE(TAG, "Adafruit_TinyUSB_Core_init");
  tusb_init();
}

void Adafruit_TinyUSB_Core_touch1200(void) {
  ESP_LOGE(TAG, "Adafruit_TinyUSB_Core_touch1200");
}

uint8_t Adafruit_USBD_Device::getSerialDescriptor(uint16_t* serial_str) {
  constexpr uint16_t kSerialDescriptor[] = {'K', 'e', 'y', 'b', 'o',
                                            'a', 'r', 'd', '\0'};

  ESP_LOGE(TAG, "Adafruit_USBD_Device::getSerialDescriptor");
  memcpy(serial_str, kSerialDescriptor, sizeof(kSerialDescriptor));
  return 8;  // # of characters.
}