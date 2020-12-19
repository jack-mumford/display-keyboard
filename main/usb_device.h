
#pragma once

#include <memory>

#include <freertos/FreeRTOS.h>

#include <esp_err.h>

#include "Adafruit_USBD_Device.h"

namespace usb {

class Device {
 public:
  Device();
  ~Device();

  /**
   * Initialize the USB device.
   */
  esp_err_t Initialize();

  /**
   * Is the device connected and configured.
   */
  bool Mounted();

  /**
   * Remote wake up host, only if suspended and enabled by host.
   */
  esp_err_t RemoteWakup();

  /**
   * Check if device is suspended.
   */
  bool Suspended();

  void Tick();

 private:
  Adafruit_USBD_Device device_;
};

}  // namespace usb