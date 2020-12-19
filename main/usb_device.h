
#pragma once

#include <memory>

#include <freertos/FreeRTOS.h>

#include <esp_err.h>

namespace usb {

class Interface {
 protected:
  const char* _desc_str;

 public:
  Interface();

  virtual uint16_t getDescriptor(uint8_t itfnum,
                                 uint8_t* buf,
                                 uint16_t bufsize) = 0;
  void setStringDescriptor(const char* str) { _desc_str = str; }
  const char* getStringDescriptor(void) { return _desc_str; }
};

class Device {
 public:
  //static esp_err_t AddInterface(const Interface* ifname);

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
  //esp_err_t addInterface(const Interface* ifname);
};

}  // namespace usb