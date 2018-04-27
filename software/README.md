# Lights box project


Build a follower node: `make -j4 -DLEADER=0`

Build a leader node: `make -j4 -DLEADER=1`

Flash to the code to all plugged in boards: `make flash`

# libftdi
Automatic flashing requires libftdi to control the cbus bits directly. In order for this to work, you should build and install a custom version of libftdi with the `ftdi.c.patch` applied to it

```
git apply ftdi.c.patch
```
