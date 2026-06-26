#include "ultrasonic.h"
#include "delay.h"

#define TRIG_PIN         GPIO_Pin_2
#define ECHO_PIN         GPIO_Pin_3
#define ULTRASONIC_PORT  GPIOA

/*--------------------------------------------------------------------
  冒泡排序（通用，7 个元素最多 21 次比较）
 *--------------------------------------------------------------------*/
static void sort_u32(uint32_t *v, int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (v[j] > v[j + 1])
                { uint32_t t = v[j]; v[j] = v[j + 1]; v[j + 1] = t; }
}

/*--------------------------------------------------------------------
  单次触发 + 回波计时，返回计数值 (1 tick = 1 us)，超时返回 0
 *--------------------------------------------------------------------*/
static uint32_t ultrasonic_read_raw(void)
{
    uint32_t timeout;

    GPIO_SetBits (ULTRASONIC_PORT, TRIG_PIN);
    delay_us(10);
    GPIO_ResetBits(ULTRASONIC_PORT, TRIG_PIN);

    timeout = 0;
    while (GPIO_ReadInputDataBit(ULTRASONIC_PORT, ECHO_PIN) == RESET)
        if (++timeout > 100000) return 0;

    TIM_SetCounter(TIM3, 0);
    TIM_Cmd(TIM3, ENABLE);

    timeout = 0;
    while (GPIO_ReadInputDataBit(ULTRASONIC_PORT, ECHO_PIN) == SET)
        if (++timeout > 300000) { TIM_Cmd(TIM3, DISABLE); return 0; }

    TIM_Cmd(TIM3, DISABLE);
    uint32_t ticks = TIM_GetCounter(TIM3);

    if (ticks < 120) return 0;   /* < 2cm 盲区噪声 */
    return ticks;
}

void Ultrasonic_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = TRIG_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin   = ECHO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
    GPIO_Init(ULTRASONIC_PORT, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period        = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler     = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    GPIO_ResetBits(ULTRASONIC_PORT, TRIG_PIN);
}

/*--------------------------------------------------------------------
  Ultrasonic_GetDistance — 密集度聚类滤波

  核心思路：
    真实距离的多次回波会密集聚集在一个小范围内，
    而噪声/多径回波则是散乱分布的。

  做法：
    1. 采样 N 次（7 次）
    2. 剔零后排序
    3. 在排序数组中滑动 3 元素窗口，找到极差最小的窗口
    4. 对这个窗口的 3 个值取均值

  举例（真实 4cm，噪声 42cm）：
    raw = [4, 42, 4, 42, 4, 4, 43]
    sorted = [4, 4, 4, 4, 42, 42, 43]
    窗口 [4,4,4] spread=0 → 选中 → 均值 4cm ✓

  即使 4 个好值 + 3 个坏值，也能正确选中好值的集群。
 *--------------------------------------------------------------------*/
float Ultrasonic_GetDistance(void)
{
    uint32_t valid[ULTRASONIC_SAMPLES];
    int vc = 0;     /* 有效采样计数 */

    /* ---- 多次采样 ---- */
    for (int i = 0; i < ULTRASONIC_SAMPLES; i++)
    {
        uint32_t t = ultrasonic_read_raw();
        if (t > 0)
            valid[vc++] = t;

        if (i < ULTRASONIC_SAMPLES - 1)
            delay_ms(20);   /* HC-SR04 最小间隔 ~15ms */
    }

    /* 有效数不够 3 个 → 返回 0 */
    if (vc < 3)
        return 0;

    /* ---- 排序 ---- */
    sort_u32(valid, vc);

    /* ---- 寻找最密集的 3 元素窗口 ---- */
    int best_i = 0;
    uint32_t best_spread = 0xFFFFFFFF;

    for (int i = 0; i <= vc - 3; i++)
    {
        uint32_t spread = valid[i + 2] - valid[i];
        if (spread < best_spread)
        {
            best_spread = spread;
            best_i = i;
        }
    }

    /* ---- 取最密集 3 点的均值 ---- */
    uint32_t sum = valid[best_i] + valid[best_i + 1] + valid[best_i + 2];
    float cm = (sum / 3.0f) * 0.017f;

    /* ---- 范围约束 ---- */
    if (cm < ULTRASONIC_MIN_RANGE || cm > ULTRASONIC_MAX_RANGE)
        return 0;

    return cm;
}
