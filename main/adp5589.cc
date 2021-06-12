#include "adp5589.h"

namespace adp5589 {
namespace Register {

const char* EventToName(EventID event) {
  switch (static_cast<uint8_t>(event)) {
    case 0:
      return "NONE";
    case 1:
      return "R0_C0";
    case 2:
      return "R0_C1";
    case 3:
      return "R0_C2";
    case 4:
      return "R0_C3";
    case 5:
      return "R0_C4";
    case 6:
      return "R0_C5";
    case 7:
      return "R0_C6";
    case 8:
      return "R0_C7";
    case 9:
      return "R0_C8";
    case 10:
      return "R0_C9";
    case 11:
      return "R0_C10";
    case 12:
      return "R1_C0";
    case 13:
      return "R1_C1";
    case 14:
      return "R1_C2";
    case 15:
      return "R1_C3";
    case 16:
      return "R1_C4";
    case 17:
      return "R1_C5";
    case 18:
      return "R1_C6";
    case 19:
      return "R1_C7";
    case 20:
      return "R1_C8";
    case 21:
      return "R1_C9";
    case 22:
      return "R1_C10";
    case 23:
      return "R2_C0";
    case 24:
      return "R2_C1";
    case 25:
      return "R2_C2";
    case 26:
      return "R2_C3";
    case 27:
      return "R2_C4";
    case 28:
      return "R2_C5";
    case 29:
      return "R2_C6";
    case 30:
      return "R2_C7";
    case 31:
      return "R2_C8";
    case 32:
      return "R2_C9";
    case 33:
      return "R2_C10";
    case 34:
      return "R3_C0";
    case 35:
      return "R3_C1";
    case 36:
      return "R3_C2";
    case 37:
      return "R3_C3";
    case 38:
      return "R3_C4";
    case 39:
      return "R3_C5";
    case 40:
      return "R3_C6";
    case 41:
      return "R3_C7";
    case 42:
      return "R3_C8";
    case 43:
      return "R3_C9";
    case 44:
      return "R3_C10";
    case 45:
      return "R4_C0";
    case 46:
      return "R4_C1";
    case 47:
      return "R4_C2";
    case 48:
      return "R4_C3";
    case 49:
      return "R4_C4";
    case 50:
      return "R4_C5";
    case 51:
      return "R4_C6";
    case 52:
      return "R4_C7";
    case 53:
      return "R4_C8";
    case 54:
      return "R4_C9";
    case 55:
      return "R4_C10";
    case 56:
      return "R5_C0";
    case 57:
      return "R5_C1";
    case 58:
      return "R5_C2";
    case 59:
      return "R5_C3";
    case 60:
      return "R5_C4";
    case 61:
      return "R5_C5";
    case 62:
      return "R5_C6";
    case 63:
      return "R5_C7";
    case 64:
      return "R5_C8";
    case 65:
      return "R5_C9";
    case 66:
      return "R5_C10";
    case 67:
      return "R6_C0";
    case 68:
      return "R6_C1";
    case 69:
      return "R6_C2";
    case 70:
      return "R6_C3";
    case 71:
      return "R6_C4";
    case 72:
      return "R6_C5";
    case 73:
      return "R6_C6";
    case 74:
      return "R6_C7";
    case 75:
      return "R6_C8";
    case 76:
      return "R6_C9";
    case 77:
      return "R6_C10";
    case 78:
      return "R7_C0";
    case 79:
      return "R7_C1";
    case 80:
      return "R7_C2";
    case 81:
      return "R7_C3";
    case 82:
      return "R7_C4";
    case 83:
      return "R7_C5";
    case 84:
      return "R7_C6";
    case 85:
      return "R7_C7";
    case 86:
      return "R7_C8";
    case 87:
      return "R7_C9";
    case 88:
      return "R7_C10";
    case 89:
      return "R0_GND";
    case 90:
      return "R1_GND";
    case 91:
      return "R2_GND";
    case 92:
      return "R3_GND";
    case 93:
      return "R4_GND";
    case 94:
      return "R5_GND";
    case 95:
      return "R6_GND";
    case 96:
      return "R7_GND";
    case 97:
      return "GPI1";
    case 98:
      return "GPI2";
    case 99:
      return "GPI3";
    case 100:
      return "GPI4";
    case 101:
      return "GPI5";
    case 102:
      return "GPI6";
    case 103:
      return "GPI7";
    case 104:
      return "GPI8";
    case 105:
      return "GPI9";
    case 106:
      return "GPI10";
    case 107:
      return "GPI11";
    case 108:
      return "GPI12";
    case 109:
      return "GPI13";
    case 110:
      return "GPI14";
    case 111:
      return "GPI15";
    case 112:
      return "GPI16";
    case 113:
      return "GPI17";
    case 114:
      return "GPI18";
    case 115:
      return "GPI19";
    case 116:
      return "LOGIC_1";
    case 117:
      return "LOGIC_2";
    case 118:
      return "UNUSED_0";
    case 119:
      return "UNUSED_1";
    case 120:
      return "UNUSED_2";
    case 121:
      return "UNUSED_3";
    case 122:
      return "UNUSED_4";
    case 123:
      return "UNUSED_5";
    case 124:
      return "UNUSED_6";
    case 125:
      return "UNUSED_7";
    case 126:
      return "UNUSED_8";
    case 127:
      return "UNLOCK_WILDCARD";
  }
  return "???";
}

FIFO::operator uint8_t() const {
  return (Event_State << 7) | static_cast<uint8_t>(IDENTIFIER);
}

PIN_CONFIG_A::operator uint8_t() const {
  return (static_cast<uint8_t>(R7_CONFIG) << 7) |
         (static_cast<uint8_t>(R6_CONFIG) << 6) |
         (static_cast<uint8_t>(R5_CONFIG) << 5) |
         (static_cast<uint8_t>(R4_CONFIG) << 4) |
         (static_cast<uint8_t>(R3_CONFIG) << 3) |
         (static_cast<uint8_t>(R2_CONFIG) << 2) |
         (static_cast<uint8_t>(R1_CONFIG) << 1) |
         (static_cast<uint8_t>(R0_CONFIG) << 0);
}

PIN_CONFIG_B::operator uint8_t() const {
  return (static_cast<uint8_t>(C7_CONFIG) << 7) |
         (static_cast<uint8_t>(C6_CONFIG) << 6) |
         (static_cast<uint8_t>(C5_CONFIG) << 5) |
         (static_cast<uint8_t>(C4_CONFIG) << 4) |
         (static_cast<uint8_t>(C3_CONFIG) << 3) |
         (static_cast<uint8_t>(C2_CONFIG) << 2) |
         (static_cast<uint8_t>(C1_CONFIG) << 1) |
         (static_cast<uint8_t>(C0_CONFIG) << 0);
}

PIN_CONFIG_C::operator uint8_t() const {
  return (static_cast<uint8_t>(C10_CONFIG) << 2) |
         (static_cast<uint8_t>(C9_CONFIG) << 1) |
         (static_cast<uint8_t>(C8_CONFIG) << 0);
}

PIN_CONFIG_D::operator uint8_t() const {
  return (static_cast<uint8_t>(PULL_SELECT) << 7) |
         (static_cast<uint8_t>(C4_EXTEND_CFG) << 6) |
         (static_cast<uint8_t>(R4_EXTEND_CFG) << 5) |
         (static_cast<uint8_t>(C6_EXTEND_CFG) << 4) |
         (static_cast<uint8_t>(R3_EXTEND_CFG) << 2) |
         (static_cast<uint8_t>(C9_EXTEND_CFG) << 1) |
         (static_cast<uint8_t>(R0_EXTEND_CFG) << 0);
}

GENERAL_CFG_B::operator uint8_t() const {
  return (static_cast<uint8_t>(OSC_EN) << 7) |
         (static_cast<uint8_t>(CORE_FREQ) << 5) |
         (static_cast<uint8_t>(LCK_TRK_LOGIC) << 4) |
         (static_cast<uint8_t>(LCK_TRK_GPI) << 3) |
         (static_cast<uint8_t>(INT_CFG) << 1) |
         (static_cast<uint8_t>(RST_CFG) << 0);
}

INT_EN::operator uint8_t() const {
  return (static_cast<uint8_t>(LOGIC2_IEN) << 5) |
         (static_cast<uint8_t>(LOGIC1_IEN) << 4) |
         (static_cast<uint8_t>(LOCK_IEN) << 3) |
         (static_cast<uint8_t>(OVRFLOW_IEN) << 2) |
         (static_cast<uint8_t>(GPI_IEN) << 1) |
         (static_cast<uint8_t>(EVENT_IEN) << 0);
}

INT_STATUS::operator uint8_t() const {
  return (static_cast<uint8_t>(LOGIC2_INT) << 5) |
         (static_cast<uint8_t>(LOGIC1_INT) << 4) |
         (static_cast<uint8_t>(LOCK_INT) << 3) |
         (static_cast<uint8_t>(OVRFLOW_INT) << 2) |
         (static_cast<uint8_t>(GPI_INT) << 1) |
         (static_cast<uint8_t>(EVENT_INT) << 0);
}

}  // namespace Register
}  // namespace adp5589
