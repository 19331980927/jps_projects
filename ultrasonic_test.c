#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>

// 桩变量（usart.c / json_parser.c 引用的全局变量）
uint8_t scan_mode = 0;
uint16_t current_angle = 0;

// 最简单的超声波测试，不依赖OLED/舵机/AT24C64
// Trig: PA2, Echo: PA3

#define TRIG_PIN GPIO_Pin_2
#define ECHO_PIN GPIO_Pin_3
#define ULTRASONIC_PORT GPIOA

void Ultrasonic_Test_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    GPIO_ResetBits(ULTRASONIC_PORT, TRIG_PIN);
}

float Ultrasonic_Test_GetDistance(void)
{
    uint32_t timeout = 0;
    uint32_t time = 0;

    // 发10us触发脉冲
    GPIO_SetBits(ULTRASONIC_PORT, TRIG_PIN);
    delay_us(10);
    GPIO_ResetBits(ULTRASONIC_PORT, TRIG_PIN);

    // 等ECHO变高
    timeout = 0;
    while(GPIO_ReadInputDataBit(ULTRASONIC_PORT, ECHO_PIN) == RESET)
    {
        if(++timeout > 100000) return -1;  // 没收到回波
    }

    // 开始计时
    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);

    // 等ECHO变低
    timeout = 0;
    while(GPIO_ReadInputDataBit(ULTRASONIC_PORT, ECHO_PIN) == SET)
    {
        if(++timeout > 300000) { TIM_Cmd(TIM3, DISABLE); return -2; }
    }

    TIM_Cmd(TIM3, DISABLE);
    time = TIM_GetCounter(TIM3);

    return time * 0.017f;
}

int main(void)
{
    float dist;

    Delay_Init();
    USART1_Init(115200);
    Ultrasonic_Test_Init();

    printf("\r\n=== Ultrasonic Test ===\r\n");
    printf("Waiting 2s for sensor to stabilize...\r\n");
    delay_ms(2000);

    while(1)
    {
        dist = Ultrasonic_Test_GetDistance();

        if(dist == -1)
            printf("NO ECHO (check wiring & 5V power)\r\n");
        else if(dist == -2)
            printf("TIMEOUT (object too far or ECHO stuck high)\r\n");
        else
            printf("Distance: %.1f cm\r\n", dist);

        delay_ms(500);
    }
}
