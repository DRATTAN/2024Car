/*
 * alert.c
 *
 *  Created on: 2024Äê7ÔÂ29ÈÕ
 *      Author: RATTAN
 */

#include "app.h"

uint16_t counter = 0;

void ALERT_Proxy(void)
{
    static uint32_t tick = 0; //tick 10ms

    tick++;

    if((tick % 20) == 0)
    {
        if(counter != 0)
        {
            DL_GPIO_togglePins(GPIO_GRP_0_ALERT1_PORT, GPIO_GRP_0_ALERT1_PIN);
            DL_GPIO_togglePins(GPIO_GRP_0_ALERT2_PORT, GPIO_GRP_0_ALERT2_PIN);
            counter--;
        }
    }
}

void ALERT_SetAlert(uint16_t count)
{
    counter = count * 2;
}

