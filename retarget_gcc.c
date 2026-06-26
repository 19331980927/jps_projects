/* retarget_gcc.c — redirect printf output to USART1 for GCC (newlib-nano)
 * ARMCC/MicroLIB uses fputc() (already in usart.c).
 * GCC/newlib-nano uses _write() — this file provides it.
 * Not added to the Keil uVision5 project, so Keil is unaffected.
 */
#include <stdint.h>
#include <unistd.h>
#include "usart.h"

int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
        USART1_SendByte((uint8_t)ptr[i]);
    return len;
}
