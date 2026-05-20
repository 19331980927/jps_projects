/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2013, ArcCore AB, Sweden, www.arccore.com.
 *
 * arm_cortex_mx_handlers.c
 * ARM Cortex-M3 exception and interrupt handlers for the OS.
 */

#include "Cpu.h"

/* OS Tick interval: 1ms = 1000Hz */
#define OS_TICK_US              1000

/*----------------------------------------------------------------------------
 * SysTick Handler - OS system tick
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void)
{
    /* Increment the OS tick counter */
    extern void Os_Tick(void);
    Os_Tick();

    /* Clear SysTick interrupt flag (automatic on read) */
    (void)0;
}

/*----------------------------------------------------------------------------
 * PendSV Handler - OS context switch
 *----------------------------------------------------------------------------*/
void PendSV_Handler(void)
{
    /* Trigger OS scheduler */
    extern void Os_Schedule(void);
    Os_Schedule();
}

/*----------------------------------------------------------------------------
 * SVC Handler - Supervisor call
 *----------------------------------------------------------------------------*/
void SVC_Handler(void)
{
    /* Get the SVC number from the instruction */
    __asm volatile (
        "   tst     lr, #4                                    \n"
        "   ite     eq                                          \n"
        "   mrseq   r0, msp                                    \n"
        "   mrsne   r0, psp                                    \n"
        "   ldr     r1, [r0, #24]                              \n"
        "   ldrb    r0, [r1, #-2]                              \n"
        ::: "r0", "r1"
    );
}

/*----------------------------------------------------------------------------
 * OS callout stubs
 * These are called by the OS kernel, weak so they can be overridden.
 *----------------------------------------------------------------------------*/
void __attribute__((weak)) Os_Callout_Startup(void) { }
void __attribute__((weak)) Os_Callout_Shutdown(void) { while (1); }
void __attribute__((weak)) Os_Callout_PreTask(void) { }
void __attribute__((weak)) Os_Callout_PostTask(void) { }
void __attribute__((weak)) Os_Callout_Idle(void) { }
