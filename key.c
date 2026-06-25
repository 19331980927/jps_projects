#include "key.h"
#include "delay.h"

#define KEY_PIN GPIO_Pin_0
#define KEY_PORT GPIOA

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 仅配置按键为上拉输入，不使用中断
    GPIO_InitStructure.GPIO_Pin = KEY_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY_PORT, &GPIO_InitStructure);
}

uint8_t Key_IsPressed(void)
{
    return (GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == RESET) ? 1 : 0;
}

uint8_t Key_Scan(void)
{
    if(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == RESET)
    {
        delay_ms(10);
        if(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == RESET)
        {
            while(GPIO_ReadInputDataBit(KEY_PORT, KEY_PIN) == RESET);
            return KEY_PRESS;
        }
    }
    return KEY_RELEASE;
}