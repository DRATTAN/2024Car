/*
 * pid.c
 *
 *  Created on: 2024Äê7ÔÂ27ÈÕ
 *      Author: RATTAN
 */

#include "app.h"

void PID_Init(PID *spid, float lim_up, float lim_down, float lim_up_err_sig, float lim_down_err_sig)
{
    spid->err = 0.0f;
    spid->err_last = 0.0f;
    spid->err_sig = 0.0f;
    spid->lim_up = lim_up;
    spid->lim_down = lim_down;
    spid->lim_up_err_sig = lim_up_err_sig;
    spid->lim_down_err_sig = lim_down_err_sig;
}

void PID_SetPid(PID *spid, float kp, float ki, float kd)
{
    spid->kp = kp;
    spid->ki = ki;
    spid->kd = kd;
}

float PID_Cuc(PID *spid, float exp, float act)
{
    float res = 0.0f;
    spid->err = exp - act;
    res = (spid->err) * (spid->kp) + ((spid->err)-(spid->err_last)) * (spid->kd) + (spid->err_sig) * (spid->ki);

    if(res > (spid->lim_up)) res = (spid->lim_up);
    if(res < (spid->lim_down)) res = (spid->lim_down);

    spid->err_last = spid->err;
    spid->err_sig += spid->err;
    if((spid->err_sig) > (spid->lim_up_err_sig)) (spid->err_sig) = (spid->lim_up_err_sig);
    if((spid->err_sig) < (spid->lim_down_err_sig)) (spid->err_sig) = (spid->lim_down_err_sig);
    return res;
}

void PID_Clear(PID *spid)
{
    spid->err = 0.0f;
    spid->err_last = 0.0f;
    spid->err_sig = 0.0f;
}
