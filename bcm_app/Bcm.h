/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2024, BCM Project.
 *
 * Bcm.h
 * BCM Application - Main Interface Header
 */

#ifndef BCM_H_
#define BCM_H_

#include "Std_Types.h"

/* BCM 主初始化 - 在 EcuM 初始化完成后调用 */
void Bcm_Init(void);

/* BCM 主函数 - 由 OS 任务周期性调度（建议 10ms 周期） */
void Bcm_MainFunction(void);

#endif /* BCM_H_ */
