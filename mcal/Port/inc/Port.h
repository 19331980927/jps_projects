/*-------------------------------- Arctic Core ------------------------------
 * Port.h - AUTOSAR PORT Driver Interface
 */

#ifndef PORT_H_
#define PORT_H_

#include "Std_Types.h"
#include "Port_ConfigTypes.h"

typedef uint32 Port_PinType;

typedef enum {
    PORT_PIN_IN,
    PORT_PIN_OUT,
} Port_PinDirectionType;

typedef uint8 Port_PinModeType;

void Port_Init(const Port_ConfigType *ConfigPtr);
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode);
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo);

#endif /* PORT_H_ */
