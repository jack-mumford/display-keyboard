
#pragma once

#include <freertos/FreeRTOS.h>

#include <esp_err.h>

namespace usb {

class Device {
 public:
  Device() = delete;
  ~Device() = delete;

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

  static void Tick();
};

}  // namespace usb