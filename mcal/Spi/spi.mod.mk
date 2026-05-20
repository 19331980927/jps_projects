# STM32F10x MCAL implementation
vpath-$(USE_SPI)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src
inc-$(USE_SPI)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src

# SPI Driver
obj-$(USE_SPI) += Spi.o

inc-$(USE_SPI) += $(ROOTDIR)/mcal/Spi/inc
vpath-$(USE_SPI) += $(ROOTDIR)/mcal/Spi/src
