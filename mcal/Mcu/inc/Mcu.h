/*-------------------------------- Arctic Core ------------------------------
 * Mcu.h - AUTOSAR MCU Driver Interface Header
 */

#ifndef MCU_H_
#define MCU_H_

#include "Std_Types.h"
#include "Mcu_ConfigTypes.h"

typedef enum {
    MCU_PLL_LOCKED,
    MCU_PLL_UNLOCKED,
} Mcu_PllStatusType;

typedef struct {
    uint32 dummy;
} Mcu_ConfigType;

void Mcu_Init(const Mcu_ConfigType *ConfigPtr);
void Mcu_GetVersionInfo(Std_VersionInfoType *versioninfo);
Mcu_PllStatusType Mcu_GetPllStatus(void);
void Mcu_PerformReset(void);

#endif /* MCU_H_ */
