/*
 * yaw_cycle.c
 *
 *  Created on: 2024Äê7ÔÂ29ÈÕ
 *      Author: RATTAN
 */

#include "app.h"

static float ExpYaw = 0.0f;
static float ActYaw = 0.0f;
PID yaw_pid;

void YAW_Cycle(uint32_t tick)
{
    int16_t res = 0.0f;
    if((tick % 3) == 0)
    {
        ActYaw = GY62_GetYaw();
        res = (int16_t)PID_Cuc(&yaw_pid, ExpYaw, ActYaw);
        MOTOR_SetExpSpeedOffset(res,-1 *  res) ;

    }
}

void YAW_SetExpYaw(float exp)
{
    ExpYaw = exp;
}

void YAW_ClearPid(void)
{
    PID_Clear(&yaw_pid);
}
