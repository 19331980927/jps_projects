/*-------------------------------- Arctic Core ------------------------------
 * arch_offset.c
 * Generate architecture-specific offsets for ARM Cortex-M3.
 */

#include "os_i.h"

#define DECLARE(sym,val) \
    __asm("#define\t" #sym "\t%0" : : "n" ((val)))

void arch_offset_dummy(void) {
    /* Exception frame offsets */
    DECLARE(EXC_FRM_R0,  0);
    DECLARE(EXC_FRM_R1,  4);
    DECLARE(EXC_FRM_R2,  8);
    DECLARE(EXC_FRM_R3,  12);
    DECLARE(EXC_FRM_R12, 16);
    DECLARE(EXC_FRM_LR,  20);
    DECLARE(EXC_FRM_PC,  24);
    DECLARE(EXC_FRM_PSR, 28);

    /* Stack frame size */
    DECLARE(STACK_APA, 32);

    /* Task offsets */
    DECLARE(STACK_CURR,        offsetof(OsTaskVarType, stack.curr));
    DECLARE(STACK_CURR_TOP,    offsetof(OsTaskVarType, stack.top));
    DECLARE(STACK_CURR_SIZE,   offsetof(OsTaskVarType, stack.size));
    DECLARE(TASK_CONST,        offsetof(OsTaskVarType, constPtr));
}
