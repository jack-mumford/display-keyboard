#!/bin/bash -e

# Fefore running:
# . $HOME/esp/esp-idf/export.sh

idf.py build

#exit 0

PORT=/dev/cu.usbserial-GT4YU06W
idf.py -p ${PORT} flash monitor
