#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := keyboard

include $(IDF_PATH)/make/project.mk

EXTRA_COMPONENT_DIRS := \
	components/lv_port_esp32/components/lv_examples \
	components/lv_port_esp32/components/lvgl \
	components/lv_port_esp32/components/lvgl_esp32_drivers/lvgl_tft \
	components/lv_port_esp32/components/lvgl_esp32_drivers/lvgl_touch \
	components/lv_port_esp32/components/lvgl_esp32_drivers

.PHONY: format
format:
	clang-format -i main/*.cc
