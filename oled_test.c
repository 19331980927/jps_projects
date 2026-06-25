	#include "stm32f10x.h"
#include "delay.h"

// 简化OLED测试 - 只测试I2C通信和基本显示

#define OLED_SCL GPIO_Pin_6
#define OLED_SDA GPIO_Pin_7
#define OLED_I2C_PORT GPIOB

void I2C_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = OLED_SCL | OLED_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_I2C_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(OLED_I2C_PORT, OLED_SCL);
    GPIO_SetBits(OLED_I2C_PORT, OLED_SDA);
}

void I2C_Start(void)
{
    GPIO_SetBits(OLED_I2C_PORT, OLED_SCL);
    GPIO_SetBits(OLED_I2C_PORT, OLED_SDA);
    delay_us(5);
    GPIO_ResetBits(OLED_I2C_PORT, OLED_SDA);
    delay_us(5);
    GPIO_ResetBits(OLED_I2C_PORT, OLED_SCL);
}

void I2C_Stop(void)
{
    GPIO_ResetBits(OLED_I2C_PORT, OLED_SCL);
    GPIO_ResetBits(OLED_I2C_PORT, OLED_SDA);
    delay_us(5);
    GPIO_SetBits(OLED_I2C_PORT, OLED_SCL);
    delay_us(5);
    GPIO_SetBits(OLED_I2C_PORT, OLED_SDA);
}

void I2C_SendByte(uint8_t dat)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        GPIO_ResetBits(OLED_I2C_PORT, OLED_SCL);
        if(dat & 0x80)
            GPIO_SetBits(OLED_I2C_PORT, OLED_SDA);
        else
            GPIO_ResetBits(OLED_I2C_PORT, OLED_SDA);
        delay_us(2);
        GPIO_SetBits(OLED_I2C_PORT, OLED_SCL);
        delay_us(2);
        dat <<= 1;
    }
    GPIO_ResetBits(OLED_I2C_PORT, OLED_SCL);
}

void I2C_WaitAck(void)
{
    GPIO_SetBits(OLED_I2C_PORT, OLED_SDA);
    GPIO_SetBits(OLED_I2C_PORT, OLED_SCL);
    delay_us(5);
    GPIO_ResetBits(OLED_I2C_PORT, OLED_SCL);
}

void OLED_WriteCmd(uint8_t cmd)
{
    I2C_Start();
    I2C_SendByte(0x78);
    I2C_WaitAck();
    I2C_SendByte(0x00);
    I2C_WaitAck();
    I2C_SendByte(cmd);
    I2C_WaitAck();
    I2C_Stop();
}

void OLED_WriteData(uint8_t dat)
{
    I2C_Start();
    I2C_SendByte(0x78);
    I2C_WaitAck();
    I2C_SendByte(0x40);
    I2C_WaitAck();
    I2C_SendByte(dat);
    I2C_WaitAck();
    I2C_Stop();
}

void OLED_Init(void)
{
    delay_ms(200);
    
    OLED_WriteCmd(0xAE); // 关显示
    OLED_WriteCmd(0xD5); // 设置时钟分频因子
    OLED_WriteCmd(0x80);
    OLED_WriteCmd(0xA8); // 设置驱动路数
    OLED_WriteCmd(0x3F);
    OLED_WriteCmd(0xD3); // 设置显示偏移
    OLED_WriteCmd(0x00);
    OLED_WriteCmd(0x40); // 设置起始行
    OLED_WriteCmd(0x8D); // 电荷泵设置
    OLED_WriteCmd(0x14);
    OLED_WriteCmd(0x20); // 设置内存地址模式
    OLED_WriteCmd(0x02);
    OLED_WriteCmd(0xA1); // 段重定向
    OLED_WriteCmd(0xC8); // 设置COM扫描方向
    OLED_WriteCmd(0xDA); // 设置COM硬件引脚配置
    OLED_WriteCmd(0x12);
    OLED_WriteCmd(0x81); // 对比度设置
    OLED_WriteCmd(0xCF);
    OLED_WriteCmd(0xD9); // 设置预充电周期
    OLED_WriteCmd(0xF1);
    OLED_WriteCmd(0xDB); // 设置VCOMH电压倍率
    OLED_WriteCmd(0x40);
    OLED_WriteCmd(0xA4); // 全局显示开启
    OLED_WriteCmd(0xA6); // 设置显示方式
    OLED_WriteCmd(0xAF); // 开启显示
    
    OLED_Clear();
}

void OLED_Clear(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        OLED_WriteCmd(0xB0 + i);
        OLED_WriteCmd(0x01);
        OLED_WriteCmd(0x10);
        for(n = 0; n < 128; n++)
        {
            OLED_WriteData(0);
        }
    }
}

void OLED_Fill(uint8_t dat)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        OLED_WriteCmd(0xB0 + i);
        OLED_WriteCmd(0x01);
        OLED_WriteCmd(0x10);
        for(n = 0; n < 128; n++)
        {
            OLED_WriteData(dat);
        }
    }
}

int main(void)
{
    Delay_Init();
    I2C_GPIO_Init();
    OLED_Init();
    
    // 测试：全屏填充
    OLED_Fill(0xFF);  // 全白
    delay_ms(1000);
    
    OLED_Fill(0x00);  // 全黑
    delay_ms(1000);
    
    // 测试：交替填充
    while(1)
    {
        OLED_Fill(0xAA);  // 灰色
        delay_ms(500);
        OLED_Fill(0x55);  // 灰色
        delay_ms(500);
    }
}
