#include "servo.h"
#include "delay.h"

#define SERVO_PIN GPIO_Pin_8
#define SERVO_PORT GPIOA

void Servo_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);
    
    // 配置PWM输出引脚 PA8 (TIM1_CH1)
    GPIO_InitStructure.GPIO_Pin = SERVO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SERVO_PORT, &GPIO_InitStructure);
    
    // TIM1时基配置
    // 50Hz PWM频率
    // 72MHz / 72 = 1MHz
    // 1MHz / 20000 = 50Hz
    TIM_TimeBaseStructure.TIM_Period = 19999;  // ARR
    TIM_TimeBaseStructure.TIM_Prescaler = 71;  // PSC
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    // TIM1_CH1 PWM配置
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1500;  // 中间位置
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);  // TIM1需要主输出使能
    TIM_Cmd(TIM1, ENABLE);
    
    Servo_SetAngle(90);  // 初始化到中间位置
}

void Servo_SetAngle(uint16_t angle)
{
    uint16_t pulse;
    
    if(angle > 180) angle = 180;
    
    // SG90舵机控制
    // 0度: 0.5ms脉冲
    // 90度: 1.5ms脉冲
    // 180度: 2.5ms脉冲
    // PWM周期20ms
    // pulse = (angle * 11.11) + 500
    pulse = (angle * 11) + 500;
    
    TIM_SetCompare1(TIM1, pulse);
}