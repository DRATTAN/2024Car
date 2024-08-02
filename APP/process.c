/*
 * process.c
 *
 *  Created on: 2024Äê7ÔÂ29ÈÕ
 *      Author: RATTAN
 */

#include "app.h"
#include "arm_acle.h"
#include "math.h"

static volatile uint8_t process = 1;
volatile uint8_t Start = 0;
uint16_t SetSpeed = 1500;
uint16_t Step = 200;

void PROCESS_SetIndex(uint8_t i)
{
    process = i;
}

void PROCESS_1(void)
{
    MOTOR_EnablePid();
    Delay_ms(2000);
    MOTOR_SetExpSpeed(SetSpeed*0.1f, SetSpeed*0.1f);
    MOTOR_SetAbsEncoderZero();
    while(1)
    {
        Delay_ms(10);
        MOTOR_SoftSpeed(0, 50000, 0.1f, 0.5f, SetSpeed);
        PHOTOCELL_GetError();
        if(PHOTOCELL_CheckUnWhite(3) && (MOTOR_GetAbsEncoderLine() > 100000)) break;
    }

    ISR_Disable();
    MOTOR_SetExpSpeedOffset(0, 0);
    MOTOR_SetExpSpeed(0,0);
    process = 0;
    ALERT_SetAlert(1);
    Delay_ms(3000);

}

void PROCESS_2(void)
{
    MOTOR_EnablePid();
    Delay_ms(2000);
    GY62_YawZero();
    YAW_SetExpYaw(-45.0f);
    YAW_ClearPid();
    ISR_Set(YAW_Cycle);
    ISR_Enable();
    Delay_ms(800);
    YAW_SetExpYaw(-90.0f);
    Delay_ms(800);
    MOTOR_SetAbsEncoderZero();
    MOTOR_SetExpSpeed(SetSpeed * 0.1f, SetSpeed * 0.1f);
    while(1)
    {
        Delay_ms(10);
        MOTOR_SoftSpeed(0, 40000, 0.1f, 1.0f, SetSpeed);
        PHOTOCELL_GetError();
        if(PHOTOCELL_CheckUnWhite(3) && (MOTOR_GetAbsEncoderLine() > 100000)) break;
    }

    ALERT_SetAlert(1);
    PHOTOCELL_ClearPid();
    ISR_Set(PHOTOCELL_Cycle);
    ISR_Enable();
    MOTOR_SetAbsEncoderZero();
    while(1)
    {
        Delay_ms(10);
        if(PHOTOCELL_CheckWhite(3) && (MOTOR_GetAbsEncoderLine() > 100000)) break;
    }

    ALERT_SetAlert(1);
    YAW_SetExpYaw(89);
    YAW_ClearPid();
    ISR_Set(YAW_Cycle);
    MOTOR_SetAbsEncoderZero();
    while(1)
    {
        Delay_ms(10);
        PHOTOCELL_GetError();
        if(PHOTOCELL_CheckUnWhite(3) && (MOTOR_GetAbsEncoderLine() > 100000)) break;

    }

    ALERT_SetAlert(1);
    PHOTOCELL_ClearPid();
    ISR_Set(PHOTOCELL_Cycle);
    ISR_Enable();
    MOTOR_SetAbsEncoderZero();
    while(1)
    {
        Delay_ms(10);
        if(PHOTOCELL_CheckWhite(3) && (MOTOR_GetAbsEncoderLine() > 100000)) break;
    }

    ISR_Disable();
    MOTOR_SetExpSpeedOffset(0, 0);
    MOTOR_SetExpSpeed(0,0);
    process = 0;
    ALERT_SetAlert(1);
    Delay_ms(3000);
}

void PROCESS_3(void)
{
    MOTOR_EnablePid();
    const float Yaw1 = -56.0f;
    const float Yaw2 = 55.6f;
    const float Yaw_fix = 20.0f;
    const int32_t Max_c = 149000;

    Delay_ms(2000);
    uint8_t cycle = 0;
    PHOTOCELL_FlushToggle();
    GY62_YawZero();
    MOTOR_SetExpSpeed(0, 0);
    YAW_SetExpYaw(-51.0f);
    YAW_ClearPid();
    ISR_Set(YAW_Cycle);
    ISR_Enable();
    Delay_ms(600);

Entrance:
    if(cycle == 0) YAW_SetExpYaw(-51.0f);
    else YAW_SetExpYaw(Yaw1);
    YAW_ClearPid();
    ISR_Set(YAW_Cycle);
    MOTOR_SetAbsEncoderZero();
    while(1)
    {
        Delay_ms(10);
        if(cycle == 0) MOTOR_SoftSpeed(0, 10000, 0.3f, 1.6f, SetSpeed);
        else MOTOR_SoftSpeed(30000, 35000, 1.6f, 2.2f, SetSpeed);
        MOTOR_SoftSpeed(130000, 135000, 1.6f, 0.6f, SetSpeed);
        PHOTOCELL_GetError();
        if(MOTOR_GetAbsEncoderLine() > Max_c) YAW_SetExpYaw(Yaw1 - Yaw_fix);
        if(PHOTOCELL_CheckUnWhite(3) && (MOTOR_GetAbsEncoderLine() > 150000)) break;
    }

    MOTOR_SetAbsEncoderZero();
    ALERT_SetAlert(1);
    PHOTOCELL_ClearPid();
    ISR_Set(PHOTOCELL_Cycle);
    while(1)
    {
        Delay_ms(10);
        MOTOR_SoftSpeed(0, 30000, 0.6f, 1.6f, SetSpeed);
        if(PHOTOCELL_CheckWhite(3) && (MOTOR_GetAbsEncoderLine() > 100000)) break;

    }

    MOTOR_SetAbsEncoderZero();
    ALERT_SetAlert(1);
    YAW_SetExpYaw(Yaw2);
    YAW_ClearPid();
    ISR_Set(YAW_Cycle);
    while(1)
    {
        Delay_ms(10);
        MOTOR_SoftSpeed(30000, 35000, 1.6f, 2.2f, SetSpeed);
        MOTOR_SoftSpeed(130000, 135000, 1.6f, 0.6f, SetSpeed);
        PHOTOCELL_GetError();
        if(MOTOR_GetAbsEncoderLine() > Max_c) YAW_SetExpYaw(Yaw2 + Yaw_fix);
        if(PHOTOCELL_CheckUnWhite(3) && (MOTOR_GetAbsEncoderLine() > 150000)) break;

    }

    MOTOR_SetAbsEncoderZero();
    ALERT_SetAlert(1);
    PHOTOCELL_ClearPid();
    ISR_Set(PHOTOCELL_Cycle);
    while(1)
    {
        Delay_ms(10);
        MOTOR_SoftSpeed(0, 30000, 0.6f, 1.6f, SetSpeed);
        if(PHOTOCELL_CheckWhite(3) && (MOTOR_GetAbsEncoderLine() > 100000)) break;
    }

    ISR_Disable();
    MOTOR_SetExpSpeedOffset(0, 0);
    MOTOR_SetExpSpeed(0,0);
    process = 0;
    ALERT_SetAlert(1);
    Delay_ms(3000);
}

void PROCESS_4(void)
{
    MOTOR_EnablePid();

    const float Yaw1 = -55.5f;
    const float Yaw2 = 54.8f;
    const float Yaw_fix = 30.0f;
    const int32_t Max_c = 149000;

    Delay_ms(2000);
    uint8_t cycle = 0;
    PHOTOCELL_FlushToggle();
    GY62_YawZero();
    MOTOR_SetExpSpeed(0, 0);
    YAW_SetExpYaw(-51.0f);
    YAW_ClearPid();
    ISR_Set(YAW_Cycle);
    ISR_Enable();
    Delay_ms(600);

Entrance:
    if(cycle == 0) YAW_SetExpYaw(-51.0f);
    else YAW_SetExpYaw(Yaw1);
    YAW_ClearPid();
    ISR_Set(YAW_Cycle);
    MOTOR_SetAbsEncoderZero();
    while(1)
    {
        Delay_ms(10);
        if(cycle == 0) MOTOR_SoftSpeed(0, 10000, 0.3f, 1.6f, SetSpeed);
        else MOTOR_SoftSpeed(30000, 35000, 1.6f, 2.2f, SetSpeed);
        MOTOR_SoftSpeed(130000, 135000, 1.6f, 0.6f, SetSpeed);
        PHOTOCELL_GetError();
        if(MOTOR_GetAbsEncoderLine() > Max_c) YAW_SetExpYaw(Yaw1 - Yaw_fix);
        if(PHOTOCELL_CheckUnWhite(3) && (MOTOR_GetAbsEncoderLine() > 150000)) break;
    }

    MOTOR_SetAbsEncoderZero();
    ALERT_SetAlert(1);
    PHOTOCELL_ClearPid();
    ISR_Set(PHOTOCELL_Cycle);
    while(1)
    {
        Delay_ms(10);
        MOTOR_SoftSpeed(0, 30000, 0.6f, 1.6f, SetSpeed);
        if(PHOTOCELL_CheckWhite(3) && (MOTOR_GetAbsEncoderLine() > 100000)) break;

    }

    MOTOR_SetAbsEncoderZero();
    ALERT_SetAlert(1);
    YAW_SetExpYaw(Yaw2);
    YAW_ClearPid();
    ISR_Set(YAW_Cycle);
    while(1)
    {
        Delay_ms(10);
        MOTOR_SoftSpeed(30000, 35000, 1.6f, 2.2f, SetSpeed);
        MOTOR_SoftSpeed(130000, 135000, 1.6f, 0.6f, SetSpeed);
        PHOTOCELL_GetError();
        if(MOTOR_GetAbsEncoderLine() > Max_c) YAW_SetExpYaw(Yaw2 + Yaw_fix);
        if(PHOTOCELL_CheckUnWhite(3) && (MOTOR_GetAbsEncoderLine() > 150000)) break;

    }

    MOTOR_SetAbsEncoderZero();
    ALERT_SetAlert(1);
    PHOTOCELL_ClearPid();
    ISR_Set(PHOTOCELL_Cycle);
    while(1)
    {
        Delay_ms(10);
        MOTOR_SoftSpeed(0, 30000, 0.6f, 1.6f, SetSpeed);
        if(PHOTOCELL_CheckWhite(3) && (MOTOR_GetAbsEncoderLine() > 100000)) break;
    }

    cycle++;
    if(cycle < 4)
    {
        ALERT_SetAlert(1);
        goto Entrance;
    }

    ISR_Disable();
    MOTOR_SetExpSpeedOffset(0, 0);
    MOTOR_SetExpSpeed(0,0);
    process = 0;
    ALERT_SetAlert(1);
    Delay_ms(3000);
}

uint8_t PROCESS_Get()
{
    return process;
}

void PROCESS_Set(uint8_t p)
{
    process = p;
}

void PROCESS_Main()
{
    if(Start == 1)
    {
        if(process == 1) PROCESS_1();
        else if(process == 2) PROCESS_2();
        else if(process == 3) PROCESS_3();
        else if(process == 4) PROCESS_4();
        Start = 0;

    }
    else return;
}

//void GROUP0_IRQHandler(void)
//{
//    if(DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_0) == GPIO_GRP_0_INT_IIDX)
//    {
//        while(DL_GPIO_readPins(GPIO_GRP_0_PORT, GPIO_GRP_0_SET_PROCESS_KEY_PIN) == 0) __nop();
//        process++;
//        if(process > 4) process = 1;
//    }
//}
