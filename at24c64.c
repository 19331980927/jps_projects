#include "at24c64.h"
#include "delay.h"
#include "oled.h"  // 使用OLED的I2C函数

#define AT24C64_ADDRESS 0xA0

// 外部声明OLED的I2C函数
extern void I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_SendByte(uint8_t dat);
extern uint8_t I2C_ReceiveByte(void);
extern void I2C_WaitAck(void);
extern void I2C_SendAck(void);
extern void I2C_SendNoAck(void);

void AT24C64_Init(void)
{
    // I2C已在OLED_Init中初始化，这里不需要重复初始化
}

void AT24C64_WriteByte(uint16_t addr, uint8_t dat)
{
    I2C_Start();
    I2C_SendByte(AT24C64_ADDRESS);
    I2C_WaitAck();
    I2C_SendByte(addr >> 8);
    I2C_WaitAck();
    I2C_SendByte(addr & 0xFF);
    I2C_WaitAck();
    I2C_SendByte(dat);
    I2C_WaitAck();
    I2C_Stop();
    delay_ms(10);
}

uint8_t AT24C64_ReadByte(uint16_t addr)
{
    uint8_t dat;
    
    I2C_Start();
    I2C_SendByte(AT24C64_ADDRESS);
    I2C_WaitAck();
    I2C_SendByte(addr >> 8);
    I2C_WaitAck();
    I2C_SendByte(addr & 0xFF);
    I2C_WaitAck();
    
    I2C_Start();
    I2C_SendByte(AT24C64_ADDRESS | 0x01);
    I2C_WaitAck();
    dat = I2C_ReceiveByte();
    I2C_SendNoAck();
    I2C_Stop();
    
    return dat;
}

void AT24C64_Write_Data(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        AT24C64_WriteByte(addr + i, buf[i]);
    }
}

void AT24C64_Read_Data(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        buf[i] = AT24C64_ReadByte(addr + i);
    }
}