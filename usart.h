#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

void USART1_Init(uint32_t baudrate);
void USART1_SendByte(uint8_t data);
void USART1_SendString(char *str);
int fputc(int ch, FILE *f);

#endif
