/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "ti_msp_dl_config.h"
#include "app.h"
#include "oled.h"
#include "photocell.h"

void Key_Get(void);

int main(void)
{
    SYSCFG_DL_init();
    NVIC_ClearPendingIRQ(DEBUG_UART_INST_INT_IRQN);
    NVIC_EnableIRQ(DEBUG_UART_INST_INT_IRQN);
    Delay_ms(100);
    APP_Init();
    Delay_ms(500);
    Oled_Init();
    PROCESS_Set(1);


    MOTOR_DisablePid();
    //MOTOR_SetExpSpeed(100, 100);
    while (1)               //check process
    {
        Key_Get();
        PHOTOCELL_GetError();
        Oled_Printf(0, 0, "Yaw: %-5.1f", GY62_GetYaw());
        Oled_Printf(0, 1, "Gray: %-3d", GrayRes);
        Oled_Printf(0, 2, "Process: %d", PROCESS_Get());
        Oled_Printf(0, 3, "Left: %-8d", MOTOR_GetLeftAbsEncoder());
        Oled_Printf(0, 4, "Right: %-8d", MOTOR_GetRightAbsEncoder());
        PROCESS_Main();
        delay_cycles(1600000);
    }
}

void DEBUG_UART_INST_IRQHandler(void)
{
#define GETFLOAT() (*(float*)(buffer + 1))
#define GET
    static uint8_t buffer[6];
    static uint8_t counter = 0;
    if(DL_UART_Main_getPendingInterrupt(DEBUG_UART_INST) == DL_UART_MAIN_IIDX_RX)
    {
        buffer[counter] = DL_UART_Main_receiveData(DEBUG_UART_INST);
        counter++;
        if(counter == 6)
        {
            Oled_Printf(0, 4, "%-2X %-2X %-2X %-2X %-2X %-2X", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);
            counter = 0;
        }
    }
}

void Key_Get()
{
    static uint8_t i = 1;
    if(DL_GPIO_readPins(GPIO_GRP_0_KEY1_PORT, GPIO_GRP_0_KEY1_PIN) == GPIO_GRP_0_KEY1_PIN)
    {
        Delay_ms(100);
        while(DL_GPIO_readPins(GPIO_GRP_0_KEY1_PORT, GPIO_GRP_0_KEY1_PIN) == GPIO_GRP_0_KEY1_PIN);
        i++;
        if(i > 4) i = 1;
        PROCESS_Set(i);
    }
    if(DL_GPIO_readPins(GPIO_GRP_0_KEY2_PORT, GPIO_GRP_0_KEY2_PIN) != GPIO_GRP_0_KEY2_PIN)
    {
        Delay_ms(100);
        while(DL_GPIO_readPins(GPIO_GRP_0_KEY2_PORT, GPIO_GRP_0_KEY2_PIN) != GPIO_GRP_0_KEY2_PIN);
        Start = 1;

    }
}
