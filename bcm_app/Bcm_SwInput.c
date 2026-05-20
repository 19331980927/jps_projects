/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2024, BCM Project.
 *
 * Bcm_SwInput.c
 * BCM Switch Input Module - reads and debounces all BCM switches.
 */

#include "Bcm_SwInput.h"
#include "Dio.h"

#define SW_INPUT_COUNT          8u
#define DEBOUNCE_THRESHOLD      5u  /* 连续采样 5 次一致才确认（10ms周期×5=50ms） */

static uint8 debounceCnt[SW_INPUT_COUNT];
static uint8 swStableState;
static uint8 swRawPrev;

void Bcm_SwInputInit(void)
{
    uint8 i;
    for (i = 0; i < SW_INPUT_COUNT; i++) {
        debounceCnt[i] = 0;
    }
    swStableState = 0;
    swRawPrev = 0;
}

/*
 * 软件去抖：连续 DEBOUNCE_THRESHOLD 次采样一致才更新状态
 * 返回稳定后的开关状态（按位表示）
 */
uint8 Bcm_SwInputRead(void)
{
    uint8 swRaw = 0;
    uint8 i;

    /* 读取硬件 IO */
    swRaw = (uint8)Dio_ReadChannel(0);

    for (i = 0; i < SW_INPUT_COUNT; i++) {
        uint8 rawBit = (swRaw >> i) & 0x01;
        uint8 prevBit = (swRawPrev >> i) & 0x01;

        if (rawBit == prevBit) {
            /* 状态稳定，计数递增 */
            if (debounceCnt[i] < DEBOUNCE_THRESHOLD) {
                debounceCnt[i]++;
                if (debounceCnt[i] >= DEBOUNCE_THRESHOLD) {
                    /* 确认状态 */
                    if (rawBit) {
                        swStableState |= (1u << i);
                    } else {
                        swStableState &= ~(1u << i);
                    }
                }
            }
        } else {
            /* 状态变化，重置计数 */
            debounceCnt[i] = 0;
        }
    }

    swRawPrev = swRaw;
    return swStableState;
}
