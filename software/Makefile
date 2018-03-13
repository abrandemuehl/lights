PORT=/dev/ttyUSB0



CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy


LIBS=lib/system_stm32f0xx.o lib/stm32f0xx_rcc.o lib/stm32f0xx_gpio.o lib/stm32f0xx_usart.o lib/stm32f0xx_tim.o
OBJS=src/main.o src/startup_stm32f0xx.o src/usart.o
OBJS += $(LIBS)
LDSCRIPT=src/stm32f0.ld
DEFINES=-DSTM32F030xC -DARM_MATH_CM0 -DUSE_STDPERIPH_DRIVER

# Add HSE frequency
DEFINES += -DHSE_VALUE='((uint32_t)24000000)'

INCLUDE=include

CXXFLAGS=-std=c++14 -Wall -Werror -mlittle-endian -mcpu=cortex-m0 -mthumb -fsingle-precision-constant
CXXFLAGS += -mfloat-abi=soft
CXXFLAGS += $(DEFINES) -I$(INCLUDE)


LDFLAGS=-mthumb -T$(LDSCRIPT)
LDFLAGS+=-specs=nosys.specs
# Get floating point functions
LDFLAGS+=-lgcc
ASFLAGS=-mthumb

BINARY=main.bin


all: $(OBJS) $(BINARY)


%elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(CXXFLAGS) $(OBJS) $(LDFLAGS) -o $@

%bin: %elf
	$(OBJCOPY) -O binary -S $< $@

%hex: %elf
	$(OBJCOPY) -O ihex $< $@

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CXXFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

stm32flash: stm32flash/*
	$(MAKE) -C $@

flash: $(BINARY) stm32flash
	./stm32flash/stm32flash -v -i rts,dtr,-dtr,-rts -R -w $< $(PORT)

clean:
	rm -f $(OBJS) $(BINARY) *.o *.bin *.hex *.elf src/*.o

.PHONY: clean
