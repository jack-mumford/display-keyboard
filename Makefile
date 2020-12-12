#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := keyboard

include $(IDF_PATH)/make/project.mk

EXTRA_COMPONENT_DIRS := \
	components/lv_port_esp32/components/lvgl_esp32_drivers \
	components/lv_port_esp32/components/lvgl_esp32_drivers/lvgl_tft \
	components/lv_port_esp32/components/lvgl_esp32_drivers/lvgl_touch

SPIFFS_IMAGE_FLASH_IN_PROJECT := 1
$(eval $(call spiffs_create_partition_image,storage,fs))