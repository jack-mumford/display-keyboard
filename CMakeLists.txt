
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENT_DIRS
  components/i2clib
  components/lv_port_esp32/components/lvgl
  components/lv_port_esp32/components/lvgl_esp32_drivers
  components/lv_port_esp32/components/lvgl_esp32_drivers/lvgl_tft
  components/lv_port_esp32/components/lvgl_esp32_drivers/lvgl_touch
)

idf_build_set_property(COMPILE_DEFINITIONS -DLV_MEM_CUSTOM=1 APPEND)

project(keyboard)
