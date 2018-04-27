PORT=/dev/ttyUSB0
LEADER=0



CC=arm-none-eabi-gcc
CXX=arm-none-eabi-g++
LD=arm-none-eabi-ld
AS=arm-none-eabi-as
OBJCOPY=arm-none-eabi-objcopy


LIBS=lib/stdperiph/system_stm32f0xx.o lib/stdperiph/stm32f0xx_rcc.o lib/stdperiph/stm32f0xx_gpio.o lib/stdperiph/stm32f0xx_usart.o lib/stdperiph/stm32f0xx_tim.o lib/stdperiph/stm32f0xx_misc.o lib/stdperiph/stm32f0xx_rtc.o lib/stdperiph/stm32f0xx_exti.o lib/stdperiph/stm32f0xx_pwr.o
# LIBS=lib/cmsis/system_stm32f0xx.o lib/hal/stm32f0xx_hal_usart.o
OBJS=$(patsubst %.cc,%.o,$(wildcard src/*.cc)) $(patsubst %.c,%.o,$(wildcard src/*.c)) src/startup_stm32f0xx.o
OBJS += $(LIBS)
LDSCRIPT=src/stm32f0.ld
DEFINES=-DSTM32F030xC -DARM_MATH_CM0 -DUSE_STDPERIPH_DRIVER -DLEADER=$(LEADER)

# Add HSE frequency
DEFINES += -DHSE_VALUE='((uint32_t)24000000)'

# INCLUDES=-Iinclude/hal -Iinclude/cmsis -Iinclude/stdperiph
INCLUDES=-Iinclude/stdperiph

# CXXFLAGS=-std=c++14 -g -Wall -mlittle-endian -mcpu=cortex-m0 -mthumb -fsingle-precision-constant # -nostdlib
# CXXFLAGS += -mfloat-abi=soft
# CXXFLAGS += $(DEFINES) $(INCLUDES)

CFLAGS=-std=c11 -g -Wall -mlittle-endian -mcpu=cortex-m0 -mthumb -fsingle-precision-constant # -nostdlib
CFLAGS += -mfloat-abi=soft
CFLAGS += $(DEFINES) $(INCLUDES)


# Must include BOTH -mcpu=cortex-m0 and -mthumb for integer division library to be linked
LDFLAGS=-mcpu=cortex-m0 -mthumb -T$(LDSCRIPT) # -Wl,-wrap,__aeabi_unwind_cpp_pr0 -Wl,-wrap,__aeabi_unwind_cpp_pr1 -Wl,-wrap,__aeabi_unwind_cpp_pr2 -Wl,-wrap,atexit
LDFLAGS+= # -specs=nano.specs
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

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) -c $< -o $@

stm32flash: stm32flash/*
	$(MAKE) -C $@

flash: $(BINARY) stm32flash
	# ./stm32flash/stm32flash -v -i rts,dtr,-dtr,-rts -R -w $< $(PORT)
	set -e; \
	./bootload_mode.py bootload $$i; \
	sleep 0.5; \
	for p in $$(ls /dev/ttyUSB*); do \
		./stm32loader.py -p $$p -ew $(BINARY) & \
	done; \
	wait; \
	./bootload_mode.py reset $$i;

reset: 
	./bootload_mode.py reset $$i

clean:
	rm -f $(OBJS) $(BINARY) *.o *.bin *.hex *.elf src/*.o

.PHONY: clean flash reset
