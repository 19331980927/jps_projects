#ifndef __FONT_H
#define __FONT_H

#include "stm32f10x.h"

// 8x16字库
extern const unsigned char F8X16[];

// 辅助函数
uint32_t OLED_Pow(uint8_t m, uint8_t n);

#endif
