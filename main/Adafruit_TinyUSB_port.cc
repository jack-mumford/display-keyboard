#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

#include <Adafruit_TinyUSB_Core.h>
#include <esp_log.h>
#include <tusb.h>
namespace {
constexpr char TAG[] = "usb_port";
}

void Adafruit_TinyUSB_Core_init() {
  ESP_LOGE(TAG, "Adafruit_TinyUSB_Core_init");
  tusb_init();
}

extern void Adafruit_TinyUSB_Core_touch1200(void) {
  ESP_LOGE(TAG, "Adafruit_TinyUSB_Core_touch1200");
}
