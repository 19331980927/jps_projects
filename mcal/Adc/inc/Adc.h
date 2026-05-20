#ifndef ADC_H_
#define ADC_H_
#include "Std_Types.h"
typedef uint32 Adc_GroupType;
typedef uint32 Adc_ValueGroupType;
void Adc_StartGroupConversion(Adc_GroupType Group);
void Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType *DataBufferPtr);
#endif
