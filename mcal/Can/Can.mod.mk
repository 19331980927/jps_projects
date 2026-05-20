# STM32F10x MCAL implementation
vpath-$(USE_CAN)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src
inc-$(USE_CAN)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src

# CAN Driver
obj-$(USE_CAN) += Can.o

inc-$(USE_CAN) += $(ROOTDIR)/mcal/Can/inc
vpath-$(USE_CAN) += $(ROOTDIR)/mcal/Can/src
