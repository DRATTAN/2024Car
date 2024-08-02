/*
 * gy62.c
 *
 *  Created on: 2024Äê7ÔÂ28ÈÕ
 *      Author: RATTAN
 */

#include "app.h"

volatile float Roll = 0.0f;
volatile float Pitch = 0.0f;
volatile float Yaw = 0.0f;
uint8_t recive_buffer[11] = {0};
void GY62_Init()
{
    //NVIC_ClearPendingIRQ(GY62_UART_INST_INT_IRQN);
    NVIC_EnableIRQ(GY62_UART_INST_INT_IRQN);
    GY62_YawZero();
    PID_Init(&yaw_pid, 1700.0f, -1700.0f, 1000.0f, -1000.0f);
    PID_SetPid(&yaw_pid, 19.0f, 0.9f, 1.5f);
}

void GY62_YawZero()
{
    uint8_t cmd[] = {0xFF, 0xAA, 0x52};
    UART_Send(GY62_UART_INST, cmd, 3);
}

inline float GY62_GetYaw(void)
{
    return Yaw;
}

void GY62_SoftYaw(int32_t Start, int32_t Stop, float Gstrat, float Gstop)
{
    float step = (Gstop - Gstrat) / (Stop - Start);
    if((MOTOR_GetAbsEncoderLine() > Start) && (MOTOR_GetAbsEncoderLine() < Stop)) YAW_SetExpYaw((Gstrat + (step * (MOTOR_GetAbsEncoderLine() - Start))));
}

void GY62_UART_INST_IRQHandler(void)
{

    uint8_t sum = 0;
    static uint16_t counter = 0;

    if(DL_UART_Main_getPendingInterrupt(GY62_UART_INST) == DL_UART_MAIN_IIDX_RX)
    {
        recive_buffer[counter] = DL_UART_Main_receiveData(GY62_UART_INST);
        if(recive_buffer[0] == 0x55) counter++;
        else counter = 0;
        if(counter == 11)
        {
            if(recive_buffer[1] == 0x53) // Euler Angles information
            {
                sum = (recive_buffer[0]+recive_buffer[1]+recive_buffer[2]+recive_buffer[3]+recive_buffer[4]+recive_buffer[5]+recive_buffer[6]+recive_buffer[7]+recive_buffer[8]+recive_buffer[9]);
                if(recive_buffer[10] == sum)
                {
                    Roll = ((float)(*(int16_t *)(recive_buffer + 2))) / 32768.0f * 180.0f;
                    Pitch = ((float)(*(int16_t *)(recive_buffer + 4))) / 32768.0f * 180.0f;
                    Yaw = ((float)(*(int16_t *)(recive_buffer + 6))) / 32768.0f * 180.0f;
                }
            }
            counter = 0;
            recive_buffer[0] = 0x00;
        }
    }
}
