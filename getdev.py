#!/usr/bin/env python3

import errno, glob, os, sys

ignored = ('cu.Bluetooth-Incoming-Port')

num_devices = 0
os.chdir("/dev")
for file in glob.glob("cu*"):
    if file not in ignored:
        print("/dev/" + file)
        num_devices += 1

if num_devices == 1:
    sys.exit(0)
if num_devices == 0:
    sys.exit(errno.ENODEV)
sys.exit(errno.EMLINK) # really anything other then the other two will do.