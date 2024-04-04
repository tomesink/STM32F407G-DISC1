#CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)
#LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc -x assembler-with-cpp
CPU = -mcpu=cortex-m4
CFLAGS=$(CPU) -mthumb -std=gnu11 -Wall -O0 -g3
LDFLAGS= -nostdlib -T link.ld -Wl,-Map=blinky.map  

SOURCES = main.c 

BUILD_DIR = /Users/martin/repos/baremetal/discovery/01-blinky

TARGET = firmware

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -f
endif

build: firmware.bin

startup.o: startup.s 
	-c $(CC) $(CFLAG) -o $@ $^

firmware.elf: $(SOURCES) startup.o
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

# Change to yours
device = STM32F407VG
# First create a file with all commands
$(BUILD_DIR)/jflash: $(BUILD_DIR)/$(TARGET).bin
	@touch $@
	@echo device $(device) > $@
	@echo -e si 1'\n'speed 4000 >> $@
	@echo loadbin $< 0x8000000 >> $@
	@echo -e r'\n'g'\n'qc >> $@
# Flash with J-Link
# Configure device name, everything else should remain the same
jflash: $(BUILD_DIR)/jflash
	JLinkExe -commanderscript $<

# flash: firmware.bin
# 	st-flash --reset write $< 0x8000000

clean:
	$(RM) firmware.*
