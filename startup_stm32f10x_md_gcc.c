/*============================================================================*/
/* startup_stm32f10x_md_gcc.c — STM32F10X_MD vector table & weak handlers    */
/*                                                                            */
/* GCC-compatible C startup file (replaces the ARMCC assembly file            */
/* start/startup_stm32f10x_md.s when building with arm-none-eabi-gcc).        */
/*                                                                            */
/* Keil uVision5 continues to use start/startup_stm32f10x_md.s unchanged.     */
/*============================================================================*/

#include <stdint.h>

/*----------------------------------------------------------------------------
  Externals — defined in the linker script STM32F103C8.ld
 *----------------------------------------------------------------------------*/
extern void _estack(void);          /* Top of stack (alias for address) */
extern uint32_t _sidata;            /* Start of .data load region in FLASH */
extern uint32_t _sdata;             /* Start of .data in RAM */
extern uint32_t _edata;             /* End of .data in RAM */
extern uint32_t _sbss;              /* Start of .bss in RAM */
extern uint32_t _ebss;              /* End of .bss in RAM */

/*----------------------------------------------------------------------------
  Forward declarations — implemented in other compilation units
 *----------------------------------------------------------------------------*/
extern int main(void);
extern void SystemInit(void);

/*----------------------------------------------------------------------------
  Core exception handlers — forward declarations
 *----------------------------------------------------------------------------*/
void Reset_Handler(void);
void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
  External interrupt handlers (STM32F10X_MD = Medium Density)
  All weak — overridden by strong definitions in stm32f10x_it.c
 *----------------------------------------------------------------------------*/
void WWDG_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void PVD_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void TAMPER_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void RTC_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void FLASH_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void RCC_IRQHandler(void)              __attribute__((weak, alias("Default_Handler")));
void EXTI0_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void EXTI1_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void EXTI2_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void EXTI3_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void EXTI4_IRQHandler(void)            __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel1_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel2_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel3_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel4_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel5_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel6_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void DMA1_Channel7_IRQHandler(void)    __attribute__((weak, alias("Default_Handler")));
void ADC1_2_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void USB_HP_CAN1_TX_IRQHandler(void)   __attribute__((weak, alias("Default_Handler")));
void USB_LP_CAN1_RX0_IRQHandler(void)  __attribute__((weak, alias("Default_Handler")));
void CAN1_RX1_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void CAN1_SCE_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void EXTI9_5_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM1_BRK_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void TIM1_UP_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM1_TRG_COM_IRQHandler(void)     __attribute__((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void TIM2_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void TIM3_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void TIM4_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void I2C1_EV_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void I2C1_ER_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void I2C2_EV_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void I2C2_ER_IRQHandler(void)          __attribute__((weak, alias("Default_Handler")));
void SPI1_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void SPI2_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void USART1_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void USART2_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void USART3_IRQHandler(void)           __attribute__((weak, alias("Default_Handler")));
void EXTI15_10_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));
void RTCAlarm_IRQHandler(void)         __attribute__((weak, alias("Default_Handler")));
void USBWakeUp_IRQHandler(void)        __attribute__((weak, alias("Default_Handler")));

/* Time-base reserved handler */
void TIM5_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void TIM6_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));
void TIM7_IRQHandler(void)             __attribute__((weak, alias("Default_Handler")));

/*----------------------------------------------------------------------------
  Vector table — placed at the beginning of flash via .isr_vector section

  Layout: [0] = initial SP, [1..15] = core exceptions, [16..58] = IRQs
  Total: 1 SP + 15 core + 43 IRQ = 59 entries
 *----------------------------------------------------------------------------*/
__attribute__((section(".isr_vector"), used))
void (* const g_pfnVectors[])(void) = {
    /* Core vectors 0..15 */
    &_estack,                          /* 0: Stack pointer (top of RAM)    */
    Reset_Handler,                     /* 1: Reset                         */
    NMI_Handler,                       /* 2: NMI                           */
    HardFault_Handler,                 /* 3: Hard fault                    */
    MemManage_Handler,                 /* 4: MPU fault                     */
    BusFault_Handler,                  /* 5: Bus fault                     */
    UsageFault_Handler,                /* 6: Usage fault                   */
    0,                                 /* 7: Reserved                      */
    0,                                 /* 8: Reserved                      */
    0,                                 /* 9: Reserved                      */
    0,                                 /*10: Reserved                      */
    SVC_Handler,                       /*11: SVCall                        */
    DebugMon_Handler,                  /*12: Debug monitor                 */
    0,                                 /*13: Reserved                      */
    PendSV_Handler,                    /*14: PendSV                        */
    SysTick_Handler,                   /*15: SysTick                       */

    /* External interrupts 16..58 (IRQ 0..42) */
    WWDG_IRQHandler,                   /*16: Window Watchdog               */
    PVD_IRQHandler,                    /*17: PVD through EXTI              */
    TAMPER_IRQHandler,                 /*18: Tamper                        */
    RTC_IRQHandler,                    /*19: RTC                           */
    FLASH_IRQHandler,                  /*20: Flash                         */
    RCC_IRQHandler,                    /*21: RCC                           */
    EXTI0_IRQHandler,                  /*22: EXTI Line 0                   */
    EXTI1_IRQHandler,                  /*23: EXTI Line 1                   */
    EXTI2_IRQHandler,                  /*24: EXTI Line 2                   */
    EXTI3_IRQHandler,                  /*25: EXTI Line 3                   */
    EXTI4_IRQHandler,                  /*26: EXTI Line 4                   */
    DMA1_Channel1_IRQHandler,          /*27: DMA1 Channel 1                */
    DMA1_Channel2_IRQHandler,          /*28: DMA1 Channel 2                */
    DMA1_Channel3_IRQHandler,          /*29: DMA1 Channel 3                */
    DMA1_Channel4_IRQHandler,          /*30: DMA1 Channel 4                */
    DMA1_Channel5_IRQHandler,          /*31: DMA1 Channel 5                */
    DMA1_Channel6_IRQHandler,          /*32: DMA1 Channel 6                */
    DMA1_Channel7_IRQHandler,          /*33: DMA1 Channel 7                */
    ADC1_2_IRQHandler,                 /*34: ADC1 & ADC2                   */
    USB_HP_CAN1_TX_IRQHandler,         /*35: USB High Prio / CAN1 TX       */
    USB_LP_CAN1_RX0_IRQHandler,        /*36: USB Low Prio / CAN1 RX0       */
    CAN1_RX1_IRQHandler,               /*37: CAN1 RX1                      */
    CAN1_SCE_IRQHandler,               /*38: CAN1 SCE                      */
    EXTI9_5_IRQHandler,                /*39: EXTI Line 5..9                */
    TIM1_BRK_IRQHandler,               /*40: TIM1 Break                    */
    TIM1_UP_IRQHandler,                /*41: TIM1 Update                   */
    TIM1_TRG_COM_IRQHandler,           /*42: TIM1 Trigger / Commutation    */
    TIM1_CC_IRQHandler,                /*43: TIM1 Capture Compare          */
    TIM2_IRQHandler,                   /*44: TIM2                          */
    TIM3_IRQHandler,                   /*45: TIM3                          */
    TIM4_IRQHandler,                   /*46: TIM4                          */
    I2C1_EV_IRQHandler,               /*47: I2C1 Event                    */
    I2C1_ER_IRQHandler,               /*48: I2C1 Error                    */
    I2C2_EV_IRQHandler,               /*49: I2C2 Event                    */
    I2C2_ER_IRQHandler,               /*50: I2C2 Error                    */
    SPI1_IRQHandler,                   /*51: SPI1                          */
    SPI2_IRQHandler,                   /*52: SPI2                          */
    USART1_IRQHandler,                 /*53: USART1                        */
    USART2_IRQHandler,                 /*54: USART2                        */
    USART3_IRQHandler,                 /*55: USART3                        */
    EXTI15_10_IRQHandler,              /*56: EXTI Line 10..15              */
    RTCAlarm_IRQHandler,               /*57: RTC Alarm through EXTI        */
    USBWakeUp_IRQHandler,              /*58: USB Wakeup through EXTI       */
};

/*----------------------------------------------------------------------------
  Default_Handler — infinite loop, used as the weak fallback for all handlers
 *----------------------------------------------------------------------------*/
void Default_Handler(void)
{
    while (1)
        ;
}

/*----------------------------------------------------------------------------
  Reset_Handler — entry point after CPU reset

  1. Copy .data from FLASH to RAM
  2. Zero-fill .bss
  3. Call SystemInit()  (clock configuration)
  4. Call main()
  5. Infinite loop (main should never return)
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
    uint32_t *src, *dst;

    /* Copy initialized data (.data) from FLASH to RAM */
    for (src = &_sidata, dst = &_sdata; dst < &_edata; src++, dst++)
        *dst = *src;

    /* Zero-fill .bss section */
    for (dst = &_sbss; dst < &_ebss; dst++)
        *dst = 0;

    /* Configure system clock */
    SystemInit();

    /* Call the user application */
    main();

    /* If main returns, loop forever */
    while (1)
        ;
}
