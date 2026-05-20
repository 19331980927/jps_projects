# STM32F10x MCAL implementation
vpath-$(USE_DIO)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src
inc-$(USE_DIO)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src

# DIO Driver
obj-$(USE_DIO) += Dio.o

inc-$(USE_DIO) += $(ROOTDIR)/mcal/Dio/inc
vpath-$(USE_DIO) += $(ROOTDIR)/mcal/Dio/src
