
# Arctic Core - ARM Cortex-M3 GCC compiler settings
# For STM32F103 (Cortex-M3)

# Architecture
cflags-y += -mcpu=cortex-m3 -mthumb

# No FPU for Cortex-M3
cflags-y += -msoft-float

# Linker flags
LDFLAGS-y += -mcpu=cortex-m3 -mthumb

# Use standard specs for embedded
LDFLAGS-y += -specs=nosys.specs -specs=nano.specs

# Use GCC for linking (not raw ld) to get startup files + interworking
LD = $(CROSS_COMPILE)gcc

# inc and vpath for OS headers
inc-y += $(ROOTDIR)/system/Os/include
vpath-y += $(ROOTDIR)/system/Os/osal/arm/armv7_m/kernel
inc-y += $(ROOTDIR)/system/Os/osal/arm/armv7_m/kernel
