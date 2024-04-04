CFLAGS  ?=  -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common  \
            -g3 -Os -ffunction-sections -fdata-sections \
            -Isrc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard \
			-mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)

CFLAGS += -Wconversion
CFLAGS += -Icmsis_core/CMSIS/Core/Include
CFLAGS += -Icmsis_f4/Include
CGLAGS += -v

LINKER_FILE = $(SRC_DIR)/linker.ld
LDFLAGS ?= -T$(LINKER_FILE) -nostartfiles -nostdlib --specs nano.specs -lc -lgcc -Wl,--gc-sections -Wl,-Map=$@.map

PROJECT = printf
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy

SRC_DIR = src


SOURCES = $(SRC_DIR)/*.c
SOURCES += cmsis_f4/Source/Templates/gcc/startup_stm32f407xx.s
BUILD_DIR = $(PWD)/build

DEVICE = STM32F407VG

TARGET = $(DEVICE)-$(PROJECT)

RM = rm -f
MD = mkdir -p

build: directories $(BUILD_DIR)/$(TARGET).bin

directories: $(BUILD_DIR)

$(BUILD_DIR):
	$(MD) $@

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(CP) -O binary $< $@

$(BUILD_DIR)/$(TARGET).elf: $(SOURCES)
	 $(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

# Flash with J-Link
flash: $(BUILD_DIR)/jflash
	JLinkExe -if swd -device $(DEVICE) -speed 4000 -commanderscript $<

# First create a file with all commands
$(BUILD_DIR)/jflash: $(BUILD_DIR)/$(TARGET).bin
	@echo loadbin $< 0x8000000 > $@
	@echo -e r'\n'g'\n'qc >> $@

clean:
	$(RM)r $(BUILD_DIR)
