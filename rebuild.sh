#!/bin/bash -e

# Fefore running:
# . $HOME/esp/esp-idf/export.sh

./doclean.sh
./config.sh
./dobuild.sh
