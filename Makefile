#===============================================================================
# Makefile — STM32F103C8 (Cortex-M3) for arm-none-eabi-gcc
# Usage:
#   make              build test.elf / test.hex / test.bin
#   make USE_EXTENDED=1   build with main_extended.c
#   make clean        remove build artifacts
#   make flash        flash via ST-Link (st-flash)
#
# Keil uVision5 uses test.uvprojx — this file does not affect it.
#===============================================================================

# Project name
TARGET := test

# Toolchain
PREFIX  := arm-none-eabi-
CC      := $(PREFIX)gcc
LD      := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
SIZE    := $(PREFIX)size
GDB     := $(PREFIX)gdb

# MCU flags
MCU_FLAGS := -mcpu=cortex-m3 -mthumb -mfloat-abi=soft

# Compile definitions (matches Keil: STM32F10X_MD, USE_STDPERIPH_DRIVER)
DEFINES := -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER

# Include paths
INC_DIRS := \
    -Istart \
    -IUser \
    -Ilibrary

# Source files
APP_SRCS := \
    ultrasonic.c \
    usart.c \
    at24c64.c \
    delay.c \
    font.c \
    json_parser.c \
    key.c \
    led.c \
    oled.c \
    servo.c

# Pick main or extended main
ifeq ($(USE_EXTENDED),1)
    APP_SRCS += main_extended.c
else
    APP_SRCS += main.c
endif

# STM32 standard peripheral library (all .c in library/)
LIB_SRCS := $(wildcard library/*.c)

# System / CMSIS sources
SYS_SRCS := \
    User/stm32f10x_it.c \
    start/system_stm32f10x.c \
    start/core_cm3.c \
    retarget_gcc.c

# GCC startup file
STARTUP_SRC := startup_stm32f10x_md_gcc.c

# All sources
ALL_SRCS := $(APP_SRCS) $(LIB_SRCS) $(SYS_SRCS) $(STARTUP_SRC)

# Object files (in build/ subdirectory)
OBJS := $(ALL_SRCS:%.c=build/%.o)
DEPS := $(OBJS:%.o=%.d)

# Compiler flags
CFLAGS := $(MCU_FLAGS) $(DEFINES) $(INC_DIRS)
CFLAGS += -Wall -Wextra -Wno-unused-parameter
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -MMD -MP
CFLAGS += -O2
CFLAGS += -g

# Linker flags
LDFLAGS := $(MCU_FLAGS)
LDFLAGS += -Wl,-Map=$(TARGET).map
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,--print-memory-usage
LDFLAGS += -TSTM32F103C8.ld
LDFLAGS += -specs=nano.specs
LDFLAGS += -specs=nosys.specs
LDFLAGS += -u _printf_float
LDFLAGS += -u _scanf_float

#===============================================================================
# Targets
#===============================================================================

.PHONY: all clean flash

all: $(TARGET).elf $(TARGET).hex $(TARGET).bin

# Link
$(TARGET).elf: $(OBJS)
	@echo "==> LINK  $@"
	$(LD) $(LDFLAGS) $^ -o $@
	$(SIZE) $@

# Compile
build/%.o: %.c | builddirs
	@echo "==> CC    $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Output directories
builddirs:
	@mkdir -p $(sort $(dir $(OBJS)))

# Generate .hex (Intel HEX)
$(TARGET).hex: $(TARGET).elf
	@echo "==> HEX   $@"
	$(OBJCOPY) -O ihex $< $@

# Generate .bin (raw binary)
$(TARGET).bin: $(TARGET).elf
	@echo "==> BIN   $@"
	$(OBJCOPY) -O binary $< $@

# Flash via ST-Link (st-flash)
flash: $(TARGET).bin
	@echo "==> FLASH $<"
	st-flash write $< 0x08000000

# Clean
clean:
	rm -rf build
	rm -f $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).map

# Include auto-generated dependency files
-include $(DEPS)
