#pragma once

#include <cstdint>

namespace adp5589 {

/**
 * Definitions for the ADP5589 keyboard IC.
 *
 * Datasheet:
 * https://www.analog.com/media/en/technical-documentation/data-sheets/ADP5589.pdf
 */

// clang-format off
enum class Register : uint8_t {
  ID                 = 0x00,
  INT_STATUS         = 0x01,
  Status             = 0x02,
  FIFO_1             = 0x03,
  FIFO_2             = 0x04,
  FIFO_3             = 0x05,
  FIFO_4             = 0x06,
  FIFO_5             = 0x07,
  FIFO_6             = 0x08,
  FIFO_7             = 0x09,
  FIFO_8             = 0x0A,
  FIFO_9             = 0x0B,
  FIFO_10            = 0x0C,
  FIFO_11            = 0x0D,
  FIFO_12            = 0x0E,
  FIFO_13            = 0x0F,
  FIFO_14            = 0x10,
  FIFO_15            = 0x11,
  FIFO_16            = 0x12,
  GPI_INT_STAT_A     = 0x13,
  GPI_INT_STAT_B     = 0x14,
  GPI_INT_STAT_C     = 0x15,
  GPI_STATUS_A       = 0x16,
  GPI_STATUS_B       = 0x17,
  GPI_STATUS_C       = 0x18,
  RPULL_CONFIG_A     = 0x19,
  RPULL_CONFIG_B     = 0x1A,
  RPULL_CONFIG_C     = 0x1B,
  RPULL_CONFIG_D     = 0x1C,
  RPULL_CONFIG_E     = 0x1D,
  GPI_INT_LEVEL_A    = 0x1E,
  GPI_INT_LEVEL_B    = 0x1F,
  GPI_INT_LEVEL_C    = 0x20,
  GPI_EVENT_EN_A     = 0x21,
  GPI_EVENT_EN_B     = 0x22,
  GPI_EVENT_EN_C     = 0x23,
  GPI_INTERRUPT_EN_A = 0x24,
  GPI_INTERRUPT_EN_B = 0x25,
  GPI_INTERRUPT_EN_C = 0x26,
  DEBOUNCE_DIS_A     = 0x27,
  DEBOUNCE_DIS_B     = 0x28,
  DEBOUNCE_DIS_C     = 0x29,
  GPO_DATA_OUT_A     = 0x2A,
  GPO_DATA_OUT_B     = 0x2B,
  GPO_DATA_OUT_C     = 0x2C,
  GPO_OUT_MODE_A     = 0x2D,
  GPO_OUT_MODE_B     = 0x2E,
  GPO_OUT_MODE_C     = 0x2F,
  GPIO_DIRECTION_A   = 0x30,
  GPIO_DIRECTION_B   = 0x31,
  GPIO_DIRECTION_C   = 0x32,
  UNLOCK1            = 0x33,
  UNLOCK2            = 0x34,
  EXT_LOCK_EVENT     = 0x35,
  UNLOCK_TIMERS      = 0x36,
  LOCK_CFG           = 0x37,
  RESET1_EVENT_A     = 0x38,
  RESET1_EVENT_B     = 0x39,
  RESET1_EVENT_C     = 0x3A,
  RESET2_EVENT_A     = 0x3B,
  RESET2_EVENT_B     = 0x3C,
  RESET2_EVENT_C     = 0x3D,
  PWM_OFFT_LOW       = 0x3E,
  PWM_OFFT_HIGH      = 0x3F,
  PWM_ONT_LOW        = 0x40,
  PWM_ONT_HIGH       = 0x41,
  PWM_CFG            = 0x42,
  CLOCK_DIV_CFG      = 0x43,
  LOGIC_1_CFG        = 0x44,
  LOGIC_2_CFG        = 0x45,
  LOGIC_FF_CFG       = 0x46,
  LOGIC_INT_EVENT_EN = 0x47,
  POLL_TIME_CFG      = 0x48,
  PIN_CONFIG_A       = 0x49,
  PIN_CONFIG_B       = 0x4A,
  PIN_CONFIG_C       = 0x4B,
  PIN_CONFIG_D       = 0x4C,
  GENERAL_CFG_B      = 0x4D,
  INT_EN             = 0x4E,
};
// clang-format on

struct Register_ID {
  uint8_t MAN : 4;
  uint8_t REV : 4;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_INT_STATUS {
  uint8_t Reserved : 2;
  /**
   * Logic 2 interrupt condition.
   *
   * 0 = no interrupt.
   * 1 = interrupt due to a general Logic 2 condition. Write a 1 to this bit to
   * clear it.
   */
  uint8_t LOGIC2_INT : 1;
  /**
   * Logic 1 interrupt condition..
   *
   * 0 = no interrupt.
   * 1 = interrupt due to a general Logic 1 condition. Write a 1 to this bit to
   * clear it.
   */
  uint8_t LOGIC1_INT : 1;
  /**
   * 0 = no interrupt.
   * 1 = interrupt due to a lock/unlock condition.
   *
   * The user can read LOCK_STAT (0x02[5]) to determine if LOCK_INT is due to a
   * lock or unlock event. If LOCK_STAT = 1, LOCK_INT is due to a lock event. If
   * LOCK_STAT = 0, LOCK_INT is due to an unlock event. Write a 1 to this bit to
   * clear it. If lock mode is enabled via the software bit LOCK_EN (0x37[0]), a
   * LOCK_INT is not generated because the processor knows it just enabled lock
   * mode. If lock mode is disabled (while locked) via the software bit LOCK_EN,
   * a LOCK_INT is not generated because the processor knows it just disabled
   * lock mode
   */
  uint8_t LOCK_INT : 1;

  /**
   *
   * Overflow interrupt condition.
   *
   * 0 = no interrupt.
   * 1 = interrupt due to an overflow condition.
   *
   * Write a 1 to this bit to clear it.
   */
  uint8_t OVRFLOW_INT : 1;

  /**
   *
   * 0 = no interrupt.
   * 1 = interrupt due to a general GPI condition.
   *
   * This bit is not set by a GPI that has been configured to update the FIFO
   * and event count. Write a 1 to this bit to clear it. This bit cannot be
   * cleared until all GPI_x_INT bits are cleared.
   */
  uint8_t GPI_INT : 1;

  /**
   * 0 = no interrupt.
   * 1 = interrupt due to key event (press/release), GPI event (GPI programmed
   * for FIFO updates), or Logic 1/Logic 2 event (programmed for FIFO
   * updates). Write a 1 to this bit to clear it.
   */
  uint8_t EVENT_INT : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_Status {
  uint8_t LOGIC2_STAT : 1;
  uint8_t LOGIC1_STAT : 1;
  uint8_t LOCK_STAT : 1;
  uint8_t EC : 5;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_FIFO {
  uint8_t EventState : 1;
  uint8_t IDENTIFIER : 7;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_UINT8 {
  uint8_t val;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_UINT8_5_3 {
  uint8_t Reserved : 5;
  uint8_t val : 3;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_GPI_INT_STAT_A = Register_UINT8;
using Register_GPI_INT_STAT_B = Register_UINT8;
using Register_GPI_INT_STAT_C = Register_UINT8_5_3;

using Register_RPULL_CONFIG_A = Register_UINT8;
using Register_RPULL_CONFIG_B = Register_UINT8;
using Register_RPULL_CONFIG_C = Register_UINT8;
using Register_RPULL_CONFIG_D = Register_UINT8;

struct Register_GPI_INT_STAT_E {
  uint8_t Reserved : 2;
  uint8_t val : 6;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_GPI_INT_LEVEL_A = Register_UINT8;
using Register_GPI_INT_LEVEL_B = Register_UINT8;
using Register_GPI_INT_LEVEL_C = Register_UINT8_5_3;

using Register_GPI_EVENT_EN_A = Register_UINT8;
using Register_GPI_EVENT_EN_B = Register_UINT8;
using Register_GPI_EVENT_EN_C = Register_UINT8_5_3;

using Register_GPI_INTERRUPT_EN_A = Register_UINT8;
using Register_GPI_INTERRUPT_EN_B = Register_UINT8;
using Register_GPI_INTERRUPT_EN_C = Register_UINT8_5_3;

using Register_DEBOUNCE_DIS_A = Register_UINT8;
using Register_DEBOUNCE_DIS_B = Register_UINT8;
using Register_DEBOUNCE_DIS_C = Register_UINT8_5_3;

using Register_GPO_DATA_OUT_A = Register_UINT8;
using Register_GPO_DATA_OUT_B = Register_UINT8;
using Register_GPO_DATA_OUT_C = Register_UINT8_5_3;

using Register_GPO_OUT_MODE_A = Register_UINT8;
using Register_GPO_OUT_MODE_B = Register_UINT8;
using Register_GPO_OUT_MODE_C = Register_UINT8_5_3;

using Register_GPIO_DIRECTION_A = Register_UINT8;
using Register_GPIO_DIRECTION_B = Register_UINT8;
using Register_GPIO_DIRECTION_C = Register_UINT8_5_3;

struct Register_UNLOCK {
  uint8_t UNLOCK_STATE : 1;
  uint8_t UNLOCK : 7;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_UNLOCK1 = Register_UNLOCK;
using Register_UNLOCK2 = Register_UNLOCK;

struct Register_EXT_LOCK_EVENT {
  uint8_t EXT_LOCK_STATE : 1;
  uint8_t EXT_LOCK_EVENT : 7;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_UNLOCK_TIMERS {
  uint8_t INT_MASK_TIMER : 5;
  uint8_t UNLOCK_TIMER : 3;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_LOCK_CFG {
  uint8_t Reserved : 7;
  uint8_t LOCK_EN : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_RESET_EVENT {
  uint8_t RESET_EVENT_Level : 1;
  uint8_t RESET_EVENT : 7;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_RESET1_EVENT_A = Register_RESET_EVENT;
using Register_RESET1_EVENT_B = Register_RESET_EVENT;
using Register_RESET1_EVENT_C = Register_RESET_EVENT;
using Register_RESET2_EVENT_A = Register_RESET_EVENT;
using Register_RESET2_EVENT_B = Register_RESET_EVENT;

struct Register_RESET2_EVENT_C {
  uint8_t RESET2_POL : 1;
  uint8_t RESET1_POL : 1;
  uint8_t RST_PASSTHRU_EN : 1;
  uint8_t RESET_TRIGGER_TIME : 3;
  uint8_t RESET_PULSE_WIDTH : 2;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_PWM_OFFT_LOW = Register_UINT8;
using Register_PWM_OFFT_HIGH = Register_UINT8;
using Register_PWM_ONT_LOW = Register_UINT8;
using Register_PWM_ONT_HIGH = Register_UINT8;

struct Register_PWM_CFG {
  uint8_t Reserved : 5;
  uint8_t PWM_IN_AND : 1;
  uint8_t PWM_MODE : 1;
  uint8_t PWM_EN : 3;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_CLOCK_DIV_CFG {
  uint8_t Reserved : 1;
  uint8_t CLK_INV : 1;
  uint8_t CLK_DIV : 5;
  uint8_t CLK_DIV_EN : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_LOGIC_CFG {
  uint8_t Reserved : 1;
  uint8_t LY_INV : 1;
  uint8_t LC_INV : 1;
  uint8_t LB_INV : 1;
  uint8_t LA_INV : 1;
  uint8_t LOGIC_SEL : 3;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_LOGIC_1_CFG = Register_LOGIC_CFG;
using Register_LOGIC_2_CFG = Register_LOGIC_CFG;

struct Register_LOGIC_FF_CFG {
  uint8_t Reserved : 4;
  uint8_t FF2_SET : 1;
  uint8_t FF2_CLR : 1;
  uint8_t FF1_SET : 1;
  uint8_t FF1_CLR : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_LOGIC_INT_EVENT_EN {
  uint8_t Reserved : 2;
  uint8_t LY2_DBNC_DIS : 1;
  uint8_t LOGIC2_EVENT_EN : 1;
  uint8_t LOGIC2_INT_LEVEL : 1;
  uint8_t LY1_DBNC_DIS : 1;
  uint8_t LOGIC1_EVENT_EN : 1;
  uint8_t LOGIC1_INT_LEVEL : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_POLL_TIME_CFG {
  uint8_t Reserved : 6;
  uint8_t KEY_POLL_TIME : 2;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_PIN_CONFIG_A = Register_UINT8;
using Register_PIN_CONFIG_B = Register_UINT8;
using Register_PIN_CONFIG_C = Register_UINT8_5_3;

struct Register_PIN_CONFIG_D {
  uint8_t PULL_SELECT : 1;
  uint8_t C4_EXTEND_CFG : 1;
  uint8_t R4_EXTEND_CFG : 1;
  uint8_t C6_EXTEND_CFG : 1;
  uint8_t R3_EXTEND_CFG : 2;
  uint8_t C9_EXTEND_CFG : 1;
  uint8_t R0_EXTEND_CFG : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_GENERAL_CFG_B {
  /**
   * Enable internal 1 MHz oscillator.
   *
   * 0 = disabled
   * 1 = enabled.
   */
  uint8_t OSC_EN : 1;

  /**
   * Sets the input clock frequency fed from the base 1 MHz oscillator
   * to the digital core. Slower frequencies result in less IDD. However,
   * key and GPI scan times increase.
   *
   * 0b00 = 50 kHz.
   * 0b00 = 100 kHz.
   * 0b00 = 200 kHz.
   * 0b00 = 500 kHz
   */
  uint8_t CORE_FREQ : 2;

  // 0 = allow logic outputs (programmed for FIFO updates) to be tracked on
  // the FIFO if the keypad is locked.
  // 1 = do not track.
  uint8_t LCK_TRK_LOGIC : 1;

  // 0 = allow GPIs (programmed for FIFO updates) to be tracked on the FIFO
  // if the keypad is locked.
  // 1 = do not track.
  uint8_t LCK_TRK_GPI : 1;
  uint8_t Unused : 1;

  /**
   * Configure the behavior of the INT pin if the user tries to clear it while
   * an interrupt is pending.
   *
   * 0 = INT pin remains asserted if an interrupt is pending.
   * 1 = INT pin deasserts for 50 µs and reasserts if an interrupt is pending.
   */
  uint8_t INT_CFG : 1;

  /**
   * Configure the response ADP5589 has to the RST pin.
   *
   * 0 = ADP5589 resets if RST is low.
   * 1 = ADP5589 does not reset if RST is low.
   */
  uint8_t RST_CFG : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_INT_EN {
  uint8_t Reserved : 2;
  /**
   * Logic 2 interrupt.
   *
   * 0 = Disabled.
   * 1 = Assert the INT pin if LOGIC2_INT is set.
   */
  uint8_t LOGIC2_IEN : 1;

  /**
   * Logic 1 interrupt.
   *
   * 0 = Disabled.
   * 1 = Assert the INT pin if LOGIC1_INT is set.
   */
  uint8_t LOGIC1_IEN : 1;

  /**
   * 0 = lock interrupt is disabled.
   * 1 = assert the INT pin if LOCK_INT is set.
   */
  uint8_t LOCK_IEN : 1;

  /**
   * 0 = overflow interrupt is disabled.
   * 1 = assert the INT pin if OVRFLOW_INT is set.
   */
  uint8_t OVRFLOW_IEN : 1;

  /**
   * 0 = GPI interrupt is disabled.
   * 1 = assert the INT pin if GPI_INT is set.
   */
  uint8_t GPI_IEN : 1;

  /**
   * 0 = event interrupt is disabled.
   * 1 = assert the INT pin if EVENT_INT is set.
   */
  uint8_t EVENT_IEN : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

}  // namespace adp5589
