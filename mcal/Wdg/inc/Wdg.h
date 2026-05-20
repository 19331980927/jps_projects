#ifndef WDG_H_
#define WDG_H_
#include "Std_Types.h"
typedef struct { uint32 dummy; } Wdg_IWDG_ConfigType;
typedef struct { uint32 dummy; } Wdg_WWDG_ConfigType;
typedef struct { uint32 dummy; } Wdg_GeneralType;
void Wdg_Trigger(void);
#endif
