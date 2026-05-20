/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2024, BCM Project.
 *
 * Bcm_Lights.h
 * BCM Lights Control Module
 */

#ifndef BCM_LIGHTS_H_
#define BCM_LIGHTS_H_

#include "Std_Types.h"

/* 灯光控制枚举 */
typedef enum {
    BCM_LIGHT_OFF = 0,
    BCM_LIGHT_LOW_BEAM,
    BCM_LIGHT_HIGH_BEAM,
    BCM_LIGHT_TURN_LEFT,
    BCM_LIGHT_TURN_RIGHT,
    BCM_LIGHT_HAZARD,
    BCM_LIGHT_DRL,          /* 日间行车灯 */
    BCM_LIGHT_FOG_FRONT,
    BCM_LIGHT_FOG_REAR,
    BCM_LIGHT_INTERIOR,
    BCM_LIGHT_BRAKE
} Bcm_LightType;

void Bcm_LightsInit(void);
void Bcm_LightsControl(uint8 swInputMask);

#endif /* BCM_LIGHTS_H_ */
