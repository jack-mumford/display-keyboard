file( GLOB SOURCES *.cc *.c )

idf_component_register(
  SRCS
    ${SOURCES}
    "${CMAKE_SOURCE_DIR}/libs/i2clib/src/address.cc"
    "${CMAKE_SOURCE_DIR}/libs/i2clib/src/master.cc"
    "${CMAKE_SOURCE_DIR}/libs/i2clib/src/operation.cc"
    "${CMAKE_SOURCE_DIR}/libs/inih/ini.c"
    "${CMAKE_SOURCE_DIR}/libs/lv_lib_png/lodepng.c"
    "${CMAKE_SOURCE_DIR}/libs/lv_lib_png/lv_png.c"
    "${CMAKE_SOURCE_DIR}/libs/kbdlib/src/lm8330.cc"
    "${CMAKE_SOURCE_DIR}/libs/tjpgdec/src/tjpgd.c"
    "${IDF_PATH}/components/tinyusb/tinyusb/src/class/hid/hid_device.c"
    "${IDF_PATH}/components/tinyusb/tinyusb/src/device/usbd.c"
    "${IDF_PATH}/components/tinyusb/tinyusb/src/device/usbd_control.c"
    "${IDF_PATH}/components/tinyusb/tinyusb/src/portable/espressif/esp32s2/dcd_esp32s2.c"
    "${IDF_PATH}/components/tinyusb/tinyusb/src/tusb.c"
  INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/components"
    "${CMAKE_SOURCE_DIR}/components/lvgl/src"
    "${CMAKE_SOURCE_DIR}/components/tarablessd1306"
    "${CMAKE_SOURCE_DIR}/libs"
    "${CMAKE_SOURCE_DIR}/libs/i2clib/include"
    "${CMAKE_SOURCE_DIR}/libs/kbdlib/include"
    "${CMAKE_SOURCE_DIR}/libs/inih"
    "${CMAKE_SOURCE_DIR}/main"
    "${IDF_PATH}/components"
    "${IDF_PATH}/components/freertos/include/freertos"
    "${IDF_PATH}/components/mbedtls/mbedtls/include"
    "${IDF_PATH}/components/mbedtls/port/include"
    "${IDF_PATH}/components/nghttp/port/include"
    "${IDF_PATH}/components/tinyusb/tinyusb/src"
  REQUIRES
    lvgl_esp32_drivers
    spi_flash
    spiffs
)

target_compile_options(
  ${COMPONENT_TARGET} PUBLIC
  "-DCFG_TUSB_MCU=OPT_MCU_ESP32S2"
  "-DLV_PNG_USE_LV_FILESYSTEM=1"
  "-DDISP_I2C_PORT=1"
)

spiffs_create_partition_image(storage ../fs FLASH_IN_PROJECT)
