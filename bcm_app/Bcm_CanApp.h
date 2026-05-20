/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2024, BCM Project.
 *
 * Bcm_CanApp.h
 * BCM CAN Application Layer - message Tx/Rx handling
 */

#ifndef BCM_CANAPP_H_
#define BCM_CANAPP_H_

#include "Std_Types.h"

void Bcm_CanAppInit(void);
void Bcm_CanAppRxProcess(void);
void Bcm_CanAppTxProcess(void);

#endif /* BCM_CANAPP_H_ */
