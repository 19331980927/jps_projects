#ifndef CAN_H_
#define CAN_H_
#include "Std_Types.h"
typedef uint32 Can_HwHandleType;
typedef struct { uint32 id; uint8 length; uint8 data[8]; } Can_PduType;
void Can_Write(Can_HwHandleType Hth, const Can_PduType *PduInfo);
#endif
