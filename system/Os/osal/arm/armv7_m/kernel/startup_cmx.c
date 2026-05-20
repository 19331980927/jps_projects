/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2013, ArcCore AB, Sweden, www.arccore.com.
 *
 * startup_cmx.c
 * ARM Cortex-M3/M4 startup code - vector table and reset handler.
 */

#include "Cpu.h"

#define STARTUP_STACK_SIZE        0x400

extern uint32 __STACK_TOP;
extern uint32 __DATA_ROM[];
extern uint32 __DATA_RAM[];
extern uint32 __DATA_END[];
extern uint32 __BSS_START[];
extern uint32 __BSS_END[];

void Reset_Handler(void);
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void);
void SysTick_Handler(void);
void Default_Handler(void);

extern int main(void);

__attribute__((used, section(".isr_vector")))
void (* const vector_table[])(void) = {
    (void (*)(void))&__STACK_TOP,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0, 0, 0, 0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
};

void Reset_Handler(void)
{
    uint32 *src, *dest;
    uint32 len;

    src = __DATA_ROM;
    dest = __DATA_RAM;
    if (src != dest) {
        len = (uint32)__DATA_END - (uint32)__DATA_RAM;
        while (len--) {
            *dest++ = *src++;
        }
    }

    dest = __BSS_START;
    len = (uint32)__BSS_END - (uint32)__BSS_START;
    while (len--) {
        *dest++ = 0;
    }

    main();
    while (1);
}

void Default_Handler(void) { while (1); }

void HardFault_Handler(void) { while (1); }
