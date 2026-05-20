/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2024, BCM Project.
 *
 * Bcm_SwInput.h
 * BCM Switch Input Module - software debouncing
 */

#ifndef BCM_SWINPUT_H_
#define BCM_SWINPUT_H_

#include "Std_Types.h"

void Bcm_SwInputInit(void);
uint8 Bcm_SwInputRead(void);

#endif /* BCM_SWINPUT_H_ */
