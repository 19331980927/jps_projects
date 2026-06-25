#include "usart.h"

void USART1_Init(uint32_t baudrate)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

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

void USART1_SendByte(uint8_t data)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, data);
}

void USART1_SendString(char *str)
{
    while(*str)
    {
        USART1_SendByte(*str++);
    }
}

int fputc(int ch, FILE *f)
{
    USART1_SendByte((uint8_t)ch);
    return ch;
}

// USART1中断服务函数
void USART1_IRQHandler(void)
{
    extern uint8_t scan_mode;
#ifdef ENABLE_EXTENDED_FEATURES
    extern uint8_t display_mode;
    static char rx_buffer[128];
    static uint8_t rx_index = 0;
    extern void Process_Remote_Command(char *cmd);
#endif
    
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART1);
        
        // 简单命令解析
        if(data == 'A' || data == 'a')
        {
            scan_mode = 1;  // 自动模式
        }
        else if(data == 'M' || data == 'm')
        {
            scan_mode = 0;  // 手动模式
        }
        
#ifdef ENABLE_EXTENDED_FEATURES
        else if(data == 'D' || data == 'd')
        {
            display_mode = !display_mode;  // 切换显示模式
        }
        
        // JSON命令接收
        if(data == '{' || rx_index > 0)
        {
            rx_buffer[rx_index++] = data;
            if(data == '}' || rx_index >= 127)
            {
                rx_buffer[rx_index] = '\0';
                Process_Remote_Command(rx_buffer);
                rx_index = 0;
            }
        }
#endif
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}