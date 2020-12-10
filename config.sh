#!/bin/bash -e

# Fefore running:
# . $HOME/esp/esp-idf/export.sh

idf.py set-target esp32s2
idf.py menuconfig
