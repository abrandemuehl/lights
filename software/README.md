# Single Board Quadcopter Software

Uses ST's [CMSIS distribution for the STM32F427xx](http://www.st.com/en/embedded-software/stsw-stm32065.html) for the chip specific information



## Setting up the linux serial port
`stty -F /dev/ttyUSB0 115200 cs8 -cstopb -parenb`
