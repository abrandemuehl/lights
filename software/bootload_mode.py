#!/usr/bin/env python2
import ftdi1 as ftdi
import time
import os
import sys
import ctypes
import re
import subprocess

BOOT0 = 1 << 2
NRST = 1 << 3
bitmode = 0xf0 | NRST


def setNRST(ftdic, value):
    global bitmode
    if(value):
        bitmode |= NRST
    else:
        bitmode &= ~NRST
    ret = ftdi.set_bitmode(ftdic, bitmode, ftdi.BITMODE_CBUS)
    if ret < 0:
        print('Cannot enable bitbang mode')
        os._exit(1)

def setBOOT0(ftdic, value):
    global bitmode
    if(value):
        bitmode |= BOOT0
    else:
        bitmode &= ~BOOT0

    ret = ftdi.set_bitmode(ftdic, bitmode, ftdi.BITMODE_CBUS)
    if ret < 0:
        print('Cannot enable bitbang mode')
        os._exit(1)

def reset(ftdic):
    setNRST(ftdic, 0)
    time.sleep(0.1)
    setNRST(ftdic, 1)
    time.sleep(0.5)

if __name__ == "__main__":
    if(len(sys.argv) < 2):
        print("Specify an operation")
        os._exit(1)
    if not sys.argv[1] in ["bootload", "reset"]:
        print("Invalid operation: %s" %sys.argv[1])
        os._exit(1)

    device_re = re.compile("Bus\s+(?P<bus>\d+)\s+Device\s+(?P<device>\d+).+ID\s(?P<id>\w+:\w+)\s(?P<tag>.+)$", re.I)
    df = subprocess.check_output(["lsusb", "-d 0403:6001"])
    devices = []
    for i in df.split('\n'):
        if i:
            info = device_re.match(i)
            if info:
                dinfo = info.groupdict()
                bus = dinfo.pop('bus')
                device = dinfo.pop('device')
                devices.append({'bus': bus, 
                    'device': device,
                    'path': '/dev/bus/usb/%s/%s' % (bus, device)})

    # print(devices)
    # if len(devices) == 0:
    #     print("No ftdi usb devices attached")
    #     os._exit(1)
    # elif(len(devices) > 1):
    #     # Multiple devices plugged in
    #     if(len(sys.argv) < 3):
    #         print("Multiple devices plugged in. Please specify an index")
    #         os._exit(1)
    #     device = devices[int(sys.argv[2])]
    # else:
    #     # Only one device plugged in
    #     device = devices[0]



    # print(device['path'])
    # ret = ftdi.usb_open(ftdic, 0x0403, 0x6001)
    print(devices)

    ftdics = []
    for device in devices:
        ret = ftdi.new()
        if ret == 0:
            print('new failed: %d' % ret)
            os._exit(1)
        ftdics.append(ret)

    for ftdic, device in zip(ftdics, devices):
        print(device['path'])
        ret = ftdi.usb_open_bus_addr(ftdic, int(device['bus']), int(device['device']))
        if ret < 0:
            print('unable to open ftdi device: %d (%s)' %
                    (ret, ftdi.get_error_string(ftdic)))
            os._exit(1)

    if(sys.argv[1] == "bootload"):
        for ftdic in ftdics:
            setBOOT0(ftdic, 1)
        time.sleep(0.1)
        for ftdic in ftdics:
            reset(ftdic)
    elif(sys.argv[1] == "reset"):
        for ftdic in ftdics:
            setBOOT0(ftdic, 0)
        time.sleep(0.1)
        for ftdic in ftdics:
            reset(ftdic)
    else:
        print("Unrecognized command")

    for ftdic in ftdics:
        ftdi.usb_close(ftdic)
        ftdi.free(ftdic)
