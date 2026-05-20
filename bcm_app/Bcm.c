/*-------------------------------- Arctic Core ------------------------------
 * Bcm.c - BCM Application Main Module
 */

#include "Bcm.h"
#include "Bcm_Lights.h"
#include "Bcm_SwInput.h"
#include "Bcm_CanApp.h"

static uint8 Bcm_Initialized = 0;

void Bcm_Init(void) {
    if (Bcm_Initialized) return;
    Bcm_LightsInit();
    Bcm_SwInputInit();
    Bcm_CanAppInit();
    Bcm_Initialized = 1;
}

void Bcm_MainFunction(void) {
    uint8 swState;
    if (!Bcm_Initialized) return;
    swState = Bcm_SwInputRead();
    Bcm_CanAppRxProcess();
    Bcm_LightsControl(swState);
    Bcm_CanAppTxProcess();
}
