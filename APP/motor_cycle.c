
#include "app.h"
#include "arm_acle.h"

volatile int16_t ActMotorLeftPulse = 0;
volatile int16_t ActMotorRightPulse = 0;
volatile static uint64_t tick = 0;
volatile int32_t AbsMotorLeftPulse = 0;
volatile int32_t AbsMotorRightPulse = 0;

int16_t ExpMotorLeftPulse = 0;
int16_t ExpMotorRightPulse = 0;

int16_t ExpMotorLeftPulseOffset = 0;
int16_t ExpMotorRightPulseOffset = 0;

static uint8_t PointerUnsafe = 1;
static uint8_t CallBackCtrl = 0;
static void (* TickerISRCallbcak)(uint32_t tick) = 0x00000000;
static uint8_t motor_enablepid = 1;

static PID left_pid;
static PID right_pid;

void MOTOR_DisablePid()
{
    motor_enablepid = 0;
}

void MOTOR_EnablePid()
{
    motor_enablepid = 1;
}

void MOTOR_Init()
{
    MOTOR_SetExpSpeed(0, 0);
    PID_Init(&left_pid, 1000.0f, -1000.0f, 1500.0f, -1500.9f);
    PID_Init(&right_pid, 1000.0f, -1000.0f, 1500.0f, -1500.9f);
    PID_SetPid(&left_pid, 2.1f, 0.8, 0.10f);
    PID_SetPid(&right_pid, 2.1f, 0.8f, 0.10f);
    NVIC_EnableIRQ(TICKER_TIMER_INST_INT_IRQN);
}

inline void MOTOR_SetExpSpeed(int16_t left, int16_t right)
{
    ExpMotorLeftPulse = left;
    ExpMotorRightPulse = right;
}

inline void MOTOR_SetExpSpeedOffset(int16_t left, int16_t right)
{
    ExpMotorLeftPulseOffset = left;
    ExpMotorRightPulseOffset = right;
}

inline void MOTOR_SetLeftDir(uint8_t d)
{
    if(d == 1)
    {
        DL_GPIO_clearPins(GPIO_GRP_0_MOTOR_LEFT_INA_PORT, GPIO_GRP_0_MOTOR_LEFT_INA_PIN);
        DL_GPIO_setPins(GPIO_GRP_0_MOTOR_LEFT_INB_PORT, GPIO_GRP_0_MOTOR_LEFT_INB_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_MOTOR_LEFT_INB_PORT, GPIO_GRP_0_MOTOR_LEFT_INB_PIN);
        DL_GPIO_setPins(GPIO_GRP_0_MOTOR_LEFT_INA_PORT, GPIO_GRP_0_MOTOR_LEFT_INA_PIN);
    }
}

inline void MOTOR_SetRightDir(uint8_t d)
{
    if(d == 1)
    {
        DL_GPIO_clearPins(GPIO_GRP_0_MOTOR_RIGHT_INA_PORT, GPIO_GRP_0_MOTOR_RIGHT_INA_PIN);
        DL_GPIO_setPins(GPIO_GRP_0_MOTOR_RIGHT_INB_PORT, GPIO_GRP_0_MOTOR_RIGHT_INB_PIN);
    }
    else
    {
        DL_GPIO_clearPins(GPIO_GRP_0_MOTOR_RIGHT_INB_PORT, GPIO_GRP_0_MOTOR_RIGHT_INB_PIN);
        DL_GPIO_setPins(GPIO_GRP_0_MOTOR_RIGHT_INA_PORT, GPIO_GRP_0_MOTOR_RIGHT_INA_PIN);
    }
}

inline void MOTOR_SetLeftDuty(int16_t duty)
{
    if(duty >= 0)
    {
        MOTOR_SetLeftDir(1);
        DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, duty, DL_TIMER_CC_0_INDEX);
    }
    else
    {
        MOTOR_SetLeftDir(0);
        DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, -1 * duty, DL_TIMER_CC_0_INDEX);

    }
}

inline void MOTOR_SetRightDuty(int16_t duty)
{
    if(duty >= 0)
    {
        MOTOR_SetRightDir(1);
        DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, duty, DL_TIMER_CC_1_INDEX);
    }
    else
    {
        MOTOR_SetRightDir(0);
        DL_TimerA_setCaptureCompareValue(MOTOR_PWM_INST, -1 * duty, DL_TIMER_CC_1_INDEX);
    }
}

void Delay_10ms(uint64_t x)
{
    uint64_t start = tick;
    while((x+start) < tick) __nop();
}

void ISR_Set(void(*p)(uint32_t))
{
    PointerUnsafe = 1;
    TickerISRCallbcak = p;
    PointerUnsafe = 0;
}

void ISR_Disable(void)
{
    CallBackCtrl = 0;
}

void ISR_Enable(void)
{
    CallBackCtrl = 1;
}

void MOTOR_SetAbsEncoderZero()
{
    AbsMotorLeftPulse = 0;
    AbsMotorRightPulse = 0;
}

int32_t MOTOR_GetAbsEncoderLine()
{
    return (AbsMotorLeftPulse + AbsMotorRightPulse) / 2;
}

inline int32_t MOTOR_GetLeftAbsEncoder()
{
    return AbsMotorLeftPulse;
}

inline int32_t MOTOR_GetRightAbsEncoder()
{
    return AbsMotorRightPulse;
}

void MOTOR_SoftSpeed(int32_t Start, int32_t Stop, float Sstart, float Sstop, int32_t Speed)
{
    float step = (Sstop - Sstart) / (Stop - Start) * Speed;
    if((MOTOR_GetAbsEncoderLine() > Start) && (MOTOR_GetAbsEncoderLine() < Stop)) MOTOR_SetExpSpeed((Speed * Sstart + (step * (MOTOR_GetAbsEncoderLine() - Start))), (Speed * Sstart + (step * (MOTOR_GetAbsEncoderLine() - Start))));
}

void TICKER_TIMER_INST_IRQHandler(void) //100hz @ above 3100 pulse
{
    volatile static uint8_t LR = 0;
    if(DL_TimerG_getPendingInterrupt(TICKER_TIMER_INST) == GPTIMER_CPU_INT_IIDX_STAT_Z)
    {
        tick++;
        if(0 == LR)
        {
            LR = 1;
            ActMotorLeftPulse = (int16_t)DL_TimerG_getTimerCount(MUX_QEI_INST);
            if((ActMotorLeftPulse < 2) && (ActMotorLeftPulse > -2)) ActMotorLeftPulse = 0;
            AbsMotorLeftPulse += ActMotorLeftPulse;
            IOMUX->SECCFG.PINCM[IOMUX_PINCM23] = 0;
            IOMUX->SECCFG.PINCM[IOMUX_PINCM24] = 0;
            IOMUX->SECCFG.PINCM[IOMUX_PINCM32] = IOMUX_PINCM32_PF_TIMG8_CCP0 | IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE;
            IOMUX->SECCFG.PINCM[IOMUX_PINCM33] = IOMUX_PINCM33_PF_TIMG8_CCP1 | IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE;
            if(motor_enablepid) MOTOR_SetLeftDuty(PID_Cuc(&left_pid, ExpMotorLeftPulse + ExpMotorLeftPulseOffset, ActMotorLeftPulse));
            DL_TimerG_setTimerCount(MUX_QEI_INST, 0);
        }
        else if(1 == LR)
        {
            LR = 0;
            ActMotorRightPulse = (int16_t)DL_TimerG_getTimerCount(MUX_QEI_INST);
            if((ActMotorRightPulse < 2) && (ActMotorRightPulse > -2)) ActMotorRightPulse = 0;
            AbsMotorRightPulse += ActMotorRightPulse;
            IOMUX->SECCFG.PINCM[IOMUX_PINCM32] = 0;
            IOMUX->SECCFG.PINCM[IOMUX_PINCM33] = 0;
            IOMUX->SECCFG.PINCM[IOMUX_PINCM23] = IOMUX_PINCM32_PF_TIMG8_CCP0 | IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE;
            IOMUX->SECCFG.PINCM[IOMUX_PINCM24] = IOMUX_PINCM33_PF_TIMG8_CCP1 | IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM_INENA_ENABLE;
            if(motor_enablepid) MOTOR_SetRightDuty(PID_Cuc(&right_pid, ExpMotorRightPulse + ExpMotorRightPulseOffset, ActMotorRightPulse));
            DL_TimerG_setTimerCount(MUX_QEI_INST, 0);
        }
        if((0 == PointerUnsafe) && CallBackCtrl) (*TickerISRCallbcak)(tick);
        ALERT_Proxy();
        //UART_Printf("speed:%d,%d,%d,%d\n", ActMotorLeftSpeed, ExpMotorLeftPulse, ActMotorRightSpeed, ExpMotorRightPulse);
    }
}
