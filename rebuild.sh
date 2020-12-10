#!/bin/bash -e

# Fefore running:
# . $HOME/esp/esp-idf/export.sh

rm -rf build

idf.py set-target esp32s2

idf.py menuconfig

./dobuild.sh
