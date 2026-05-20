@ ARM Cortex-M3 OS Kernel Assembly
.cpu cortex-m3
.thumb
.syntax unified

.thumb_func
.global Os_ContextSwitch
Os_ContextSwitch:
    bx lr

.thumb_func
.global Os_StartFirstTask
Os_StartFirstTask:
    bx lr

.thumb_func
.global Os_Tick
Os_Tick:
    bx lr

.thumb_func
.global Os_Schedule
Os_Schedule:
    bx lr

.end
