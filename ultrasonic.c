#include "ultrasonic.h"
#include "delay.h"

#define TRIG_PIN GPIO_Pin_2
#define ECHO_PIN GPIO_Pin_3
#define ULTRASONIC_PORT GPIOA

void Ultrasonic_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // TRIG引脚配置为输出 PA2
    GPIO_InitStructure.GPIO_Pin = TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStructure);

    // ECHO引脚配置为输入 PA3
    GPIO_InitStructure.GPIO_Pin = ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStructure);
    
    // TIM3配置用于测距计时
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;  // 1MHz计数
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    GPIO_ResetBits(ULTRASONIC_PORT, TRIG_PIN);
}

float Ultrasonic_GetDistance(void)
{
    uint32_t time = 0;
    float distance;
    
    // 发送10us触发脉冲
    GPIO_SetBits(ULTRASONIC_PORT, TRIG_PIN);
    delay_us(10);
    GPIO_ResetBits(ULTRASONIC_PORT, TRIG_PIN);
    
    // 等待回响信号上升沿（增加超时保护）
    uint32_t timeout = 0;
    while(GPIO_ReadInputDataBit(ULTRASONIC_PORT, ECHO_PIN) == RESET)
    {
        timeout++;
        if(timeout > 100000)  // 等待上升沿超时
        {
            return 0;
        }
    }

    // 启动定时器
    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);

    // 等待回响信号下降沿
    while(GPIO_ReadInputDataBit(ULTRASONIC_PORT, ECHO_PIN) == SET)
    {
        time++;
        if(time > 300000)  // 超时保护（约40ms，支持4m+测距）
        {
            TIM_Cmd(TIM3, DISABLE);
            return 0;
        }
    }
    
    // 停止定时器
    TIM_Cmd(TIM3, DISABLE);
    time = TIM_GetCounter(TIM3);
    
    // 计算距离: 距离 = 时间 * 声速 / 2
    // 声速 = 340m/s = 34000cm/s
    // 计数频率1MHz, 每个计数1us
    // distance = time * 0.034 / 2 = time * 0.017 cm
    distance = time * 0.017f;

    return distance;
}
	