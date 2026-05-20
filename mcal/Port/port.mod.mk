# STM32F10x MCAL implementation
vpath-$(USE_PORT)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src
inc-$(USE_PORT)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src

# GPIO Port Driver
obj-$(USE_PORT) += Port.o

inc-$(USE_PORT) += $(ROOTDIR)/mcal/Port/inc
vpath-$(USE_PORT) += $(ROOTDIR)/mcal/Port/src
