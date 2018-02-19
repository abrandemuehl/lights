PORT=/dev/ttyUSB0



CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy


LIBS=lib/system_stm32f0xx.o lib/stm32f0xx_rcc.o lib/stm32f0xx_gpio.o lib/stm32f0xx_usart.o
OBJS=src/main.o src/startup.o
OBJS += $(LIBS)
LDSCRIPT=src/stm32f0.ld
DEFINES=-DSTM32F030xC -DARM_MATH_CM0 -DUSE_STDPERIPH_DRIVER

# Add HSE frequency
DEFINES+='-DHSE_VALUE=((uint32_t)24000000)'

INCLUDE=include

CFLAGS=-std=c11 -O2 -Wall -Werror -mlittle-endian -mcpu=cortex-m0 -mthumb -fsingle-precision-constant
CFLAGS += -mfloat-abi=soft
CFLAGS += $(DEFINES) -I$(INCLUDE)


LDFLAGS=-mthumb -T$(LDSCRIPT)
LDFLAGS+=-specs=nosys.specs
# Get floating point functions
LDFLAGS+=-lgcc
ASFLAGS=-mthumb

BINARY=main.bin


all: $(OBJS) $(BINARY)


%elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

%bin: %elf
	$(OBJCOPY) -O binary -S $< $@

%hex: %elf
	$(OBJCOPY) -O ihex $< $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

stm32flash: stm32flash/*
	$(MAKE) -C $@

flash: $(BINARY) stm32flash
	./stm32flash/stm32flash -v -i -rts,dtr,-dtr,rts -R -w $< $(PORT)

clean:
	rm -f $(CMSIS_DeviceSystem) $(OBJS) $(BINARY) *.o *.bin *.hex *.elf src/*.o

.PHONY: clean
