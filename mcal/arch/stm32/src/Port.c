/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2013, ArcCore AB, Sweden, www.arccore.com.
 *
 * Port.c - STM32F10x PORT Driver (GPIO pin configuration).
 * Uses STM32F10x StdPeriph Driver.
 */

#include "Port.h"
#include "Port_ConfigTypes.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/*----------------------------------------------------------------------------
 * Port_Init
 * Initialize all GPIO pins based on the configuration.
 *----------------------------------------------------------------------------*/
void Port_Init(const Port_ConfigType *ConfigPtr)
{
    (void)ConfigPtr;

    /* Default: all GPIO ports are configured as input with pull-up */
    GPIO_InitTypeDef gpioInit;

    gpioInit.GPIO_Pin  = GPIO_Pin_All;
    gpioInit.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOA, &gpioInit);
    GPIO_Init(GPIOB, &gpioInit);
    GPIO_Init(GPIOC, &gpioInit);
    GPIO_Init(GPIOD, &gpioInit);

    /* Enable USART1 for debug console */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
}

/*----------------------------------------------------------------------------
 * Port_SetPinDirection
 *----------------------------------------------------------------------------*/
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    GPIO_TypeDef *port;
    uint16 pin;
    GPIOMode_TypeDef mode;

    port = (GPIO_TypeDef *)(GPIOA_BASE + ((Pin >> 8) & 0xFF) * 0x0400);
    pin = (uint16)(1u << (Pin & 0x0F));

    mode = (Direction == PORT_PIN_IN) ?
           GPIO_Mode_IN_FLOATING : GPIO_Mode_Out_PP;

    GPIO_InitTypeDef gpioInit;
    gpioInit.GPIO_Pin   = pin;
    gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
    gpioInit.GPIO_Mode  = mode;
    GPIO_Init(port, &gpioInit);
}

/*----------------------------------------------------------------------------
 * Port_GetVersionInfo
 *----------------------------------------------------------------------------*/
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    if (versioninfo != NULL) {
        versioninfo->vendorID = 0;
        versioninfo->moduleID = 0;
        versioninfo->sw_major_version = 1;
        versioninfo->sw_minor_version = 0;
        versioninfo->sw_patch_version = 0;
    }
}
