#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H

#include "stm32f10x.h"

#define ULTRASONIC_MAX_RANGE  250.0f
#define ULTRASONIC_MIN_RANGE  2.0f
#define ULTRASONIC_SAMPLES    7       /* 每次测距采样次数，密集度聚类滤波 */

void Ultrasonic_Init(void);
float Ultrasonic_GetDistance(void);

#endif
