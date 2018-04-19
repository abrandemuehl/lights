#!/usr/bin/env python2
import ftdi1 as ftdi
import time
import os
import sys

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
    ftdic = ftdi.new()
    if ftdic == 0:
        print('new failed: %d' % ret)
        os._exit(1)
    ret = ftdi.usb_open(ftdic, 0x0403, 0x6001)
    if ret < 0:
        print('unable to open ftdi device: %d (%s)' %
                (ret, ftdi.get_error_string(ftdic)))
        os._exit(1)

    if(sys.argv[1] == "bootload"):
        setBOOT0(ftdic, 1)
        time.sleep(0.1)
        reset(ftdic)
    elif(sys.argv[1] == "reset"):
        setBOOT0(ftdic, 0)
        time.sleep(0.1)
        reset(ftdic)
    else:
        print("Unrecognized command")

    ftdi.usb_close(ftdic)
    ftdi.free(ftdic)
