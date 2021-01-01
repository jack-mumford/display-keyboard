#!/usr/bin/env python3

import errno
import glob
import os
import subprocess
import sys


class PortManager(object):
    ignored = ('/dev/cu.Bluetooth-Incoming-Port')

    @staticmethod
    def __GetSerialWildcard():
        return '/dev/cu.usbserial-*'

    @staticmethod
    def __GetModemWildcard():
        return '/dev/cu.usbmodem*'

    @staticmethod
    def __GetMatchingPorts(wildcard):
        ports = []
        for file in glob.glob(wildcard, recursive=False):
            if file in PortManager.ignored:
                continue
            ports.append(file)
        return ports

    @staticmethod
    def GetSerialPorts():
        return PortManager.__GetMatchingPorts(PortManager.__GetSerialWildcard())

    @staticmethod
    def GetModemPorts():
        return PortManager.__GetMatchingPorts(PortManager.__GetModemWildcard())

    @staticmethod
    def GetFlashPort():
        ports = PortManager.GetModemPorts()
        if ports:
            return ports
        return PortManager.GetSerialPorts()

    @staticmethod
    def GetMonitorPort():
        ports = PortManager.GetSerialPorts()
        if ports:
            return ports
        return PortManager.GetFlashPort()


def TargetNeedsMonitorPort(target):
    return target in ('monitor')


def TargetNeedsFlashPort(target):
    return target in ('flash', 'flash-erase', 'manual-flash',
                      'flash-feathers2-stock-bootloader')


def MakeTargets(targets):
    flash_ports = PortManager.GetFlashPort()
    monitor_ports = PortManager.GetMonitorPort()
    for target in targets:
        if TargetNeedsMonitorPort(target) and not monitor_ports:
            print("%s needs a port, but can't find one", target, file=sys.stderr)
            sys.exit(errno.ENODEV)
        if TargetNeedsFlashPort(target) and not flash_ports:
            print("%s needs a port, but can't find one", target, file=sys.stderr)
            sys.exit(errno.ENODEV)
    if flash_ports:
        os.environ["FLASH_PORT"] = flash_ports[0]
    if monitor_ports:
        os.environ["MONITOR_PORT"] = monitor_ports[0]
    print('Flash ports: %s' % flash_ports)
    print('Monitor ports: %s' % monitor_ports)
    cmd = ['make', '--file=Makefile-build']
    cmd.extend(targets)
    subprocess.check_call(cmd)

if 'IDF_PATH' not in os.environ:
    print('ESP-IDF environment not sourced. Run:', file=sys.stderr)
    print(file=sys.stderr)
    print('. $HOME/esp/esp-idf/export.sh', file=sys.stderr)
    print(file=sys.stderr)
    sys.exit(1)

targets = sys.argv[1:]
MakeTargets(targets)
