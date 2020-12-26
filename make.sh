#!/bin/bash -e

# Run this first:
# . $HOME/esp/esp-idf/export.sh

port=$(./getdev.py)
PORT=$port make --file=Makefile-build $*
