/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "usb_misc.h"

#include <tusb.h>

namespace {

constexpr static inline bool isInvalidUtf8Octet(uint8_t t) {
  // see bullets in https://tools.ietf.org/html/rfc3629#section-1
  return (t == 0xc0) || (t == 0xC1) || (t >= 0xF5);
}

//
// This function has an UNWRITTEN CONTRACT that the buffer is either:
// 1. Pre-validated as legal UTF-8, -OR-
// 2. has a trailing zero-value octet/byte/uint8_t  (aka null-terminated string)
//
// If the above are not true, this decoder may read past the end of the
// allocated buffer, by up to three bytes.
//
// U+1F47F == 👿 ("IMP")
//         == 0001_1111_0100_0111_1111 ==> requires four-byte encoding in UTF-8
//            AABB BBBB CCCC CCDD DDDD ==> 0xF0 0x9F 0x91 0xBF
//
// Example sandwich and safety variables are there to cover the
// two most-common stack layouts for declared variables, in unoptimized
// code, so that the bytes surrounding those allocated for 'evilUTF8'
// are guaranteed to be non-zero and valid UTF8 continuation octets.
//     uint8_t safety1      = 0;
//     uint8_t sandwich1[4] = { 0x81, 0x82, 0x83, 0x84 };
//     uint8_t evilUTF8[5]  = { 0xF0, 0x9F, 0x91, 0xBF, 0xF9 };
//     uint8_t sandwich2[4] = { 0x85, 0x86, 0x87, 0x88 };
//     uint8_t safety2      = 0;
//
// NOTE: evilUTF8 could just contain a single byte 0xF9 ....
//
// Attempting to decode evilUTF8 will progress to whatever is next to it on the
// stack. The above should work when optimizations are turned
//
static int8_t utf8Codepoint(const uint8_t* utf8, uint32_t* codepointp) {
  const uint32_t CODEPOINT_LOWEST_SURROGATE_HALF = 0xD800;
  const uint32_t CODEPOINT_HIGHEST_SURROGATE_HALF = 0xDFFF;

  *codepointp = 0xFFFD;  // always initialize output to known value ... 0xFFFD
                         // (REPLACEMENT CHARACTER) seems the natural choice
  uint32_t codepoint;
  int len;

  // The upper bits define both the length of additional bytes for the
  // multi-byte encoding, as well as defining how many bits of the first byte
  // are included in the codepoint. Each additional byte starts with 0b10xxxxxx,
  // encoding six additional bits for the codepoint.
  //
  // For key summary points, see:
  // * https://tools.ietf.org/html/rfc3629#section-3
  //
  if (isInvalidUtf8Octet(
          utf8[0])) {  // do not allow illegal octet sequences (e.g., 0xC0 0x80
                       // should NOT decode to NULL)
    return -1;
  }

  if (utf8[0] <
      0x80) {  // characters 0000 0000..0000 007F (up to  7 significant bits)
    len = 1;
    codepoint = utf8[0];
  } else if ((utf8[0] & 0xe0) ==
             0xc0) {  // characters 0000 0080..0000 07FF (up to 11 significant
                      // bits, so first byte encodes five bits)
    len = 2;
    codepoint = utf8[0] & 0x1f;
  } else if ((utf8[0] & 0xf0) ==
             0xe0) {  // characters 0000 8000..0000 FFFF (up to 16 significant
                      // bits, so first byte encodes four bits)
    len = 3;
    codepoint = utf8[0] & 0x0f;
  } else if ((utf8[0] & 0xf8) ==
             0xf0) {  // characters 0001 0000..0010 FFFF (up to 21 significant
                      // bits, so first byte encodes three bits)
    len = 4;
    codepoint = utf8[0] & 0x07;
  } else {  // UTF-8 is defined to only map to Unicode -- 0x00000000..0x0010FFFF
    // 5-byte and 6-byte sequences are not legal per RFC3629
    return -1;
  }

  for (int i = 1; i < len; i++) {
    if ((utf8[i] & 0xc0) != 0x80) {
      // the additional bytes in a valid UTF-8 multi-byte encoding cannot have
      // either of the top two bits set This is more restrictive than
      // isInvalidUtf8Octet()
      return -1;
    }
    codepoint <<= 6;  // each continuation byte adds six bits to the codepoint
    codepoint |= utf8[i] & 0x3f;  // mask off the top two continuation bits, and
                                  // add the six relevant bits
  }

  // explicit validation to prevent overlong encodings
  if ((len == 1) && (codepoint > 0x00007F)) {
    return -1;
  } else if ((len == 2) && ((codepoint < 0x000080) || (codepoint > 0x0007FF))) {
    return -1;
  } else if ((len == 3) && ((codepoint < 0x000800) || (codepoint > 0x00FFFF))) {
    return -1;
  } else if ((len == 4) && ((codepoint < 0x010000) || (codepoint > 0x10FFFF))) {
    // "You might expect larger code points than U+10FFFF
    // to be expressible, but Unicode is limited in Sections 12
    // of RFC3629 to match the limits of UTF-16." -- Wikipedia UTF-8 note
    // See https://tools.ietf.org/html/rfc3629#section-12
    return -1;
  }

  // high and low surrogate halves (U+D800 through U+DFFF) used by UTF-16 are
  // not legal Unicode values ... see RFC 3629.
  if ((codepoint >= CODEPOINT_LOWEST_SURROGATE_HALF) &&
      (codepoint <= CODEPOINT_HIGHEST_SURROGATE_HALF)) {
    return -1;
  }

  *codepointp = codepoint;
  return len;
}

}  // namespace

//--------------------------------------------------------------------+
// Some of TinyUSB class driver requires strong callbacks, which cause
// link error if 'Adafruit_TinyUSB_Arduino' is not included, provide
// weak callback here to prevent linking error
//--------------------------------------------------------------------+

extern "C" {

// MSC
TU_ATTR_WEAK int32_t tud_msc_read10_cb(uint8_t lun,
                                       uint32_t lba,
                                       uint32_t offset,
                                       void* buffer,
                                       uint32_t bufsize) {
  return -1;
}
TU_ATTR_WEAK int32_t tud_msc_write10_cb(uint8_t lun,
                                        uint32_t lba,
                                        uint32_t offset,
                                        uint8_t* buffer,
                                        uint32_t bufsize) {
  return -1;
}
TU_ATTR_WEAK void tud_msc_inquiry_cb(uint8_t lun,
                                     uint8_t vendor_id[8],
                                     uint8_t product_id[16],
                                     uint8_t product_rev[4]) {}
TU_ATTR_WEAK bool tud_msc_test_unit_ready_cb(uint8_t lun) {
  return false;
}
TU_ATTR_WEAK void tud_msc_capacity_cb(uint8_t lun,
                                      uint32_t* block_count,
                                      uint16_t* block_size) {}
TU_ATTR_WEAK int32_t tud_msc_scsi_cb(uint8_t lun,
                                     uint8_t const scsi_cmd[16],
                                     void* buffer,
                                     uint16_t bufsize) {
  return -1;
}

}  // extern "C"

namespace usb {

int strcpy_utf16(uint16_t* buf, const char* s, int bufsize) {
  int i = 0;
  int buflen = 0;

  while (s[i] != 0) {
    uint32_t codepoint;
    int8_t utf8len = utf8Codepoint((const uint8_t*)s + i, &codepoint);

    if (utf8len < 0) {
      // Invalid utf8 sequence, skip it
      i++;
      continue;
    }

    i += utf8len;

    if (codepoint <= 0xffff) {
      if (buflen == bufsize)
        break;

      buf[buflen++] = codepoint;

    } else {
      if (buflen + 1 >= bufsize)
        break;

      // Surrogate pair
      codepoint -= 0x10000;
      buf[buflen++] = (codepoint >> 10) + 0xd800;
      buf[buflen++] = (codepoint & 0x3ff) + 0xdc00;
    }
  }

  return buflen;
}

}  // namespace usb
