#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

#define KEY_PRESS 1
#define KEY_RELEASE 0

void Key_Init(void);
uint8_t Key_Scan(void);
uint8_t Key_IsPressed(void);

#endif
