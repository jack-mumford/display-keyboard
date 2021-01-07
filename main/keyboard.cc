#include "keyboard.h"

#include <cstdint>
#include <cstring>
#include <utility>

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <esp_log.h>
#include <i2clib/operation.h>

// clang-format off
// See datasheet pg. 43.
enum class Register : uint8_t {
  KBDSETTLE  = 0x01, // Keypad settle time.
  KBDBOUNCE  = 0x02, // Debounce time.
  KBDSIZE    = 0x03, // Set keypad size.
  KBDDEDCFG0 = 0x04, // Dedicated key 0.
  KBDDEDCFG1 = 0x05, // Dedicated key 1.
  KBDRIS     = 0x06, // Keyboard raw interrupt status.
  KBDMIS     = 0x07, // Keypad Masked Interrupt Status.
  KBDIC      = 0x08, // Keypad Interrupt Clear.
  KBDMSK     = 0x09, // Keypad Interrupt Mask.
  KBDCODE0   = 0x0B, // Keyboard Code 0.
  KBDCODE1   = 0x0C, // Keyboard Code 1.
  KBDCODE2   = 0x0D, // Keyboard Code 2.
  KBDCODE3   = 0x0E, // Keyboard Code 3.
  EVTCODE    = 0x10, // Key Event Code.
  TIMCFG0    = 0x60, // PWM Timer 0 Configuration.
  PWMCFG0    = 0x61, // PWM Timer 0 Configuration Control.
  TIMCFG1    = 0x68, // PWM Timer 1 Configuration.
  PWMCFG1    = 0x69, // PWM Timer 1 Configuration Control.
  TIMCFG2    = 0x70, // PWM Timer 2 Configuration.
  PWMCFG2    = 0x71, // PWM Timer 2 Configuration Control.
  TIMSWRES   = 0x78, // PWM Timer Software Reset.
  TIMRIS     = 0x7A, // PWM Timer Interrupt Status.
  TIMMIS     = 0x7B, // PWM Timer Masked Interrupt Status.
  TIMIC      = 0x7C, // PWM Timer Interrupt Clear.
  PWMWP      = 0x7D, // PWM Timer Pattern Pointer.
  PWMCFG     = 0x7E, // PWM Script.
  I2CSA      = 0x80, // I2C-Compatible ACCESS.bus 10-Bit & 7-Bit Slave Address.
  MFGCODE    = 0x80, // Manufacturer Code.
  SWREV      = 0x81, // Software Revision.
  RSTCTRL    = 0x82, // System Reset.
  RSTINTCLR  = 0x84, // Clear NO Init/Power-On Interrupt.
  CLKMODE    = 0x88, // Clock Mode.
  CLKEN      = 0x8A, // Clock enable.
  AUTOSLP    = 0x8B, // Auto-sleep Enable.
  AUTOSLPTIL = 0x8C, // Auto-sleep time (low)
  AUTOSLPTIH = 0x8D, // Auto-sheep time (high)
  IRQST      = 0x91, // Interrupt Global Interrupt Status.
  IOCFG      = 0xA7, // Input/Output Pin Mapping Configuration.
  IOPC0      = 0xAA, // Pull Resistor Configuration 0.
  IOPC1      = 0xAC, // Pull Resistor Configuration 1.
  IOPC2      = 0xAE, // Pull Resistor Configuration 2.
  GPIODATA0  = 0xC0, // GPIO Data 0.
  GPIODATA1  = 0xC2, // GPIO Data 1.
  GPIODATA2  = 0xC4, // GPIO Data 2.
  GPIODIR0   = 0xC6, // GPIO Port Direction 0.
  GPIODIR1   = 0xC7, // GPIO Port Direction 1.
  GPIODIR2   = 0xC8, // GPIO Port Direction 2.
  GPIOIS0    = 0xC9, // Interrupt Sense Configuration 0.
  GPIOIS1    = 0xCA, // Interrupt Sense Configuration 1.
  GPIOIS2    = 0xCB, // Interrupt Sense Configuration 2.
  GPIOIBE0   = 0xCC, // GPIO Interrupt Edge Configuration 0.
  GPIOIBE1   = 0xCD, // GPIO Interrupt Edge Configuration 1.
  GPIOIBE2   = 0xCE, // GPIO Interrupt Edge Configuration 2.
  GPIOIEV0   = 0xCF, // GPIO Interrupt Edge Select 0.
  GPIOIEV1   = 0xD0, // GPIO Interrupt Edge Select 1.
  GPIOIEV2   = 0xD1, // GPIO Interrupt Edge Select 2.
  GPIOIE0    = 0xD2, // GPIO Interrupt Enable 0.
  GPIOIE1    = 0xD3, // GPIO Interrupt Enable 1.
  GPIOIE2    = 0xD4, // GPIO Interrupt Enable 2.
  GPIORIS0   = 0xD6, // Raw Interrupt Status 0.
  GPIORIS1   = 0xD7, // Raw Interrupt Status 1.
  GPIORIS2   = 0xD8, // Raw Interrupt Status 2.
  GPIOMIS0   = 0xD9, // Masked Interrupt Status 0.
  GPIOMIS1   = 0xDA, // Masked Interrupt Status 1.
  GPIOMIS2   = 0xDB, // Masked Interrupt Status 2.
  GPIOIC0    = 0xDC, // GPIO Clear Interrupt 0.
  GPIOIC1    = 0xDD, // GPIO Clear Interrupt 1.
  GPIOIC2    = 0xDE, // GPIO Clear Interrupt 2.
  GPIOOME0   = 0xE0, // GPIO Open Drain Mode Enable 0.
  GPIOOMS0   = 0xE1, // GPIO Open Drain Mode Select 0.
  GPIOOME1   = 0xE2, // GPIO Open Drain Mode Enable 1.
  GPIOOMS1   = 0xE3, // GPIO Open Drain Mode Select 1.
  GPIOOME2   = 0xE4, // GPIO Open Drain Mode Enable 2.
  GPIOOMS2   = 0xE5, // GPIO Open Drain Mode Select 2.
  GPIOWAKE0  = 0xE9, // GPIO Wake-Up 0.
  GPIOWAKE1  = 0xEA, // GPIO Wake-Up 1.
  GPIOWAKE2  = 0xEB, // GPIO Wake-Up 2.
};
// clang-format on

namespace {
constexpr char TAG[] = "kbd_kbd";
constexpr uint8_t kSlaveAddress = 0x88;  // I2C address of LM8330 IC.
constexpr uint8_t k12msec = 0x80;

struct Register_IOCFG {
  uint8_t Reserved1 : 3;  // Reserved - set to zero.
  uint8_t GPIOSEL : 1;    // KPY11 config. 1 = IRQN enabled.
  uint8_t Reserved2 : 1;  // Reserved - set to zero.
  uint8_t BALLCFG : 3;    // Select column to configure.

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_KBDSIZE {
  uint8_t ROWSIZE : 4;  // Number of rows in the keyboard matrix.
  uint8_t COLSIZE : 4;  // Number of columns in the keyboard matrix.

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_CLKEN {
  uint8_t Reserved1 : 5;  // Reserved - set to zero.
  uint8_t TIMEN : 1;      // PWM Timer 0, 1, 2 clock enable.
  uint8_t Reserved2 : 1;  // Reserved - set to zero.
  uint8_t KBDEN : 1;      // Keyboard clock enabled (enables/disables key scan).

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_KBDMSK {
  uint8_t Reserved : 4;  // Reserved.
  uint8_t MSKELINT : 1;  // Keyboard event lost interrupt RELINT is masked.
  uint8_t MSKEINT : 1;   // keyboard event interrupt REVINT is masked,
  uint8_t MSKLINT : 1;   // keyboard lost interrupt RKLINT is masked.
  uint8_t MSKSINT : 1;   // keyboard status interrupt RSINT is masked.

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

struct Register_KBDIC {
  uint8_t SFOFF : 1;     // Switches off scanning of special function (SF) keys.
  uint8_t Reserved : 5;  // Keyboard event lost interrupt RELINT is masked.
  uint8_t EVTIC : 1;  // Clear EVTCODE FIFO and corresponding interrupts REVTINT
                      // and RELINT.
  uint8_t KBDIC : 1;  // Clear RSINT and RKLINT interrupt bits.

  operator uint8_t() const { return *reinterpret_cast<const uint8_t*>(this); }
};

constexpr uint8_t kPullNeither = 0x0;
constexpr uint8_t kPullDown = 0x1;
constexpr uint8_t kPullUp = 0x2;

struct Register_IOPC1 {
  uint16_t KPY7PR : 2;  // pullup: 2/3, pulldown: 1, no-pull: 0.
  uint16_t KPY6PR : 2;
  uint16_t KPY5PR : 2;
  uint16_t KPY4PR : 2;
  uint16_t KPY3PR : 2;
  uint16_t KPY2PR : 2;
  uint16_t KPY1PR : 2;
  uint16_t KPY0PR : 2;

  operator uint16_t() const { return *reinterpret_cast<const uint16_t*>(this); }
};

struct Register_KBDDEDCFG {
  uint16_t KPX7 : 1;
  uint16_t KPX6 : 1;
  uint16_t KPX5 : 1;
  uint16_t KPX4 : 1;
  uint16_t KPX3 : 1;
  uint16_t KPX2 : 1;
  uint16_t KPY11 : 1;
  uint16_t KPY10 : 1;
  uint16_t KPY9 : 1;
  uint16_t KPY8 : 1;
  uint16_t KPY7 : 1;
  uint16_t KPY6 : 1;
  uint16_t KPY5 : 1;
  uint16_t KPY4 : 1;
  uint16_t KPY3 : 1;
  uint16_t KPY2 : 1;

  operator uint16_t() const { return *reinterpret_cast<const uint16_t*>(this); }
};

static_assert(sizeof(Register_IOCFG) == sizeof(uint8_t));
static_assert(sizeof(Register_IOPC1) == sizeof(uint16_t));
static_assert(sizeof(Register_KBDDEDCFG) == sizeof(uint16_t));

}  // namespace

Keyboard::Keyboard(i2c::Master i2c_master)
    : i2c_master_(std::move(i2c_master)) {}

Keyboard::~Keyboard() = default;

esp_err_t Keyboard::Initialize() {
  esp_err_t err = WriteByte(Register::KBDSETTLE, k12msec);
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::KBDBOUNCE, k12msec);
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::KBDSIZE, Register_KBDSIZE{
                                         .ROWSIZE = 8,
                                         .COLSIZE = 8,
                                     });
  if (err != ESP_OK)
    return err;
  err = WriteWord(Register::KBDDEDCFG0, Register_KBDDEDCFG{
                                            .KPX7 = 1,
                                            .KPX6 = 1,
                                            .KPX5 = 1,
                                            .KPX4 = 1,
                                            .KPX3 = 1,
                                            .KPX2 = 1,
                                            .KPY11 = 1,
                                            .KPY10 = 1,
                                            .KPY9 = 1,
                                            .KPY8 = 1,
                                            .KPY7 = 1,
                                            .KPY6 = 1,
                                            .KPY5 = 1,
                                            .KPY4 = 1,
                                            .KPY3 = 1,
                                            .KPY2 = 1,
                                        });
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::IOCFG, Register_IOCFG{
                                       .Reserved1 = 0,
                                       .GPIOSEL = 1,
                                       .Reserved2 = 0,
                                       .BALLCFG = 1,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteWord(Register::IOPC0, Register_IOCFG{
                                       .Reserved1 = 0,
                                       .GPIOSEL = 0,
                                       .Reserved2 = 0,
                                       .BALLCFG = 0,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteWord(Register::IOPC1, Register_IOPC1{
                                       .KPY7PR = kPullDown,
                                       .KPY6PR = kPullDown,
                                       .KPY5PR = kPullDown,
                                       .KPY4PR = kPullDown,
                                       .KPY3PR = kPullDown,
                                       .KPY2PR = kPullDown,
                                       .KPY1PR = kPullDown,
                                       .KPY0PR = kPullDown,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::KBDIC, Register_KBDIC{
                                       .SFOFF = false,
                                       .Reserved = 0,
                                       .EVTIC = true,
                                       .KBDIC = true,
                                   });
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::KBDMSK, Register_KBDMSK{
                                        .Reserved = 0,
                                        .MSKELINT = 0,
                                        .MSKEINT = 0,
                                        .MSKLINT = 1,
                                        .MSKSINT = 1,
                                    });
  if (err != ESP_OK)
    return err;
  err = WriteByte(Register::CLKEN, Register_CLKEN{
                                       .Reserved1 = 0,
                                       .TIMEN = false,
                                       .Reserved2 = 0,
                                       .KBDEN = true,

                                   });
  if (err != ESP_OK)
    return err;
  return ESP_OK;
}

esp_err_t Keyboard::LogEventCount() {
  return ESP_OK;
}

esp_err_t Keyboard::WriteByte(Register reg, uint8_t value) {
  return i2c_master_.WriteRegister(kSlaveAddress, static_cast<uint8_t>(reg),
                                   value);
}

esp_err_t Keyboard::WriteWord(Register reg, uint16_t value) {
  i2c::Operation op = i2c_master_.CreateWriteOp(
      kSlaveAddress, static_cast<uint8_t>(reg), "Kbd::WriteWord");
  if (!op.ready())
    return ESP_FAIL;
  return op.Write(&value, sizeof(value)) ? ESP_OK : ESP_FAIL;
}