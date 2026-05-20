/*-------------------------------- Arctic Core ------------------------------
 * OS Configuration for STM32F103 BCM
 */

#ifndef OS_CFG_H_
#define OS_CFG_H_

/* Task Count */
#define OS_TASK_CNT                     (1u)
#define OS_ISR_CNT                      (1u)
#define OS_COUNTER_CNT                  (0u)
#define OS_RESOURCE_CNT                 (0u)
#define OS_ALARM_CNT                    (0u)
#define OS_SCHEDTBL_CNT                 (0u)
#define OS_SCHTBL_CNT                   OS_SCHEDTBL_CNT
#define OS_APPLICATION_CNT              (1u)

/* Application Support - Enable type definitions */
#define OS_USE_APPLICATIONS             STD_ON

/* OS Configuration Parameters */
#define OS_MAX_ACTIVE_PRIORITIES        (16u)
#define OS_MAX_TASKS                    (8u)
#define OS_MAX_RESOURCES                (4u)
#define OS_MAX_ALARMS                   (4u)
#define OS_MAX_EVENTS                   (8u)
#define OS_MAX_COUNTERS                 (2u)
#define OS_MAX_SCHED_TABLES             (1u)
#define OS_MAX_APPLICATIONS             (1u)

/* Tick Configuration */
#define OS_TICK_FREQ_HZ                 (1000u)     /* 1ms tick */

/* Schedule Configuration */
#define OS_SCHEDULE_POLICY              OS_SCHEDULE_FULL_PREEMPTIVE

/* Features */
#define OS_USE_EVENTS                   STD_ON
#define OS_USE_ALARMS                   STD_OFF
#define OS_USE_RESOURCES                STD_OFF
#define OS_USE_SCHED_TABLES             STD_OFF
#define OS_USE_STACK_MONITORING         STD_OFF

/* Hook Routines */
#define OS_USE_STARTUP_HOOK             STD_OFF
#define OS_USE_SHUTDOWN_HOOK            STD_OFF
#define OS_USE_PRE_TASK_HOOK            STD_OFF
#define OS_USE_POST_TASK_HOOK           STD_OFF
#define OS_USE_IDLE_HOOK                STD_OFF

/* Error Handling */
#define OS_USE_PARAMETER_VALIDATION     STD_OFF
#define OS_USE_USER_ERROR_HOOK          STD_OFF

/* Idle Task */
#define OS_IDLE_TASK_STACK_SIZE         (128u)

/* Interrupt Stack */
#define OS_INTERRUPT_STACK_SIZE         (512u)

/* ISR Config */
#define OS_ISR_MAX_CNT                  OS_ISR_CNT
#define IRQ_INTERRUPT_OFFSET            (16u)
#define NUMBER_OF_INTERRUPTS_AND_EXCEPTIONS (240u)

/* Multi-Core */
#define OS_NUM_CORES                    (1u)

/* Timing Protection */
#define OS_USE_TIMING_PROTECTION        STD_OFF

/* SC3/SC4 compliance */
#define OS_SC3                          STD_OFF
#define OS_SC4                          STD_OFF

#endif /* OS_CFG_H_ */
