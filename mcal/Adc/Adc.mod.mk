# STM32F10x MCAL implementation
vpath-$(USE_ADC)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src
inc-$(USE_ADC)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src

# ADC Driver
obj-$(USE_ADC) += Adc.o

inc-$(USE_ADC) += $(ROOTDIR)/mcal/Adc/inc
vpath-$(USE_ADC) += $(ROOTDIR)/mcal/Adc/src
