
# ARCH defines
ARCH=armv7_m
ARCH_FAM=arm
ARCH_MCU=stm32

#
# CFG (y/n) macros
#

CFG=ARM ARMV7_M ARM_CM3 THUMB
CFG+=STM32 STM32F1X

# Add our board
CFG+=BRD_STM32F103_BCM
CFG+=TIMER_DWT
CFG+=CREATE_SREC

#
# ST have devided devices into ( See chapter 6 in Ref manual )
# LD - Low Density.    STM32F101xx,F102xx,F103xx). Flash 16->32Kbytes
# MD - Medium Density. Same as above.              Flash 64->128Kbytes
# HD - High Density.   STM32F101xx,F103xx.         Flash 256->512Kbytes
# CL - Connectivity Line. STM32F105xx,F107xx
#
# Uncomment the density matching your chip:
CFG+=STM32_HD
# CFG+=STM32_MD

# What buildable modules does this board have,
# default or private

# MCAL (BCM essential drivers)
MOD_AVAIL+=ADC CAN DIO MCU FLS PORT PWM GPT EA BCM_APP

# Required modules
MOD_USE += MCU KERNEL DIO PORT BCM_APP

#
# Extra defines
#

# Select the right device in ST header files.
# [ STM32F10X_LD | STM32F10X_MD | STM32F10X_HD | STM32F10X_CL ]
# Keep in sync with the CFG density selection above.
def-y += STM32F10X_HD
# def-y += STM32F10X_MD

# Default cross compiler
DEFAULT_CROSS_COMPILE = /opt/arm-none-eabi/bin/arm-none-eabi-

def-y += L_BOOT_RESERVED_SPACE=0x0
