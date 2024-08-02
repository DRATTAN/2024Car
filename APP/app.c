/*
 * app.c
 *
 *  Created on: 2024Äê7ÔÂ27ÈÕ
 *      Author: RATTAN
 */

#ifndef APP_APP_C_
#define APP_APP_C_

#include "app.h"

void APP_Init(void)
{
    MOTOR_Init();
    GY62_Init();
    PHOTOCELL_Init();
}

void Delay_ms(uint32_t x)
{
    delay_cycles(x*80000);
}

void Calibration(void)
{

}

#endif /* APP_APP_C_ */
