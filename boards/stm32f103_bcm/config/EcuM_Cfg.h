/* EcuM Configuration for STM32F103 BCM */
#ifndef ECUM_CFG_H_
#define ECUM_CFG_H_

#include "Std_Types.h"

#define ECUM_VERSION_INFO_API           STD_OFF
#define ECUM_DEV_ERROR_DETECT           STD_OFF

#define ECUM_MAIN_FUNCTION_PERIOD       (200)
#define ECUM_NVRAM_READALL_TIMEOUT      (10000)
#define ECUM_NVRAM_WRITEALL_TIMEOUT     (10000)
#define ECUM_NVRAM_MIN_RUN_DURATION     (10000)

/* Wakeup source defines */
#define ECUM_WKSOURCE_POWER             (0x01u)
#define ECUM_WKSOURCE_RESET             (0x02u)
#define ECUM_WKSOURCE_INTERNAL_RESET    (0x04u)
#define ECUM_WKSOURCE_INTERNAL_WDG      (0x08u)
#define ECUM_WKSOURCE_EXTERNAL_WDG      (0x10u)

/* Validation */
#define ECUM_VALIDATION_TIMEOUT         (100)

typedef struct {
    uint32 dummy;
} EcuM_ConfigType;

#endif
