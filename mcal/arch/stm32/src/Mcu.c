/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2013, ArcCore AB, Sweden, www.arccore.com.
 *
 * Mcu.c - STM32F10x MCU Driver implementation.
 * Uses STM32F10x Standard Peripheral Library.
 */

#include "Mcu.h"
#include "Mcu_ConfigTypes.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"

/* System clock: 72MHz using HSE + PLL */
#define HSE_VALUE               ((uint32_t)8000000)
#define SYSCLK_FREQ_72MHz       72000000

static uint32 Mcu_ClockSpeed = 0;

/*----------------------------------------------------------------------------
 * Mcu_Init
 * Initialize the MCU hardware (clock, PLL, flash latency).
 *----------------------------------------------------------------------------*/
void Mcu_Init(const Mcu_ConfigType *ConfigPtr)
{
    (void)ConfigPtr;

    /* Reset RCC configuration */
    RCC_DeInit();

    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_WaitForHSEStartUp() != SUCCESS);

    /* Flash: 2 wait states for 72MHz */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK (72MHz max) */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 (36MHz max) */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLL: 8MHz HSE * 9 = 72MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
    RCC_PLLCmd(ENABLE);
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08);

    Mcu_ClockSpeed = SYSCLK_FREQ_72MHz;

    /* Enable required peripheral clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
}

/*----------------------------------------------------------------------------
 * Mcu_GetVersionInfo
 *----------------------------------------------------------------------------*/
void Mcu_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    if (versioninfo != NULL) {
        versioninfo->vendorID = 0;
        versioninfo->moduleID = 0;
        versioninfo->sw_major_version = 1;
        versioninfo->sw_minor_version = 0;
        versioninfo->sw_patch_version = 0;
    }
}

/*----------------------------------------------------------------------------
 * Mcu_GetPllStatus
 *----------------------------------------------------------------------------*/
Mcu_PllStatusType Mcu_GetPllStatus(void)
{
    return (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == SET) ?
           MCU_PLL_LOCKED : MCU_PLL_UNLOCKED;
}

/*----------------------------------------------------------------------------
 * Mcu_PerformReset
 *----------------------------------------------------------------------------*/
void Mcu_PerformReset(void)
{
    NVIC_SystemReset();
}
