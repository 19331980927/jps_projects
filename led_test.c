#include "stm32f10x.h"

// 最简LED测试 - 确认程序是否运行

void Delay_Simple(uint32_t count)
{
    while(count--)
    {
        uint32_t i = 50000;
        while(i--);
    }
}

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    // 配置PC13为推挽输出
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    while(1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);  // LED On
        Delay_Simple(10);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // LED Off
        Delay_Simple(10);
    }
}
