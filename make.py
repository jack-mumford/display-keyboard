#!/usr/bin/env python3

import errno
import glob
import os
import subprocess
import sys


class SdkConfigManager(object):

    @staticmethod
    def GetConsoleTarget(sdkconfig):
        if 'CONFIG_ESP_CONSOLE_UART' in sdkconfig and \
            sdkconfig['CONFIG_ESP_CONSOLE_UART'] == 'y':
            return 'UART'
        if 'CONFIG_ESP_CONSOLE_CDC' in sdkconfig and \
            sdkconfig['CONFIG_ESP_CONSOLE_CDC'] == 'y':
            return 'CDC'
        if 'CONFIG_ESP_CONSOLE_CUSTOM' in sdkconfig and \
            sdkconfig['CONFIG_ESP_CONSOLE_CUSTOM'] == 'y':
            return 'CUSTOM'
        return 'NONE'

    @staticmethod
    def Load():
        values = dict()
        with open('sdkconfig', 'r') as f:
            for line in f.readlines():
                line = line.strip()
                if len(line) < 1:
                    continue
                if line[0] == '#':
                    continue
                items = line.split('=')
                if len(items) != 2:
                    print(line)
                if items[0] in values:
                    print('Already have: ' + line)
                values[items[0]] = items[1]
        return values

class PortManager(object):
    ignored = ('/dev/cu.Bluetooth-Incoming-Port')

    def __init__(self, sdkconfig):
        self.__sdkconfig = sdkconfig
        self.__console_target = SdkConfigManager.GetConsoleTarget(sdkconfig)

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

    def GetMonitorPort(self):
        if self.__console_target == 'UART':
            return PortManager.GetSerialPorts()
        return PortManager.GetModemPorts()

    def PrintPorts(self):
        print('Serial ports: %s' % PortManager.GetSerialPorts())
        print('Modem ports: %s' % PortManager.GetSerialPorts())
        print('Flash port: %s' % PortManager.GetFlashPort())
        print('Monitor port: %s' % self.GetMonitorPort())


def TargetNeedsMonitorPort(target):
    return target in ('monitor')


def TargetNeedsFlashPort(target):
    return target in ('flash', 'flash-erase', 'manual-flash',
                      'flash-feathers2-stock-bootloader')


def MakeTargets(targets, port_manager):
    flash_ports = PortManager.GetFlashPort()
    monitor_ports = port_manager.GetMonitorPort()
    for target in targets:
        if TargetNeedsMonitorPort(target) and not monitor_ports:
            print("Target \"%s\" needs a monitor port, but can't find one." % target, file=sys.stderr)
            sys.exit(errno.ENODEV)
        if TargetNeedsFlashPort(target) and not flash_ports:
            print("Target \"%s\" needs a flash port, but can't find one." % target, file=sys.stderr)
            sys.exit(errno.ENODEV)
    if flash_ports:
        os.environ["FLASH_PORT"] = flash_ports[0]
    if monitor_ports:
        os.environ["MONITOR_PORT"] = monitor_ports[0]
    cmd = ['make', '--file=Makefile-build']
    cmd.extend(targets)
    subprocess.check_call(cmd)

if 'IDF_PATH' not in os.environ:
    print('ESP-IDF environment not sourced. Run:', file=sys.stderr)
    print(file=sys.stderr)
    print('. $HOME/esp/esp-idf/export.sh', file=sys.stderr)
    print(file=sys.stderr)
    sys.exit(1)

port_manager = PortManager(SdkConfigManager.Load())

targets = sys.argv[1:]
MakeTargets(targets, port_manager)
