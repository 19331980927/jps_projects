/* ARM Cortex-M3 Architecture Init */
#include "Os.h"
#include "Cpu.h"

void arch_init(void) {
    /* Disable interrupts during init */
    __disable_irq();
    
    /* Configure SysTick for OS tick (1ms) */
    /* SysTick_Config is in CMSIS */
    extern uint32 SysTick_Config(uint32 ticks);
    SysTick_Config(72000000 / 1000);  /* 72MHz / 1000 = 1ms */
}

void arch_enable_interrupts(void) {
    __enable_irq();
}

void arch_disable_interrupts(void) {
    __disable_irq();
}
