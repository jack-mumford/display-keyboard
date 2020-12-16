#!/bin/sh -e

# Fefore running:
# . $HOME/esp/esp-idf/export.sh

make --file=Makefile-build $*
