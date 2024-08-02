/*
 * print.c
 *
 *  Created on: 2024Äê7ÔÂ27ÈÕ
 *      Author: RATTAN
 */

#include "app.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void UART_Send(UART_Regs *reg, uint8_t *p, uint8_t length)
{
    uint16_t i = 0;
    while(i < length)
    {
        DL_UART_Main_transmitDataBlocking(reg, p[i]);
        i++;
    }
}

void UART_Printf(const char* fmt, ...)
{
    va_list ap;
    uint8_t buffer[128] = {0};
    uint32_t i = 0;

    va_start(ap, fmt);
    vsprintf((char *)buffer, fmt, ap);
    va_end(ap);
    buffer[127] = 0;

    while(i < 128)
    {
        if(buffer[i] == 0x00) break;
        DL_UART_Main_transmitDataBlocking(DEBUG_UART_INST, buffer[i]);
        i++;
    }
}

