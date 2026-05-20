/* ARM Cortex-M3 System Tick implementation */
void Os_IncrementTick(void);
void os_sys_tick_handler(void) {
    Os_IncrementTick();
}
