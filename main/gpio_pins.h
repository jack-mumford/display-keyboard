#pragma once

#include <driver/i2c.h>
#include <hal/gpio_types.h>

#define BOARD_FEATHERS2 1
#define BOARD_CUCUMBER 0

#if (BOARD_FEATHERS2 == 1)

constexpr gpio_num_t kActivityGPIO = GPIO_NUM_13;  // Pin for activity LED.

constexpr i2c_port_t kKeyboardPort = I2C_NUM_0;       // I2C port for keyboard
constexpr gpio_num_t kI2C0_SDA_GPIO = GPIO_NUM_8;     // I2C port 0 SDA pin.
constexpr gpio_num_t kI2C0_SCL_GPIO = GPIO_NUM_9;     // I2C port 0 SCL pin.
constexpr gpio_num_t kKeyboardINTGPIO = GPIO_NUM_38;  // Keyboard event INT pin.

constexpr i2c_port_t kVolumeDisplayPort = I2C_NUM_1;  // I2C port for vol. disp.
constexpr gpio_num_t kI2C1_SDA_GPIO = GPIO_NUM_1;     // I2C port 1 SDA pin.
constexpr gpio_num_t kI2C1_SCL_GPIO = GPIO_NUM_3;     // I2C port 1 SCL pin.

/*
 * The SPI pins, used for display/touch, are specified in sdkconfig.
 *
 * SPI-CS-DISP  = 7
 * SPI-CS-TOUCH = 10
 * SPI-MOSI     = 35 (used for touch, but not display)
 * SPI-SCK      = 36
 * SPI-MISO     = 37
 * DC           = 5
 * Reset        = 0 (TODO: verify this)
 */

#elif (BOARD_CUCUMBER == 1)
#else
#error "define the board"
#endif
