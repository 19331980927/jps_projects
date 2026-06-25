#include "led.h"

#define LED_PIN GPIO_Pin_13
#define LED_PORT GPIOC

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = LED_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(LED_PORT, LED_PIN);  // 初始关闭
}

void LED_On(void)
{
    GPIO_SetBits(LED_PORT, LED_PIN);  // 反转：高电平亮
}

void LED_Off(void)
{
    GPIO_ResetBits(LED_PORT, LED_PIN);  // 反转：低电平灭
}

void LED_Toggle(void)
{
    if(GPIO_ReadOutputDataBit(LED_PORT, LED_PIN))
        GPIO_ResetBits(LED_PORT, LED_PIN);
    else
        GPIO_SetBits(LED_PORT, LED_PIN);
}