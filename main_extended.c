#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "at24c64.h"
#include "ultrasonic.h"
#include "servo.h"
#include "key.h"
#include "led.h"
#include "json_parser.h"
#include <stdio.h>
#include <string.h>

/*
 * 硬件引脚分配：
 * LED: PC13
 * 按键: PA0 (外部中断)
 * 舵机SG90: PA8 (TIM1_CH1 PWM)
 * 超声波HC-SR04: Trig-PA2, Echo-PA3
 * OLED SSD1306: SCL-PB6, SDA-PB7 (I2C)
 * AT24C64: SCL-PB6, SDA-PB7 (I2C, 与OLED共用)
 * USART1: TX-PA10, RX-PA9 (波特率115200)
 */

// 全局变量
uint8_t scan_mode = 0;  // 0:手动, 1:自动扫描
uint8_t display_mode = 0;  // 0:文本显示, 1:雷达图显示
uint16_t current_angle = 90;
float current_distance = 0;
uint8_t warning_flag = 0;
uint16_t scan_data[181];
uint8_t remote_control = 0;  // 远程控制标志

// 函数声明
void System_Init(void);
void Auto_Scan(void);
void Manual_Control(void);
void OLED_Display(void);
void OLED_DrawRadarDisplay(void);
void Send_Data_To_NodeRED(void);
void Process_Remote_Command(char *cmd);
void Send_OLED_Screen_To_NodeRED(void);

int main(void)
{
    System_Init();
    
    printf("Extended System Init Complete!\r\n");
    OLED_ShowString(0, 0, "Extended Ready", 16);
    delay_ms(1000);
    
    while(1)
    {
        if(scan_mode == 1)
        {
            Auto_Scan();
        }
        else
        {
            Manual_Control();
        }
        
        if(display_mode == 0)
        {
            OLED_Display();
        }
        else
        {
            OLED_DrawRadarDisplay();
        }
        
        Send_Data_To_NodeRED();
        
        // 扩展功能：定期发送OLED屏幕内容到Node-RED
        if(remote_control)
        {
            Send_OLED_Screen_To_NodeRED();
        }
        
        delay_ms(100);
    }
}

void System_Init(void)
{
    Delay_Init();
    USART1_Init(115200);
    OLED_Init();
    AT24C64_Init();
    Ultrasonic_Init();
    Servo_Init();
    Key_Init();
    LED_Init();
    
    AT24C64_Read_Data(0, (uint8_t*)scan_data, sizeof(scan_data));
}

void Auto_Scan(void)
{
    static uint8_t direction = 0;
    static uint16_t angle = 0;
    
    if(direction == 0)
    {
        angle += 2;
        if(angle >= 180) direction = 1;
    }
    else
    {
        angle -= 2;
        if(angle <= 0) direction = 0;
    }
    
    Servo_SetAngle(angle);
    current_angle = angle;
    delay_ms(20);
    
    current_distance = Ultrasonic_GetDistance();
    scan_data[angle] = (uint16_t)(current_distance * 10);
    
    if(current_distance < 30 && current_distance > 0)
    {
        warning_flag = 1;
        LED_On();
    }
    else
    {
        warning_flag = 0;
        LED_Off();
    }
}

void Manual_Control(void)
{
    static uint8_t key_state = 0;
    
    if(Key_Scan() == KEY_PRESS)
    {
        key_state = !key_state;
        if(key_state)
        {
            current_angle += 10;
            if(current_angle > 180) current_angle = 0;
        }
        else
        {
            if(current_angle < 10) current_angle = 180;
            else current_angle -= 10;
        }
        Servo_SetAngle(current_angle);
        delay_ms(100);
        
        current_distance = Ultrasonic_GetDistance();
        scan_data[current_angle] = (uint16_t)(current_distance * 10);
        
        if(current_distance < 30 && current_distance > 0)
        {
            warning_flag = 1;
            LED_On();
        }
        else
        {
            warning_flag = 0;
            LED_Off();
        }
    }
}

// 扩展功能：OLED雷达图显示
void OLED_DrawRadarDisplay(void)
{
    OLED_Clear();
    
    // 在OLED上绘制雷达图
    OLED_DrawRadar(64, 32, 30, scan_data);
    
    // 显示当前角度和距离
    char buf[16];
    sprintf(buf, "A:%d D:%.0f", current_angle, current_distance);
    OLED_ShowString(0, 56, buf, 16);
}

void OLED_Display(void)
{
    char buf[32];
    
    OLED_Clear();
    
    sprintf(buf, "Angle: %d", current_angle);
    OLED_ShowString(0, 0, buf, 16);
    
    sprintf(buf, "Dist: %.1fcm", current_distance);
    OLED_ShowString(0, 16, buf, 16);
    
    if(scan_mode)
        OLED_ShowString(0, 32, "Mode: Auto", 16);
    else
        OLED_ShowString(0, 32, "Mode: Manual", 16);
    
    if(warning_flag)
        OLED_ShowString(0, 48, "WARNING!", 16);
    else
        OLED_ShowString(0, 48, "Safe", 16);
}

void Send_Data_To_NodeRED(void)
{
    char json_buf[128];
    
    sprintf(json_buf, "{\"angle\":%d,\"distance\":%.1f,\"warning\":%d,\"display_mode\":%d}", 
            current_angle, current_distance, warning_flag, display_mode);
    
    printf("%s\r\n", json_buf);
}

// 扩展功能：处理远程命令
void Process_Remote_Command(char *cmd)
{
    // 解析显示模式切换命令
    if(strstr(cmd, "\"display_mode\":1"))
    {
        display_mode = 1;
    }
    else if(strstr(cmd, "\"display_mode\":0"))
    {
        display_mode = 0;
    }
    
    // 解析远程控制开关
    if(strstr(cmd, "\"remote_control\":1"))
    {
        remote_control = 1;
    }
    else if(strstr(cmd, "\"remote_control\":0"))
    {
        remote_control = 0;
    }
    
    JSON_ParseCommand(cmd);
}

// 扩展功能：发送OLED屏幕内容到Node-RED
void Send_OLED_Screen_To_NodeRED(void)
{
    // 这里简化处理，实际可以发送OLED的显存数据
    printf("{\"oled_screen\":\"radar_mode\",\"angle\":%d,\"distance\":%.1f}\r\n", 
           current_angle, current_distance);
}

// 修改USART中断以支持远程命令
void USART1_IRQHandler(void)
{
    static char rx_buffer[128];
    static uint8_t rx_index = 0;
    
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART1);
        
        // 简单命令
        if(data == 'A' || data == 'a')
        {
            scan_mode = 1;
        }
        else if(data == 'M' || data == 'm')
        {
            scan_mode = 0;
        }
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
        
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}
