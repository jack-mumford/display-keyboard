file( GLOB SOURCES *.cc *.c )

idf_component_register(
  SRCS
    ${SOURCES}
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/tusb.c"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/device/usbd.c"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/device/usbd_control.c"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/portable/espressif/esp32s2/dcd_esp32s2.c"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/class/hid/hid_device.c"
    "${CMAKE_SOURCE_DIR}/components/inih/ini.c"
    "${CMAKE_SOURCE_DIR}/components/i2clib/src/master.cc"
    "${CMAKE_SOURCE_DIR}/components/i2clib/src/operation.cc"
    "${CMAKE_SOURCE_DIR}/components/lv_lib_png/lodepng.c"
    "${CMAKE_SOURCE_DIR}/components/lv_lib_png/lv_png.c"
  INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/components"
    "${CMAKE_SOURCE_DIR}/components/i2clib/include"
    "${CMAKE_SOURCE_DIR}/components/inih"
    "${CMAKE_SOURCE_DIR}/components/lv_port_esp32/components/lvgl/lvgl/src"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src"
    "${CMAKE_SOURCE_DIR}/main"
    "${IDF_PATH}/components/driver/include"
    "${IDF_PATH}/components/esp-tls"
    "${IDF_PATH}/components/esp_common/include"
    "${IDF_PATH}/components/esp_http_client/include"
    "${IDF_PATH}/components/esp_http_server/include"
    "${IDF_PATH}/components/freertos/include/freertos"
    "${IDF_PATH}/components/hal/esp32s2/include"
    "${IDF_PATH}/components/hal/include"
    "${IDF_PATH}/components/json"
    "${IDF_PATH}/components/mbedtls/esp_crt_bundle/include"
    "${IDF_PATH}/components/mbedtls/mbedtls/include"
    "${IDF_PATH}/components/mbedtls/port/include"
    "${IDF_PATH}/components/nghttp/port/include"
    "${IDF_PATH}/components/nvs_flash/include"
    "${IDF_PATH}/components/soc/esp32s2/include"
    "${IDF_PATH}/components/spiffs/include"
  REQUIRES
    i2clib
    lvgl_esp32_drivers
    lvgl_tft
    lvgl_touch
    spi_flash
    spiffs
    tinyusb
)

target_compile_options(
  ${COMPONENT_TARGET} PUBLIC
  "-DCFG_TUSB_MCU=OPT_MCU_ESP32S2"
  "-DLV_PNG_USE_LV_FILESYSTEM=1"
)

spiffs_create_partition_image(storage ../fs FLASH_IN_PROJECT)
