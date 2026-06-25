#ifndef __AT24C64_H
#define __AT24C64_H

#include "stm32f10x.h"

void AT24C64_Init(void);
void AT24C64_WriteByte(uint16_t addr, uint8_t dat);
uint8_t AT24C64_ReadByte(uint16_t addr);
void AT24C64_Write_Data(uint16_t addr, uint8_t *buf, uint16_t len);
void AT24C64_Read_Data(uint16_t addr, uint8_t *buf, uint16_t len);

#endif
