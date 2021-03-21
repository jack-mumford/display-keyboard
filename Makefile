
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := keyboard

include $(IDF_PATH)/make/project.mk

EXTRA_COMPONENT_DIRS := components/lvgl_esp32_drivers

SPIFFS_IMAGE_FLASH_IN_PROJECT := 1
$(eval $(call spiffs_create_partition_image,storage,fs))
