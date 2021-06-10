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
  uint8_t MAN : 4;  // Manufacturer.
  uint8_t REV : 4;  // Hardware revision.

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
  /**
   * 0 = output from Logic Block 2. (LY2) is low.
   * 1 = output from Logic Block 2. (LY2) is high.
   */
  uint8_t LOGIC2_STAT : 1;

  /**
   * 0 = output from Logic Block 1 (LY1) is low.
   * 1 = output from Logic Block 1 (LY1) is high.
   */
  uint8_t LOGIC1_STAT : 1;

  /**
   * 0 = unlocked.
   * 1 = locked.
   */
  uint8_t LOCK_STAT : 1;

  /**
   * Event count value.
   *
   * Indicates how many events are currently stored on the FIFO.
   */
  uint8_t EC : 5;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

/**
 * FIFO event ID from datasheet table 11.
 */
enum class EventID : uint8_t {
  NONE = 0,
  R0_C0 = 1,
  R0_C1 = 2,
  R0_C2 = 3,
  R0_C3 = 4,
  R0_C4 = 5,
  R0_C5 = 6,
  R0_C6 = 7,
  R0_C7 = 8,
  R0_C8 = 9,
  R0_C9 = 10,
  R0_C10 = 11,
  R1_C0 = 12,
  R1_C1 = 13,
  R1_C2 = 14,
  R1_C3 = 15,
  R1_C4 = 16,
  R1_C5 = 17,
  R1_C6 = 18,
  R1_C7 = 19,
  R1_C8 = 20,
  R1_C9 = 21,
  R1_C10 = 22,
  R2_C0 = 23,
  R2_C1 = 24,
  R2_C2 = 25,
  R2_C3 = 26,
  R2_C4 = 27,
  R2_C5 = 28,
  R2_C6 = 29,
  R2_C7 = 30,
  R2_C8 = 31,
  R2_C9 = 32,
  R2_C10 = 33,
  R3_C0 = 34,
  R3_C1 = 35,
  R3_C2 = 36,
  R3_C3 = 37,
  R3_C4 = 38,
  R3_C5 = 39,
  R3_C6 = 40,
  R3_C7 = 41,
  R3_C8 = 42,
  R3_C9 = 43,
  R3_C10 = 44,
  R4_C0 = 45,
  R4_C1 = 46,
  R4_C2 = 47,
  R4_C3 = 48,
  R4_C4 = 49,
  R4_C5 = 50,
  R4_C6 = 51,
  R4_C7 = 52,
  R4_C8 = 53,
  R4_C9 = 54,
  R4_C10 = 55,
  R5_C0 = 56,
  R5_C1 = 57,
  R5_C2 = 58,
  R5_C3 = 59,
  R5_C4 = 60,
  R5_C5 = 61,
  R5_C6 = 62,
  R5_C7 = 63,
  R5_C8 = 64,
  R5_C9 = 65,
  R5_C10 = 66,
  R6_C0 = 67,
  R6_C1 = 68,
  R6_C2 = 69,
  R6_C3 = 70,
  R6_C4 = 71,
  R6_C5 = 72,
  R6_C6 = 73,
  R6_C7 = 74,
  R6_C8 = 75,
  R6_C9 = 76,
  R6_C10 = 77,
  R7_C0 = 78,
  R7_C1 = 79,
  R7_C2 = 80,
  R7_C3 = 81,
  R7_C4 = 82,
  R7_C5 = 83,
  R7_C6 = 84,
  R7_C7 = 85,
  R7_C8 = 86,
  R7_C9 = 87,
  R7_C10 = 88,
  R0_GND = 89,
  R1_GND = 90,
  R2_GND = 91,
  R3_GND = 92,
  R4_GND = 93,
  R5_GND = 94,
  R6_GND = 95,
  R7_GND = 96,
  GPI1 = 97,    // (ROW 0)
  GPI2 = 98,    // (ROW 1)
  GPI3 = 99,    // (ROW 2)
  GPI4 = 100,   // (ROW 3)
  GPI5 = 101,   // (ROW 4)
  GPI6 = 102,   // (ROW 5)
  GPI7 = 103,   // (ROW 6)
  GPI8 = 104,   // (ROW 7)
  GPI9 = 105,   // (COL 0)
  GPI10 = 106,  // (COL 1)
  GPI11 = 107,  // (COL 2)
  GPI12 = 108,  // (COL 3)
  GPI13 = 109,  // (COL 4)
  GPI14 = 110,  // (COL 5)
  GPI15 = 111,  // (COL 6)
  GPI16 = 112,  // (COL 7)
  GPI17 = 113,  // (COL 8)
  GPI18 = 114,  // (COL 9)
  GPI19 = 115,  // (COL 10)
  LOGIC_1 = 116,
  LOGIC_2 = 117,
  UNUSED_0 = 118,
  UNUSED_1 = 119,
  UNUSED_2 = 120,
  UNUSED_3 = 121,
  UNUSED_4 = 122,
  UNUSED_5 = 123,
  UNUSED_6 = 124,
  UNUSED_7 = 125,
  UNUSED_8 = 126,
  UNLOCK_WILDCARD = 127,
};

/**
 * FIFO1 - FIFO16.
 */
struct Register_FIFO {
  /**
   * Represents the state of the event that is recorded in IDENTIFIER.
   *
   * For key events (Event 1 to Event 96).
   * 1 = key is pressed.
   * 0 = key is released.
   *
   * For GPI and logic events (Event 97 to Event 117).
   * 1 = GPI/logic is active.
   * 0 = GPI/logic is inactive.
   *
   * Active and inactive states are programmable.
   */
  uint8_t Event_State : 1;

  /**
   * The event identifier.
   */
  EventID IDENTIFIER : 7;

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

struct Register_GPI_INT_STAT_A {
  uint8_t GPI_8_INT : 1;
  uint8_t GPI_7_INT : 1;
  uint8_t GPI_6_INT : 1;
  uint8_t GPI_5_INT : 1;
  uint8_t GPI_4_INT : 1;
  uint8_t GPI_3_INT : 1;
  uint8_t GPI_2_INT : 1;
  uint8_t GPI_1_INT : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_GPI_INT_STAT_B {
  uint8_t GPI_16_INT : 1;
  uint8_t GPI_15_INT : 1;
  uint8_t GPI_14_INT : 1;
  uint8_t GPI_13_INT : 1;
  uint8_t GPI_12_INT : 1;
  uint8_t GPI_11_INT : 1;
  uint8_t GPI_10_INT : 1;
  uint8_t GPI_9_INT : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_GPI_INT_STAT_C {
  uint8_t Reserved : 5;
  uint8_t GPI_19_INT : 1;
  uint8_t GPI_18_INT : 1;
  uint8_t GPI_17_INT : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_GPI_STATUS_A {
  uint8_t GPI_8_STAT : 1;
  uint8_t GPI_7_STAT : 1;
  uint8_t GPI_6_STAT : 1;
  uint8_t GPI_5_STAT : 1;
  uint8_t GPI_4_STAT : 1;
  uint8_t GPI_3_STAT : 1;
  uint8_t GPI_2_STAT : 1;
  uint8_t GPI_1_STAT : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_GPI_STATUS_B {
  uint8_t GPI_16_STAT : 1;
  uint8_t GPI_15_STAT : 1;
  uint8_t GPI_14_STAT : 1;
  uint8_t GPI_13_STAT : 1;
  uint8_t GPI_12_STAT : 1;
  uint8_t GPI_11_STAT : 1;
  uint8_t GPI_10_STAT : 1;
  uint8_t GPI_9_STAT : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_GPI_STATUS_C {
  uint8_t Reserved : 5;
  uint8_t GPI_19_STAT : 1;
  uint8_t GPI_18_STAT : 1;
  uint8_t GPI_17_STAT : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

// clang-format off
enum class PullupConfig : uint8_t {
  k300Up    = 0b00,  // enable 300 kΩ pull-up.
  k300Down  = 0b01,  // enable 300 kΩ pull-down.
  k100Up    = 0b10,  // enable 100 kΩ pull-up.
  kDisabled = 0b11,  // disable all pull-up/pull-down resistors
};
// clang-format on

struct Register_RPULL_CONFIG_A {
  PullupConfig R3_PULL_CFG : 2;
  PullupConfig R2_PULL_CFG : 2;
  PullupConfig R1_PULL_CFG : 2;
  PullupConfig R0_PULL_CFG : 2;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_RPULL_CONFIG_B {
  PullupConfig R7_PULL_CFG : 2;
  PullupConfig R6_PULL_CFG : 2;
  PullupConfig R5_PULL_CFG : 2;
  PullupConfig R4_PULL_CFG : 2;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_RPULL_CONFIG_C {
  PullupConfig C3_PULL_CFG : 2;
  PullupConfig C2_PULL_CFG : 2;
  PullupConfig C1_PULL_CFG : 2;
  PullupConfig C0_PULL_CFG : 2;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_RPULL_CONFIG_D {
  PullupConfig C7_PULL_CFG : 2;
  PullupConfig C6_PULL_CFG : 2;
  PullupConfig C5_PULL_CFG : 2;
  PullupConfig C4_PULL_CFG : 2;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_RPULL_CONFIG_E {
  uint8_t Reserved : 2;
  PullupConfig C10_PULL_CFG : 2;
  PullupConfig C9_PULL_CFG : 2;
  PullupConfig C8_PULL_CFG : 2;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

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
  /**
   * Defines which state the first unlock event should be.
   *
   * For key events:
   * 0 = not applicable; releases not used for unlock.
   * 1 = press is used as unlock event.
   *
   * For GPIs and logic outputs configured for FIFO updates:
   * 0 = inactive event used as reset condition.
   * 1 = active event used as reset condition.
   */
  uint8_t UNLOCK_STATE : 1;

  /**
   * Defines the first event that must be detected to unlock the keypad after
   * LOCK_EN has been set.
   */
  uint8_t UNLOCK : 7;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_UNLOCK1 = Register_UNLOCK;
using Register_UNLOCK2 = Register_UNLOCK;

struct Register_EXT_LOCK_EVENT {
  /**
   * Defines which state the lock event should be.
   *
   * For key events:
   * 0 = not applicable; releases not used for unlock.
   * 1 = press is used as unlock event.
   *
   * For GPIs and logic outputs configured for FIFO updates:
   * 0 = inactive event used as reset condition.
   * 1 = active event used as reset condition.
   */
  uint8_t EXT_LOCK_STATE : 1;

  /**
   * Defines an event that can lock the keypad.
   *
   * When this event is detected, LOCK_INT is set.
   */
  uint8_t EXT_LOCK_EVENT : 7;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_UNLOCK_TIMERS {
  /**
   * If the keypad is locked and this timer is set, any key event (or GPI/logic
   * event programmed to FIFO update) is allowed to generate an EVENT_INT
   * interrupt. This timer then begins counting, and no further events generate
   * an interrupt until this timer has expired (or both unlock events have
   * occurred).
   *
   * Value = number of seconds with 0 = disabled. [0..31].
   */
  uint8_t INT_MASK_TIMER : 5;

  /**
   * Defines the time in which the second unlock event must occur after the
   * first unlock event has occurred. If the second unlock event does not occur
   * within this time (or any other event occurs), the keypad goes back to full
   * lock mode.
   *
   * Range: [0..7] seconds where 0=disabled.
   */
  uint8_t UNLOCK_TIMER : 3;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_LOCK_CFG {
  uint8_t Reserved : 7;
  /**
   * Enable the lock function.
   */
  uint8_t LOCK_EN : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_RESET_EVENT {
  /**
   * Defines which level the first reset event should be.
   *
   * For key events:
   * 0 = not applicable; releases not used for reset generation.
   * 1 = press is used as reset event.
   *
   * For GPIs and logic outputs configured for FIFO updates:
   * 0 = inactive event used as reset condition.
   * 1 = active event used as reset condition.
   */
  uint8_t RESET_EVENT_Level : 1;

  /**
   * Defines an event that can be used to generate the RESET1 signal.
   * Up to three events can be defined for generating the RESET1 signal, using
   * RESET1_EVENT_A[6:0], RESET1_EVENT_B[6:0], and RESET1_EVENT_C[6:0].
   * If one of the registers is 0, that register is not used for reset
   * generation. All reset events must be detected at the same time to trigger
   * the reset.
   */
  uint8_t RESET_EVENT : 7;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_RESET1_EVENT_A = Register_RESET_EVENT;
using Register_RESET1_EVENT_B = Register_RESET_EVENT;
using Register_RESET1_EVENT_C = Register_RESET_EVENT;
using Register_RESET2_EVENT_A = Register_RESET_EVENT;
using Register_RESET2_EVENT_B = Register_RESET_EVENT;

enum class PulseWidth : uint8_t {
  k500us = 0b00,
  k1ms = 0b01,
  k2ms = 0b10,
  k10ms = 0b11,
};

enum class ResetTime : uint8_t {
  kImmediate = 0b000,
  k1_0s = 0b001,  // 1.0 sec.
  k1_5s = 0b010,  // 1.5 sec.
  k2_0s = 0b011,  // 2.0 sec.
  k2_5s = 0b100,  // 2.5 sec.
  k3_0s = 0b101,  // 3.0 sec.
  k3_5s = 0b110,  // 3.5 sec.
  k4_0s = 0b111,  // 4.0 sec.
};

struct Register_RESET2_EVENT_C {
  /**
   * Sets the polarity of RESET2.
   *
   * 0 = RESET2 is active low.
   * 1 = RESET2 is active high.
   */
  uint8_t RESET2_POL : 1;

  /**
   * Sets the polarity of RESET1.
   *
   * 0 = RESET2 is active low.
   * 1 = RESET2 is active high.
   */
  uint8_t RESET1_POL : 1;

  /**
   * Allows the RST pin to override (OR with) the RESET1 signal.
   * Function not applicable to RESET2.
   */
  uint8_t RST_PASSTHRU_EN : 1;

  /**
   * Defines the length of time that the reset events must be active before a
   * reset signal is generated.
   *
   * All events must be active at the same time for the same duration. Parameter
   * common to both RESET1 and RESET2.
   */
  ResetTime RESET_TRIGGER_TIME : 3;

  /**
   * Defines the pulse width of the reset signals.
   *
   * Parameter common to both RESET1 and RESET2.
   */
  PulseWidth RESET_PULSE_WIDTH : 2;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_PWM_OFFT_LOW = Register_UINT8;
using Register_PWM_OFFT_HIGH = Register_UINT8;
using Register_PWM_ONT_LOW = Register_UINT8;
using Register_PWM_ONT_HIGH = Register_UINT8;

struct Register_PWM_CFG {
  uint8_t Reserved : 5;

  /**
   * AND the internally generated PWM signal with an externally supplied PWM
   * signal (C6).
   */
  uint8_t PWM_IN_AND : 1;

  /**
   * Defines PWM mode.
   *
   * 0 = continuous.
   * 1 = one shot.
   *
   * If a one-shot is performed, the PWM_EN bit is automatically cleared.
   * If a second one-shot must be performed, the user must set PWM_EN again
   **/
  uint8_t PWM_MODE : 1;

  /**
   * Enable PWM generator.
   */
  uint8_t PWM_EN : 3;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_CLOCK_DIV_CFG {
  uint8_t Reserved : 1;

  /**
   * Inverts the divided down clock signal.
   */
  bool CLK_INV : 1;

  /**
   * Defines the divide down scale of the externally supplied clock.
   *
   * 0b00000 = divide by 1 (pass-through).
   * 0b00001 = divide by 2.
   * 0b00010 = divide by 3.
   * 0b00011 = divide by 4.
   * ...
   * 0b11111 = divide by 32.
   */
  uint8_t CLK_DIV : 5;

  /**
   * Enables the clock divider circuit to divide down the externally supplied
   * clock signal.
   */
  bool CLK_DIV_EN : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

// clang-format off
enum class MuxState : uint8_t {
  kOff   = 0b000,  // Off/disable.
  kAND   = 0b001,
  kOR    = 0b010,
  kXOR   = 0b011,
  kFF    = 0b100,
  kIN_LA = 0b101,
  kIN_LB = 0b110,
  kIN_LC = 0b111,
};
// clang-format on

struct Register_LOGIC_CFG {
  uint8_t Reserved : 1;
  /**
   * 0 = LYn output not inverted before passing into Logic Block n.
   * 1 = inverts output LYn from Logic Block n.
   */
  bool LY_INV : 1;

  /**
   * 0 = LCn input not inverted before passing into Logic Block n.
   * 1 = inverts input LCn before passing it into Logic Block n.
   */
  bool LC_INV : 1;

  /**
   * 0 = LBn input not inverted before passing into Logic Block n.
   * 1 = inverts input LBn before passing it into Logic Block n.
   */
  bool LB_INV : 1;

  /**
   * 0 = LAn input not inverted before passing into Logic Block n.
   * 1 = inverts input LAn before passing it into Logic Block n.
   */
  bool LA_INV : 1;

  /**
   * Configures the digital mux for Logic Block n.
   */
  MuxState LOGIC_SEL : 3;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

using Register_LOGIC_1_CFG = Register_LOGIC_CFG;
using Register_LOGIC_2_CFG = Register_LOGIC_CFG;

struct Register_LOGIC_FF_CFG {
  uint8_t Reserved : 4;

  /**
   * 0 = FF2 not set in Logic Block 2.
   * 1 = set FF2 in Logic Block 2.
   */
  bool FF2_SET : 1;

  /**
   * 0 = FF2 not cleared in Logic Block 2.
   * 1 = clear FF2 in Logic Block 2.
   */
  bool FF2_CLR : 1;

  /**
   * 0 = FF1 not set in Logic Block 1.
   * 1 = set FF1 in Logic Block 1.
   */
  bool FF1_SET : 1;

  /**
   * 0 = FF1 not cleared in Logic Block 1.
   * 1 = clear FF1 in Logic Block 1.
   */
  bool FF1_CLR : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_LOGIC_INT_EVENT_EN {
  uint8_t Reserved : 2;

  /**
   * Logic Block 2 output debounce disable.
   *
   * 0 = output of Logic Block 2 is debounced before entering the
   * event/interrupt block.
   *
   * 1 = output of Logic Block 2 is not debounced before entering the
   * event/interrupt block. Use with caution because glitches may generate
   * interrupts prematurely.
   */
  bool LY2_DBNC_DIS : 1;

  /**
   * LY2 activity interrupt generation.
   *
   * 0 = LY2 cannot generate interrupt.
   * 1 = allow LY2 activity to generate events on the FIFO.
   */
  bool LOGIC2_EVENT_EN : 1;

  /**
   * LY2 logic level interrupt generation type.
   *
   * 0 = LY2 is active low.
   * 1 = LY2 is active high.
   */
  uint8_t LOGIC2_INT_LEVEL : 1;

  /**
   * Logic Block 1 output debounce disable.
   *
   * 0 = output of Logic Block 1 is debounced before entering the
   * event/interrupt block.
   *
   * 1 = output of Logic Block 1 is not debounced before entering the
   * event/interrupt block. Use with caution because glitches may generate
   * interrupts prematurely.
   */
  bool LY1_DBNC_DIS : 1;

  /**
   * LY1 activity interrupt generation.
   *
   * 0 = LY1 cannot generate interrupt.
   * 1 = allow LY1 activity to generate events on the FIFO.
   */
  bool LOGIC1_EVENT_EN : 1;

  /**
   * Configure the logic level of LY1 that generates an interrupt.
   *
   * 0 = LY1 is active low.
   * 1 = LY1 is active high.
   */
  uint8_t LOGIC1_INT_LEVEL : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

enum class PollTime : uint8_t {
  k10ms = 0b00,
  k20ms = 0b01,
  k30ms = 0b10,
  k40ms = 0b11,
};

struct Register_POLL_TIME_CFG {
  uint8_t Reserved : 6;
  /**
   * Configure time between consecutive scan cycles.
   */
  PollTime KEY_POLL_TIME : 2;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_PIN_CONFIG_A {
  uint8_t R7_CONFIG : 1;
  uint8_t R6_CONFIG : 1;
  uint8_t R5_CONFIG : 1;
  uint8_t R4_CONFIG : 1;
  uint8_t R3_CONFIG : 1;
  uint8_t R2_CONFIG : 1;
  uint8_t R1_CONFIG : 1;
  uint8_t R0_CONFIG : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_PIN_CONFIG_B {
  uint8_t C7_CONFIG : 1;
  uint8_t C6_CONFIG : 1;
  uint8_t C5_CONFIG : 1;
  uint8_t C4_CONFIG : 1;
  uint8_t C3_CONFIG : 1;
  uint8_t C2_CONFIG : 1;
  uint8_t C1_CONFIG : 1;
  uint8_t C0_CONFIG : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_PIN_CONFIG_C {
  uint8_t Reserved : 5;
  uint8_t C10_CONFIG : 1;
  uint8_t C9_CONFIG : 1;
  uint8_t C8_CONFIG : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_PIN_CONFIG_D {
  /**
   * 0 = 300 kΩ used for row pull-up during key scanning.
   * 1 = 100 kΩ used for row pull-up during key scanning.
   */
  uint8_t PULL_SELECT : 1;

  /**
   * 0 = C4 remains configured as GPIO 13.
   * 1 = C4 reconfigured as RESET2 output.
   */
  uint8_t C4_EXTEND_CFG : 1;

  /**
   * 0 = R4 remains configured as GPIO 5.
   * 1 = R4 reconfigured as RESET1 output.
   */
  uint8_t R4_EXTEND_CFG : 1;

  /**
   * 0 = C6 remains configured as GPIO 15.
   * 1 = C6 reconfigured as LC2 input for Logic Block 2.
   */
  uint8_t C6_EXTEND_CFG : 1;

  /**
   * 0b00 = R3 remains configured as GPIO 4.
   * 0b01 = R3 reconfigured as LC1 input for Logic Block 1.
   * 0b10 = R3 reconfigured as PWM_OUT/CLK_OUT outputs from PWM
   *        and clock divider blocks.
   * 0b11 = unused.
   */
  uint8_t R3_EXTEND_CFG : 2;

  /**
   * 0 = C9 remains configured as GPIO 18.
   * 1 = C9 reconfigured as LY2 output from Logic Block 2.
   */
  uint8_t C9_EXTEND_CFG : 1;

  /**
   * 0 = R0 remains configured as GPIO 1.
   * 1 = R0 reconfigured as LY1 output from Logic Block 1.
   */
  uint8_t R0_EXTEND_CFG : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

enum class CoreFrequency : uint8_t {
  kHz50 = 0b00,
  kHz100 = 0b01,
  kHz200 = 0b10,
  kHz500 = 0b11,
};

struct Register_GENERAL_CFG_B {
  /**
   * Enable internal 1 MHz oscillator.
   *
   * 0 = disabled
   * 1 = enabled.
   */
  bool OSC_EN : 1;

  /**
   * Sets the input clock frequency fed from the base 1 MHz oscillator
   * to the digital core. Slower frequencies result in less IDD. However,
   * key and GPI scan times increase.
   */
  CoreFrequency CORE_FREQ : 2;

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
  bool LOGIC2_IEN : 1;

  /**
   * Logic 1 interrupt.
   *
   * 0 = Disabled.
   * 1 = Assert the INT pin if LOGIC1_INT is set.
   */
  bool LOGIC1_IEN : 1;

  /**
   * 0 = lock interrupt is disabled.
   * 1 = assert the INT pin if LOCK_INT is set.
   */
  bool LOCK_IEN : 1;

  /**
   * 0 = overflow interrupt is disabled.
   * 1 = assert the INT pin if OVRFLOW_INT is set.
   */
  bool OVRFLOW_IEN : 1;

  /**
   * 0 = GPI interrupt is disabled.
   * 1 = assert the INT pin if GPI_INT is set.
   */
  bool GPI_IEN : 1;

  /**
   * 0 = event interrupt is disabled.
   * 1 = assert the INT pin if EVENT_INT is set.
   */
  bool EVENT_IEN : 1;

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

}  // namespace adp5589
