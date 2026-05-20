/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2013, ArcCore AB, Sweden, www.arccore.com.
 *
 * Dio.c - STM32F10x DIO Driver (Digital I/O).
 * Uses STM32F10x StdPeriph Driver for register access.
 */

#include "Dio.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

/*----------------------------------------------------------------------------
 * Dio_WriteChannel
 * Set a single DIO channel to HIGH or LOW.
 *----------------------------------------------------------------------------*/
void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level)
{
    /* ChannelId encodes port (bits 8-15) and pin (bits 0-7) */
    GPIO_TypeDef *port;
    uint16 pin;

    port = (GPIO_TypeDef *)(GPIOA_BASE + ((ChannelId >> 8) & 0xFF) * 0x0400);
    pin = (uint16)(1u << (ChannelId & 0x0F));

    if (Level == STD_HIGH) {
        GPIO_SetBits(port, pin);
    } else {
        GPIO_ResetBits(port, pin);
    }
}

/*----------------------------------------------------------------------------
 * Dio_ReadChannel
 * Read a single DIO channel level.
 *----------------------------------------------------------------------------*/
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId)
{
    GPIO_TypeDef *port;
    uint16 pin;

    port = (GPIO_TypeDef *)(GPIOA_BASE + ((ChannelId >> 8) & 0xFF) * 0x0400);
    pin = (uint16)(1u << (ChannelId & 0x0F));

    return (GPIO_ReadInputDataBit(port, pin) != RESET) ? STD_HIGH : STD_LOW;
}

/*----------------------------------------------------------------------------
 * Dio_ReadChannelGroup
 * Read a group of DIO channels.
 *----------------------------------------------------------------------------*/
void Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr,
                          Dio_PortLevelType *PortLevelPtr)
{
    GPIO_TypeDef *port;

    if (ChannelGroupIdPtr == NULL || PortLevelPtr == NULL) return;

    port = (GPIO_TypeDef *)(GPIOA_BASE +
           ((ChannelGroupIdPtr->port >> 8) & 0xFF) * 0x0400);

    *PortLevelPtr = (Dio_PortLevelType)GPIO_ReadInputData(port);
}

/*----------------------------------------------------------------------------
 * Dio_WriteChannelGroup
 * Write to a group of DIO channels.
 *----------------------------------------------------------------------------*/
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr,
                           Dio_PortLevelType Level)
{
    GPIO_TypeDef *port;

    if (ChannelGroupIdPtr == NULL) return;

    port = (GPIO_TypeDef *)(GPIOA_BASE +
           ((ChannelGroupIdPtr->port >> 8) & 0xFF) * 0x0400);

    GPIO_Write(port, (uint16_t)Level);
}

/*----------------------------------------------------------------------------
 * Dio_GetVersionInfo
 *----------------------------------------------------------------------------*/
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    if (versioninfo != NULL) {
        versioninfo->vendorID = 0;
        versioninfo->moduleID = 0;
        versioninfo->sw_major_version = 1;
        versioninfo->sw_minor_version = 0;
        versioninfo->sw_patch_version = 0;
    }
}
