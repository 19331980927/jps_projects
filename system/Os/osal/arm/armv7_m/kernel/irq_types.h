/*-------------------------------- Arctic Core ------------------------------
 * IRQ Types for ARM Cortex-M3
 */

#ifndef IRQ_TYPES_H_
#define IRQ_TYPES_H_

#include "Std_Types.h"

/* Interrupt/IRQ number type */
typedef uint32 IrqType;

/* CPU/Core ID type */
typedef uint32 Cpu_t;

/* Stack and frame types */
typedef struct {
    uint32 r0;
    uint32 r1;
    uint32 r2;
    uint32 r3;
    uint32 r12;
    uint32 lr;
    uint32 pc;
    uint32 psr;
} Os_ExceptionFrameType;

/* IRQ handler function pointer */
typedef void (*Os_IrqHandlerType)(void);

#endif /* IRQ_TYPES_H_ */
