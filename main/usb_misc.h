#pragma once

#include <cstdint>

namespace usb {

int strcpy_utf16(uint16_t* buf, const char* s, int bufsize);

}  // namespace usb