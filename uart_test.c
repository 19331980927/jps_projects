#include "stm32f10x.h"

// 最简串口测试 - 只测试串口输出（不使用printf）

void USART1_Init_Simple(uint32_t baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

    // 配置TX引脚 PA9 (STM32F103 USART1标准TX引脚)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置RX引脚 PA10 (STM32F103 USART1标准RX引脚)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // USART配置
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;  // 只使用发送
    USART_Init(USART1, &USART_InitStructure);

    // 使能USART
    USART_Cmd(USART1, ENABLE);

    // 清除标志位
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_ClearFlag(USART1, USART_FLAG_TXE);
}

void USART_SendString(char *str)
{
    while(*str)
    {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *str++);
    }
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void Delay_Simple(uint32_t count)
{
    while(count--)
    {
        uint32_t i = 100000;
        while(i--);
    }
}

int main(void)
{
    USART1_Init_Simple(115200);
    Delay_Simple(20);  // 启动延时

    // 直接发送字节测试
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, 0x55);  // 发送0x55 (ASCII: U)
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    Delay_Simple(10);

    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, 0x41);  // 发送0x41 (ASCII: A)
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    Delay_Simple(10);

    while(1)
    {
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, 0x42);  // 发送0x42 (ASCII: B)
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        Delay_Simple(20);
    }
}
