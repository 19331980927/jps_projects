#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include <stdio.h>

// 简化测试程序 - 用于排查串口和OLED问题

int main(void)
{
    Delay_Init();
    USART1_Init(115200);
    OLED_Init();
    
    // 测试1：串口测试
    printf("=== Test Start ===\r\n");
    printf("USART Test: OK\r\n");
    
    // 测试2：LED闪烁测试
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    // 测试3：OLED测试
    OLED_Clear();
    OLED_ShowString(0, 0, "Test", 16);
    OLED_ShowString(0, 2, "123", 16);
    
    printf("OLED Test: OK\r\n");
    
    // 测试4：LED闪烁循环
    while(1)
    {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);  // LED On
        delay_ms(500);
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // LED Off
        delay_ms(500);
        
        printf("Loop: LED Toggle\r\n");
    }
}
