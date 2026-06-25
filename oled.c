#include "oled.h"
#include "delay.h"
#include "font.h"
#include <math.h>
#include <stdlib.h>

extern const unsigned char F8X16[];
uint8_t OLED_Buffer[128 * 8] = {0};

#define OLED_ADDRESS 0x78

// I2C引脚定义
#define OLED_SCL GPIO_Pin_6
#define OLED_SDA GPIO_Pin_7
#define OLED_I2C_PORT GPIOB

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

uint8_t I2C_ReceiveByte(void)
{
    uint8_t i, dat = 0;
    GPIO_SetBits(OLED_I2C_PORT, OLED_SDA);
    for(i = 0; i < 8; i++)
    {
        dat <<= 1;
        GPIO_SetBits(OLED_I2C_PORT, OLED_SCL);
        delay_us(2);
        if(GPIO_ReadInputDataBit(OLED_I2C_PORT, OLED_SDA))
            dat |= 0x01;
        GPIO_ResetBits(OLED_I2C_PORT, OLED_SCL);
        delay_us(2);
    }
    return dat;
}

void I2C_SendAck(void)
{
    GPIO_ResetBits(OLED_I2C_PORT, OLED_SDA);
    GPIO_SetBits(OLED_I2C_PORT, OLED_SCL);
    delay_us(5);
    GPIO_ResetBits(OLED_I2C_PORT, OLED_SCL);
}

void I2C_SendNoAck(void)
{
    GPIO_SetBits(OLED_I2C_PORT, OLED_SDA);
    GPIO_SetBits(OLED_I2C_PORT, OLED_SCL);
    delay_us(5);
    GPIO_ResetBits(OLED_I2C_PORT, OLED_SCL);
}

static void OLED_WriteCmd(uint8_t cmd)
{
    I2C_Start();
    I2C_SendByte(OLED_ADDRESS);
    I2C_WaitAck();
    I2C_SendByte(0x00);
    I2C_WaitAck();
    I2C_SendByte(cmd);
    I2C_WaitAck();
    I2C_Stop();
}

static void OLED_WriteData(uint8_t dat)
{
    I2C_Start();
    I2C_SendByte(OLED_ADDRESS);
    I2C_WaitAck();
    I2C_SendByte(0x40);
    I2C_WaitAck();
    I2C_SendByte(dat);
    I2C_WaitAck();
    I2C_Stop();
}

// 设置页地址模式下的位置
static void OLED_SetPagePos(uint8_t x, uint8_t page)
{
    OLED_WriteCmd(0xB0 + page);            // 设置页地址
    OLED_WriteCmd(((x & 0xF0) >> 4) | 0x10); // 列高4位
    OLED_WriteCmd(x & 0x0F);               // 列低4位
}

// 逐页刷新整个屏幕缓冲区（页地址模式）
static void OLED_Flush(void)
{
    uint8_t page, col;
    for(page = 0; page < 8; page++)
    {
        OLED_SetPagePos(0, page);
        for(col = 0; col < 128; col++)
            OLED_WriteData(OLED_Buffer[page * 128 + col]);
    }
}

void OLED_Refresh(void)
{
    OLED_Flush();
}

void OLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = OLED_SCL | OLED_SDA;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_I2C_PORT, &GPIO_InitStructure);

    GPIO_SetBits(OLED_I2C_PORT, OLED_SCL);
    GPIO_SetBits(OLED_I2C_PORT, OLED_SDA);

    delay_ms(200);

    OLED_WriteCmd(0xAE); // 关显示
    OLED_WriteCmd(0xD5); // 时钟分频
    OLED_WriteCmd(0x80);
    OLED_WriteCmd(0xA8); // 复用率
    OLED_WriteCmd(0x3F); // 1/64
    OLED_WriteCmd(0xD3); // 显示偏移
    OLED_WriteCmd(0x00);
    OLED_WriteCmd(0x40); // 起始行=0
    OLED_WriteCmd(0x8D); // 电荷泵
    OLED_WriteCmd(0x14); // 使能
    OLED_WriteCmd(0x20); // 寻址模式
    OLED_WriteCmd(0x02); // 页地址模式
    OLED_WriteCmd(0xA1); // 段重映射: 左右反转(127→0)
    OLED_WriteCmd(0xC8); // COM扫描方向: 上下反转
    OLED_WriteCmd(0xDA); // COM引脚配置
    OLED_WriteCmd(0x12);
    OLED_WriteCmd(0x81); // 对比度
    OLED_WriteCmd(0xCF);
    OLED_WriteCmd(0xD9); // 预充电周期
    OLED_WriteCmd(0xF1);
    OLED_WriteCmd(0xDB); // VCOMH电压
    OLED_WriteCmd(0x40);
    OLED_WriteCmd(0xA4); // 显示内容=RAM
    OLED_WriteCmd(0xA6); // 正常显示(非反色)
    OLED_WriteCmd(0xAF); // 开显示
    OLED_Clear();
}

void OLED_Clear(void)
{
    uint16_t i;
    for(i = 0; i < 128 * 8; i++)
        OLED_Buffer[i] = 0;
    OLED_Flush();
}

void OLED_SetPos(uint8_t x, uint8_t y)
{
    OLED_SetPagePos(x, y);
}

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size)
{
    uint8_t c, i;
    uint16_t index;
    c = chr - ' ';
    if(x > 127) return;
    if(y > 6) return;

    index = y * 128 + x;
    for(i = 0; i < 8; i++)
        OLED_Buffer[index + i] = F8X16[c * 16 + i];

    index = (y + 1) * 128 + x;
    for(i = 0; i < 8; i++)
        OLED_Buffer[index + i] = F8X16[c * 16 + i + 8];
}

void OLED_ShowString(uint8_t x, uint8_t y, char *str, uint8_t size)
{
    uint8_t j = 0;
    while(str[j] != '\0')
    {
        OLED_ShowChar(x, y, str[j], size);
        x += 8;
        if(x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;

    for(t = 0; t < len; t++)
    {
        temp = (num / OLED_Pow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                OLED_ShowChar(x + (size / 2) * t, y, ' ', size);
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (size / 2) * t, y, temp + '0', size);
    }
}

void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t mode)
{
    uint8_t page, bit;
    uint16_t index;

    if(x >= 128 || y >= 64) return;

    page = y / 8;
    bit = 1 << (y % 8);
    index = page * 128 + x;

    if(mode)
        OLED_Buffer[index] |= bit;
    else
        OLED_Buffer[index] &= ~bit;
}

void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t mode)
{
    int dx, dy, sx, sy, err, e2;

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    sx = (x1 < x2) ? 1 : -1;
    sy = (y1 < y2) ? 1 : -1;
    err = dx - dy;

    while(1)
    {
        OLED_DrawPoint(x1, y1, mode);
        if(x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if(e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        if(e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}

void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r, uint8_t mode)
{
    int a, b;
    int di;

    a = 0;
    b = r;
    di = 3 - 2 * r;

    while(a <= b)
    {
        OLED_DrawPoint(x + b, y + a, mode);
        OLED_DrawPoint(x + b, y - a, mode);
        OLED_DrawPoint(x - b, y + a, mode);
        OLED_DrawPoint(x - b, y - a, mode);
        OLED_DrawPoint(x + a, y + b, mode);
        OLED_DrawPoint(x + a, y - b, mode);
        OLED_DrawPoint(x - a, y + b, mode);
        OLED_DrawPoint(x - a, y - b, mode);
        a++;
        if(di < 0)
            di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}

// OLED画雷达图（带持久化扫描轨迹）
void OLED_DrawRadar(uint8_t cx, uint8_t cy, uint8_t radius, uint16_t *data,
                    uint16_t scan_angle, uint16_t *trail, uint8_t trail_cnt,
                    uint16_t max_range)
{
    uint8_t i;
    float angle, dist;
    int x, y, len;
    float scale = (float)radius / max_range;  // 像素/cm 比例

    // 清除雷达区域
    for(i = 0; i < radius * 2; i++)
        OLED_DrawLine(cx - radius, cy - radius + i, cx + radius, cy - radius + i, 0);

    // 画同心圆
    OLED_DrawCircle(cx, cy, radius, 1);
    OLED_DrawCircle(cx, cy, radius / 2, 1);

    // 画扫描轨迹点
    for(i = 0; i < trail_cnt; i++)
    {
        angle = trail[i] * 3.14159 / 180.0;
        dist = data[trail[i]] / 10.0 * scale;   // cm → 像素
        if(dist <= 0 || dist > radius) dist = radius;
        len = (int)dist;
        x = (int)cx + (int)(len * cos(angle));
        y = (int)cy - (int)(len * sin(angle));
        if(x >= 0 && x < 128 && y >= 0 && y < 64)
            OLED_DrawPoint(x, y, 1);
    }

    // 画障碍物点
    for(i = 0; i <= 180; i++)
    {
        if(data[i] > 0 && data[i] < 2000)
        {
            angle = i * 3.14159 / 180.0;
            dist = data[i] / 10.0 * scale;       // cm → 像素
            if(dist > radius) dist = radius;
            x = (int)cx + (int)(dist * cos(angle));
            y = (int)cy - (int)(dist * sin(angle));
            if(x >= 0 && x < 128 && y >= 0 && y < 64)
                OLED_DrawPoint(x, y, 1);
        }
    }

    // 画当前扫描线
    angle = scan_angle * 3.14159 / 180.0;
    x = (int)cx + (int)(radius * cos(angle));
    y = (int)cy - (int)(radius * sin(angle));
    if(x >= 0 && x < 128 && y >= 0 && y < 64)
        OLED_DrawLine(cx, cy, x, y, 1);
}
