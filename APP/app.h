/*
 * app.h
 *
 *  Created on: 2024Äê7ÔÂ27ÈÕ
 *      Author: RATTAN
 */

#ifndef APP_APP_H_
#define APP_APP_H_

#include "ti_msp_dl_config.h"

#define PRINTF_UART_INST DEBUG_UART_INST

typedef struct _PID
{
    float kp;
    float ki;
    float kd;
    float err;
    float err_last;
    float err_sig;
    float lim_up;
    float lim_down;
    float lim_up_err_sig;
    float lim_down_err_sig;
} PID;

extern volatile int16_t ActMotorLeftPulse;
extern volatile int16_t ActMotorRightPulse;
extern volatile int32_t AbsMotorLeftPulse;
extern volatile int32_t AbsMotorRightPulse;

extern volatile float Roll;
extern volatile float Yaw;
extern volatile float Pitch;

extern volatile uint8_t Start;

extern int8_t GrayError;
extern uint8_t GrayRes;

extern PID yaw_pid;

void Delay_10ms(uint64_t x);
void Delay_ms(uint32_t x);

void APP_Init(void);

uint8_t PROCESS_Get(void);
void PROCESS_Set(uint8_t p);
void PROCESS_Main(void);
void PROCESS_1(void);
void PROCESS_2(void);
void PROCESS_3(void);
void PROCESS_4(void);

void UART_Printf(const char* fmt, ...);
void UART_Send(UART_Regs *reg, uint8_t *p, uint8_t length);

void PID_Init(PID *spid, float lim_up, float lim_down, float lim_up_err_sig, float lim_down_err_sig);
void PID_SetPid(PID *spid, float kp, float ki, float kd);
float PID_Cuc(PID *spid, float exp, float act);
void PID_Clear(PID *spid);

void ISR_Set(void(*p)(uint32_t));
void ISR_Disable(void);
void ISR_Enable(void);


void MOTOR_Init(void);
void MOTOR_SetExpSpeed(int16_t left, int16_t right);
void MOTOR_SetLeftDuty(int16_t duty);
void MOTOR_SetRightDuty(int16_t duty);
void MOTOR_SetLeftDir(uint8_t d);
void MOTOR_SetRightDir(uint8_t d);
void MOTOR_SetExpSpeedOffset(int16_t left, int16_t right);
void MOTOR_DisablePid(void);
void MOTOR_EnablePid(void);
void MOTOR_SetAbsEncoderZero(void);
int32_t MOTOR_GetAbsEncoderLine(void);
int32_t MOTOR_GetLeftAbsEncoder(void);
int32_t MOTOR_GetRightAbsEncoder(void);
void MOTOR_SoftSpeed(int32_t Start, int32_t Stop, float Sstart, float Sstop, int32_t Speed);

void GY62_Init(void);
void GY62_YawZero(void);
void YAW_Cycle(uint32_t tick);
float GY62_GetYaw(void);
void YAW_SetExpYaw(float exp);
void YAW_ClearPid(void);
void GY62_SoftYaw(int32_t Start, int32_t Stop, float Gstrat, float Gstop);

int8_t PHOTOCELL_GetError(void);
void PHOTOCELL_Init(void);
void PHOTOCELL_Cycle(uint32_t tick);
uint8_t PHOTOCELL_GetToggle(void);
void PHOTOCELL_IsStep(void);
void PHOTOCELL_ClearPid(void);
void PHOTOCELL_FlushToggle(void);
uint8_t PHOTOCELL_CheckWhite(uint8_t i);
uint8_t PHOTOCELL_CheckUnWhite(uint8_t i);

void ALERT_Proxy(void);
void ALERT_SetAlert(uint16_t count);

#endif /* APP_APP_H_ */
