#!/bin/bash -e

# Fefore running:
# . $HOME/esp/esp-idf/export.sh

./clean.sh
./config.sh
./dobuild.sh
