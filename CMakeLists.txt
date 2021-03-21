
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set(EXTRA_COMPONENT_DIRS
  components/lvgl
  components/lvgl_esp32_drivers
  components/tarablessd1306
)

idf_build_set_property(COMPILE_DEFINITIONS -DLV_MEM_CUSTOM=1 APPEND)

project(keyboard)
