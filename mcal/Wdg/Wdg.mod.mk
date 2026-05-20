# STM32F10x MCAL implementation
vpath-$(USE_WDG)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src
inc-$(USE_WDG)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src

# WDG (Watchdog) Driver
obj-$(USE_WDG) += Wdg.o

inc-$(USE_WDG) += $(ROOTDIR)/mcal/Wdg/inc
vpath-$(USE_WDG) += $(ROOTDIR)/mcal/Wdg/src
