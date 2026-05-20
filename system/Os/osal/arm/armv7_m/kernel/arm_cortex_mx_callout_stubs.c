/* ARM Cortex-M3 OS Callout Stubs */
#include "Os.h"

void __attribute__((weak)) Os_Callout_Startup(void) { }
void __attribute__((weak)) Os_Callout_Shutdown(void) { while (1); }
void __attribute__((weak)) Os_Callout_PreTask(void) { }
void __attribute__((weak)) Os_Callout_PostTask(void) { }
void __attribute__((weak)) Os_Callout_Idle(void) { }
void __attribute__((weak)) EcuM_Init(void) { }
void __attribute__((weak)) Irq_SOI(void) { }
void __attribute__((weak)) Irq_EOI(void) { }
