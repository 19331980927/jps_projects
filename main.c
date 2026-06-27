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

// 扩展功能开关
#define ENABLE_EXTENDED_FEATURES

// 全局变量
uint8_t scan_mode = 0;  // 0:手动, 1:自动扫描
uint16_t current_angle = 90;  // 当前角度
float current_distance = 0;   // 当前距离
uint8_t warning_flag = 0;    // 预警标志
uint8_t blocked_flag = 0;    // 遮挡标志: 1=模块被遮挡
uint16_t scan_data[181];      // 扫描数据缓存(0-180度)

uint8_t display_mode = 1;  // 0:文本显示, 1:雷达图显示
uint8_t remote_control = 0;  // 远程控制标志
uint8_t clear_flag = 0;      // 清屏通知Node-RED

// 扫描轨迹（保留最近扫描过的角度，形成扇形）
uint16_t scan_trail[90];
uint8_t trail_count = 0;
uint8_t trail_head = 0;

// 函数声明
void System_Init(void);
void Auto_Scan(void);
void Manual_Control(void);
void Save_Scan_Data(void);
void OLED_Display(void);
void Send_Data_To_NodeRED(void);

void OLED_DrawRadarDisplay(void);

// 遮挡检测：连续近距离（< 8cm）判定传感器被遮挡
// HC-SR04 在 3cm 以内已进入盲区边缘，可信度很低
static uint8_t check_blocked(float raw)
{
    static uint8_t blocked_cnt = 0;   // 连续近距离计数
    static uint8_t ok_cnt = 0;        // 正常值计数（解除用）

    uint8_t is_close = (raw > 0 && raw < 8.0f) ? 1 : 0;
    uint8_t is_invalid = (raw < 2.0f) ? 1 : 0;  // 盲区

    if (is_invalid || is_close)
    {
        // 本次读数在遮挡区间
        if (blocked_flag)
        {
            // 已遮挡：忽略一切
            ok_cnt = 0;
            return 1;
        }
        blocked_cnt++;
        if (blocked_cnt >= 3)
        {
            blocked_flag = 1;
            blocked_cnt = 0;
            ok_cnt = 0;
            return 1;
        }
        // 还没到阈值，允许回退旧值
        return 0;
    }

    // 本次有正常回波
    if (blocked_flag)
    {
        ok_cnt++;
        if (ok_cnt >= 3)
        {
            blocked_flag = 0;
            ok_cnt = 0;
        }
        return 1;  // 还没解除，仍然忽略
    }

    // 正常状态
    blocked_cnt = 0;
    return 0;
}

// LED blink — 每次调用翻转，~0.5 秒周期（亮 0.25s / 灭 0.25s）
static void led_blink_toggle(void)
{
    LED_Toggle();
}
// 清除所有扫描数据（含EEPROM清零）
void Clear_Scan_Data(void)
{
    uint16_t i;
    for(i = 0; i < 181; i++)
        scan_data[i] = 0;
    trail_count = 0;
    trail_head = 0;
    AT24C64_Write_Data(0, (uint8_t*)scan_data, sizeof(scan_data));
}

// 仅清空内存（不动EEPROM），长按保存后使用
void Reset_Scan_Data(void)
{
    uint16_t i;
    for(i = 0; i < 181; i++)
        scan_data[i] = 0;
    trail_count = 0;
    trail_head = 0;
}

int main(void)
{
    uint16_t hold_cnt = 0;

    System_Init();

    printf("System Init Complete!\r\n");
    delay_ms(1000);

    while(1)
    {
        // 按键处理：短按切模式，长按2秒清除数据
        if(Key_IsPressed())
        {
            hold_cnt = 0;
            while(Key_IsPressed())
            {
                hold_cnt++;
                delay_ms(20);
                if(hold_cnt >= 100) break;  // 2秒上限
            }

            if(hold_cnt >= 100)
            {
                // 长按2秒：保存EEPROM + 清屏重新开始
                Save_Scan_Data();
                Reset_Scan_Data();
                clear_flag = 1;
                OLED_Clear();
                OLED_ShowString(16, 2, "DATA SAVED", 16);
                OLED_Refresh();
                delay_ms(1000);
            }
            else if(hold_cnt >= 2)
            {
                // 短按：切换模式
                scan_mode = !scan_mode;
            }
        }

        if(scan_mode == 1)
        {
            Auto_Scan();
        }
        else
        {
            // 手动模式：检测串口命令设定的角度变化
            static uint16_t last_remote_angle = 999;
            Manual_Control();
            if(current_angle != last_remote_angle)
            {
                last_remote_angle = current_angle;
                Servo_SetAngle(current_angle);
                delay_ms(80);
                float raw = Ultrasonic_GetDistance();
                if(raw >= 2.0f && raw <= 250.0f)
                {
                    current_distance = raw;
                    scan_data[current_angle] = (uint16_t)(raw * 10);
                }
                scan_trail[trail_head] = current_angle;
                trail_head = (trail_head + 1) % 90;
                if(trail_count < 90) trail_count++;
            }
        }

        if(display_mode == 1)
            OLED_DrawRadarDisplay();
        else
            OLED_Display();

        Send_Data_To_NodeRED();
        delay_ms(100);
    }
}

// 系统初始化
void System_Init(void)
{
    Delay_Init();
    USART1_Init(115200);
    OLED_Init();
    AT24C64_Init();
    Ultrasonic_Init();
    Servo_Init();  // 启用舵机
    Key_Init();  // 启用按键
    LED_Init();
    LED_Off();  // 确保LED初始状态为关闭

    // 读取上次保存的数据
    AT24C64_Read_Data(0, (uint8_t*)scan_data, sizeof(scan_data));
}

// 自动扫描模式
void Auto_Scan(void)
{
    static uint8_t direction = 0;  // 0:正向, 1:反向
    static uint16_t angle = 0;
    
    if(direction == 0)
    {
        angle += 2;
        if(angle >= 180) { direction = 1; }
    }
    else
    {
        angle -= 2;
        if(angle <= 0) { direction = 0; }
    }
    
    Servo_SetAngle(angle);
    current_angle = angle;
    delay_ms(60);  // 等待舵机稳定

    // 读取超声波，遮挡检测
    {
        float raw = Ultrasonic_GetDistance();
        if (check_blocked(raw))
        {
            // 传感器被遮挡：忽略读数，LED 闪烁
            led_blink_toggle();
            warning_flag = 1;
        }
        else if (raw >= 2.0f && raw <= 250.0f)
        {
            current_distance = raw;
            scan_data[angle] = (uint16_t)(raw * 10);
            if (raw < 30.0f)
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
        else if (scan_data[angle] > 0 && scan_data[angle] <= 2500)
        {
            // 读不到或超限，沿用该角度历史值
            current_distance = scan_data[angle] / 10.0f;
            warning_flag = 0;
            LED_Off();
        }
        else
        {
            current_distance = 0;
            warning_flag = 0;
            LED_Off();
        }
    }

    printf("SCAN a:%d d:%.1f dir:%d blk:%d\r\n", angle, current_distance, direction, blocked_flag);
}

// 手动控制模式
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

        // 读取超声波，遮挡检测
        {
            float raw = Ultrasonic_GetDistance();
            if (check_blocked(raw))
            {
                led_blink_toggle();
                warning_flag = 1;
            }
            else if (raw >= 2.0f && raw <= 250.0f)
            {
                current_distance = raw;
                scan_data[current_angle] = (uint16_t)(raw * 10);
                if (raw < 30.0f)
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
            else if (scan_data[current_angle] > 0 && scan_data[current_angle] <= 2500)
            {
                current_distance = scan_data[current_angle] / 10.0f;
                warning_flag = 0;
                LED_Off();
            }
            else
            {
                current_distance = 0;
                warning_flag = 0;
                LED_Off();
            }
        }
    }
}

// OLED显示
void OLED_Display(void)
{
    static uint16_t last_angle = 0;
    static float last_distance = 0;
    static uint8_t last_warning = 0;
    char buf[22];

    // 只在数据变化时更新，避免闪烁
    if(last_angle != current_angle || last_distance != current_distance || last_warning != warning_flag)
    {
        OLED_Clear();

        // 第0-1行：角度
        sprintf(buf, "ANG  %3d", current_angle);
        OLED_ShowString(0, 0, buf, 16);

        // 第2-3行：距离（被遮挡时显示 BLOCKED）
        if (blocked_flag)
            OLED_ShowString(0, 2, "BLOCKED!", 16);
        else
        {
            sprintf(buf, "DIS  %3d", (int)current_distance);
            OLED_ShowString(0, 2, buf, 16);
        }

        // 第4-5行：模式
        if(scan_mode)
            OLED_ShowString(0, 4, "MODE AUTO", 16);
        else
            OLED_ShowString(0, 4, "MODE MANU", 16);

        // 第6-7行：警告
        if(warning_flag)
            OLED_ShowString(0, 6, "WARN ON ", 16);
        else
            OLED_ShowString(0, 6, "WARN OFF", 16);

        OLED_Refresh();

        last_angle = current_angle;
        last_distance = current_distance;
        last_warning = warning_flag;
    }
}

// 发送数据到Node-RED
void Send_Data_To_NodeRED(void)
{
    char json_buf[128];
    uint8_t clr = clear_flag;
    clear_flag = 0;
    
    sprintf(json_buf, "{\"angle\":%d,\"distance\":%.1f,\"warning\":%d,\"blocked\":%d,\"display_mode\":%d,\"clear\":%d}",
            current_angle, current_distance, warning_flag, blocked_flag, display_mode, clr);
    
    printf("%s\r\n", json_buf);
}

// 保存扫描数据到AT24C64
void Save_Scan_Data(void)
{
    AT24C64_Write_Data(0, (uint8_t*)scan_data, sizeof(scan_data));
}

// OLED雷达图显示
void OLED_DrawRadarDisplay(void)
{
    // 记录扫描轨迹（环形缓冲区，最多存90个角度）
    scan_trail[trail_head] = current_angle;
    trail_head = (trail_head + 1) % 90;
    if(trail_count < 90) trail_count++;

    OLED_Clear();

    OLED_DrawRadar(64, 32, 30, scan_data, current_angle, scan_trail, trail_count, 200);

    char buf[16];
    if (blocked_flag)
        OLED_ShowString(0, 0, "BLOCKED!", 16);
    else
    {
        sprintf(buf, "A:%d D:%.0f", current_angle, current_distance);
        OLED_ShowString(0, 0, buf, 16);
    }
    OLED_Refresh();
}

// 处理远程命令
void Process_Remote_Command(char *cmd)
{
    printf("CMD:%s\r\n", cmd);
    
    JSON_ParseCommand(cmd);
    
    // 收到任何角度命令都直接驱动舵机
    if(strstr(cmd, "\"angle\":"))
    {
        printf("ANGLE CMD: set servo to %d\r\n", current_angle);
        Servo_SetAngle(current_angle);
    }
}