/*
 * photocell.h
 *
 *  Created on: 2024Äê7ÔÂ29ÈÕ
 *      Author: RATTAN
 */

#include "app.h"
#include "photocell.h"

int16_t ExpGry = 0;
int16_t ActGry = 0;
int8_t GrayError = 0;
uint8_t GrayRes = 0;
uint8_t GrayToggle = 0;
PID gray_pid = {0};

int8_t last_state[10] = {0,0,0,0,0,0,0,0,0,0};

void PHOTOCELL_Init(void)
{
    PID_Init(&gray_pid, 3100.0f, -3100.0f, 4000.0f, -4000.0f);
    PID_SetPid(&gray_pid, 160.0f, 3.0f, 4.8f);
}

int8_t PHOTOCELL_GetError(void)
{
    GrayRes = Photocell_ReadDigitalValue();
    int8_t res = (GrayRes >> 4) - (GrayRes&0x0f);
    GrayError = res;
    last_state[9] = last_state[8];
    last_state[8] = last_state[7];
    last_state[7] = last_state[6];
    last_state[6] = last_state[5];
    last_state[5] = last_state[4];
    last_state[4] = last_state[3];
    last_state[3] = last_state[2];
    last_state[2] = last_state[1];
    last_state[1] = last_state[0];
    last_state[0] = GrayRes;
    PHOTOCELL_IsStep();
    return res;
}


void PHOTOCELL_Cycle(uint32_t tick)
{
    int16_t res = 0.0f;
    if((tick % 2) == 0)
    {
        ActGry = PHOTOCELL_GetError();
        res = (int16_t)PID_Cuc(&gray_pid, ExpGry, ActGry);
        MOTOR_SetExpSpeedOffset(-1 * res, res);
    }
}

inline uint8_t PHOTOCELL_GetToggle()
{
    return GrayToggle;
}

void PHOTOCELL_IsStep(void)
{
    if((last_state[0] != last_state[1])) GrayToggle = !GrayToggle;
}

uint8_t PHOTOCELL_CheckWhite(uint8_t i)
{
    uint8_t counter = 0;
    for(uint8_t j = 0; j < i; j++)
    {
        if(last_state[j] == 0) counter++;
    }
    if(counter == i) return 1;
    else return 0;
}

uint8_t PHOTOCELL_CheckUnWhite(uint8_t i)
{
    uint8_t counter = 0;
    for(uint8_t j = 0; j < i; j++)
    {
        if(last_state[j] != 0) counter++;
    }
    if(counter == i) return 1;
    else return 0;
}

void PHOTOCELL_FlushToggle(void)
{
    PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();
    PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();
    PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();
    PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();
    PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();
    PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();PHOTOCELL_GetError();
}

void PHOTOCELL_ClearPid(void)
{
    PID_Clear(&gray_pid);
}
