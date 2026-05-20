# STM32F10x MCAL implementation
vpath-$(USE_PWM)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src
inc-$(USE_PWM)-$(CFG_STM32F1X) += $(ROOTDIR)/mcal/arch/stm32/src

# PWM Driver
obj-$(USE_PWM) += Pwm.o

inc-$(USE_PWM) += $(ROOTDIR)/mcal/Pwm/inc
vpath-$(USE_PWM) += $(ROOTDIR)/mcal/Pwm/src
