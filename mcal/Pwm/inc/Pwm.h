#ifndef PWM_H_
#define PWM_H_
#include "Std_Types.h"
#include "Pwm_ConfigTypes.h"
void Pwm_SetDutyCycle(Pwm_ChannelType ChannelNumber, uint16 DutyCycle);
void Pwm_SetPeriodAndDuty(Pwm_ChannelType ChannelNumber, Pwm_PeriodType Period, uint16 DutyCycle);
#endif
