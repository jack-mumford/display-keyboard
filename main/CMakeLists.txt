file( GLOB SOURCES *.cc )

idf_component_register(
  SRCS
    ${SOURCES}
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/tusb.c"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/device/usbd.c"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/device/usbd_control.c"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/portable/espressif/esp32s2/dcd_esp32s2.c"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/class/hid/hid_device.c"
    "${CMAKE_SOURCE_DIR}/components/inih/ini.c"
  INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/main"
    "${CMAKE_SOURCE_DIR}/components/inih"
    "${CMAKE_SOURCE_DIR}/components/lv_port_esp32/components/lvgl/lvgl/src"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src"
    "${IDF_PATH}/components/freertos/include/freertos"
    "${IDF_PATH}/components/nvs_flash/include"
    "${IDF_PATH}/components/spiffs/include"
  REQUIRES
    lvgl_esp32_drivers
    lvgl_tft
    lvgl_touch
    spi_flash
    spiffs
)

target_compile_options(
  ${COMPONENT_TARGET} PUBLIC
  "-DCFG_TUSB_MCU=OPT_MCU_ESP32S2"
)

spiffs_create_partition_image(storage ../fs FLASH_IN_PROJECT)