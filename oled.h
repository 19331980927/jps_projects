#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t mode);
void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode);
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t mode);
void OLED_DrawRadar(uint8_t cx, uint8_t cy, uint8_t radius, uint16_t *data,
                    uint16_t scan_angle, uint16_t *trail, uint8_t trail_cnt,
                    uint16_t max_range);
void OLED_SetPos(uint8_t x, uint8_t y);
void OLED_Refresh(void);

#endif
