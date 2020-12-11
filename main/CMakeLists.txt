idf_component_register(
  SRCS
    "display.cc"
    "keyboard_main.cc"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src/tusb.c"
  INCLUDE_DIRS
    "${CMAKE_SOURCE_DIR}/main"
    "${CMAKE_SOURCE_DIR}/components/tinyusb/src"
    "${IDF_PATH}/components/freertos/include/freertos"
  REQUIRES
    lvgl_esp32_drivers
    lvgl
    lv_examples
    lvgl_tft
    lvgl_touch
    spi_flash
)

target_compile_definitions(
  ${COMPONENT_LIB} PRIVATE LV_CONF_INCLUDE_SIMPLE=1
)

target_compile_options(
  ${COMPONENT_TARGET} PUBLIC
  "-DCFG_TUSB_MCU=OPT_MCU_ESP32S2"
)
