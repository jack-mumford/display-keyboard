
#pragma once

#include <freertos/FreeRTOS.h>

#include <esp_err.h>

namespace usb {

class USB {
 public:
  USB() = delete;
  ~USB() = delete;

  /**
   * Initialize the USB device.
   */
  static esp_err_t Initialize();

  /**
   * Is the device connected and configured.
   */
  static bool Mounted();

  /**
   * Remote wake up host, only if suspended and enabled by host.
   */
  static esp_err_t RemoteWakup();

  /**
   * Check if device is suspended.
   */
  static bool Suspended();
};

}  // namespace usb