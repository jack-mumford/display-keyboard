#!/bin/bash -e

# Fefore running:
# . $HOME/esp/esp-idf/export.sh

idf.py build

exit 0

PORT=foo
idf.py -p ${PORT} flash monitor
