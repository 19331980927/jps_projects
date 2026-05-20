# STM32F10x MCAL implementation
vpath-$(USE_GPT)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src
inc-$(USE_GPT)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src

# GPT (General Purpose Timer) Driver
obj-$(USE_GPT) += Gpt.o

inc-$(USE_GPT) += $(ROOTDIR)/mcal/Gpt/inc
vpath-$(USE_GPT) += $(ROOTDIR)/mcal/Gpt/src
