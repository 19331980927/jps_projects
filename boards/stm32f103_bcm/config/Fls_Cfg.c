/*-------------------------------- Arctic Core ------------------------------
 * Copyright (C) 2013, ArcCore AB, Sweden, www.arccore.com.
 * Contact: <contact@arccore.com>
 *
 * You may ONLY use this file:
 * 1)if you have a valid commercial ArcCore license and then in accordance with
 * the terms contained in the written license agreement between you and ArcCore,
 * or alternatively
 * 2)if you follow the terms found in GNU General Public License version 2 as
 * published by the Free Software Foundation and appearing in the file
 * LICENSE.GPL included in the packaging of this file or here
 * <http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt>
 *-------------------------------- Arctic Core -----------------------------*/


#include "Fls.h"
#include <stdlib.h>

#if defined(CFG_STM32F1X)

#if defined(STM32F10X_HD)
/*
 * High Density (512KB Flash, 2KB pages).
 * Use the last 16 pages (32KB) for NVM emulation.
 * Pages: 0x08078000 .. 0x08080000 (top 32KB of 512KB)
 */
const FlashType flashInfo[] = {
    {
        .size = FLS_TOTAL_SIZE,
        .sectCnt = FLASH_MAX_PAGES,
        .sectAddr = { 0x08078000,
        /*.sectAddr[1] = */ 0x08078800,
        /*.sectAddr[2] = */ 0x08079000,
        /*.sectAddr[3] = */ 0x08079800,
        /*.sectAddr[4] = */ 0x0807A000,
        /*.sectAddr[5] = */ 0x0807A800,
        /*.sectAddr[6] = */ 0x0807B000,
        /*.sectAddr[7] = */ 0x0807B800,
        /*.sectAddr[8] = */ 0x0807C000,
        /*.sectAddr[9] = */ 0x0807C800,
        /*.sectAddr[10] = */ 0x0807D000,
        /*.sectAddr[11] = */ 0x0807D800,
        /*.sectAddr[12] = */ 0x0807E000,
        /*.sectAddr[13] = */ 0x0807E800,
        /*.sectAddr[14] = */ 0x0807F000,
        /*.sectAddr[15] = */ 0x0807F800,
        /*.sectAddr[16] = */ 0x08080000, }
    }
};

#elif defined(STM32F10X_MD)
/*
 * Medium Density (128KB Flash, 1KB pages).
 * Use the last 16 pages (16KB) for NVM emulation.
 * Pages: 0x0801C000 .. 0x08020000 (top 16KB of 128KB)
 */
const FlashType flashInfo[] = {
    {
        .size = FLS_TOTAL_SIZE,
        .sectCnt = FLASH_MAX_PAGES,
        .sectAddr = { 0x0801C000,
        /*.sectAddr[1] = */ 0x0801C400,
        /*.sectAddr[2] = */ 0x0801C800,
        /*.sectAddr[3] = */ 0x0801CC00,
        /*.sectAddr[4] = */ 0x0801D000,
        /*.sectAddr[5] = */ 0x0801D400,
        /*.sectAddr[6] = */ 0x0801D800,
        /*.sectAddr[7] = */ 0x0801DC00,
        /*.sectAddr[8] = */ 0x0801E000,
        /*.sectAddr[9] = */ 0x0801E400,
        /*.sectAddr[10] = */ 0x0801E800,
        /*.sectAddr[11] = */ 0x0801EC00,
        /*.sectAddr[12] = */ 0x0801F000,
        /*.sectAddr[13] = */ 0x0801F400,
        /*.sectAddr[14] = */ 0x0801F800,
        /*.sectAddr[15] = */ 0x0801FC00,
        /*.sectAddr[16] = */ 0x08020000, }
    }
};

#else
#error "STM32F10X density not defined"
#endif

#else
#error CPU NOT supported
#endif


const Fls_ConfigType FlsConfigSet[]=
{
  {
     .FlsJobEndNotification = NULL,
     .FlsJobErrorNotification = NULL,
     .FlsInfo = flashInfo
  }
};
