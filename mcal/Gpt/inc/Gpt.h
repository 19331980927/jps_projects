#ifndef GPT_H_
#define GPT_H_
#include "Std_Types.h"
typedef uint32 Gpt_ChannelType;
typedef uint32 Gpt_ValueType;
void Gpt_StartTimer(Gpt_ChannelType Channel, Gpt_ValueType Value);
void Gpt_StopTimer(Gpt_ChannelType Channel);
#endif
