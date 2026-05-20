# STM32F10x MCAL implementation
vpath-$(USE_MCU)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src
inc-$(USE_MCU)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src

# MCU Driver
obj-$(USE_MCU) += Mcu.o
obj-$(USE_MCU) += Mcu_Lcfg.o

inc-$(USE_MCU) += $(ROOTDIR)/mcal/Mcu/inc
vpath-$(USE_MCU) += $(ROOTDIR)/mcal/Mcu/src
