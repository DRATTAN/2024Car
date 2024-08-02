/*
 * lib_I2C.c
 *
 *  Created on: 2024Äê7ÔÂ24ÈÕ
 *      Author: RATTAN
 */
#include"lib_i2c.h"
#include "arm_acle.h"

void LIB_I2CTransmitPoll(I2C_Regs *i2c, uint8_t DevAddr, uint8_t *buffer, uint16_t size)
{
    uint16_t counter = 0;
    while((DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS)) __nop();
    DL_I2C_flushControllerTXFIFO(i2c);
    while(!(DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_IDLE)) __nop();
    DL_I2C_setTargetAddress(i2c, DevAddr >> 1);
    DL_I2C_setControllerDirection(i2c, 0);
    DL_I2C_setTransactionLength(i2c, size);
    DL_I2C_transmitControllerData(i2c, *(buffer + counter));
    counter++;
    DL_I2C_enableStartCondition(i2c);
    DL_I2C_enableStopCondition(i2c);
    DL_I2C_enableControllerBurst(i2c);

    while(counter < size)
    {
        while(DL_I2C_isControllerTXFIFOFull(i2c)) __nop();
        DL_I2C_transmitControllerData(i2c, *(buffer + counter));
        counter++;
    }
}

void LIB_I2CTransmitMemPoll(I2C_Regs *i2c, uint8_t DevAddr, uint8_t MemAddr, uint8_t *buffer, uint16_t size)
{
    uint16_t counter = 0;
    while((DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS)) __nop();
    DL_I2C_flushControllerTXFIFO(i2c);
    while(!(DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_IDLE)) __nop();
    DL_I2C_setTargetAddress(i2c, DevAddr >> 1);
    DL_I2C_setControllerDirection(i2c, 0);
    DL_I2C_setTransactionLength(i2c, size + 1);
    DL_I2C_transmitControllerData(i2c, MemAddr);
    DL_I2C_enableStartCondition(i2c);
    DL_I2C_enableStopCondition(i2c);
    DL_I2C_enableControllerBurst(i2c);

    while(counter < size)
    {
        while(DL_I2C_isControllerTXFIFOFull(i2c)) __nop();
        DL_I2C_transmitControllerData(i2c, *(buffer + counter));
        counter++;
    }
}

void LIB_I2CRecivePoll(I2C_Regs *i2c, uint8_t DevAddr, uint8_t *buffer, uint16_t size)
{
    uint16_t counter = 0;
    while((DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS)) __nop();
    DL_I2C_flushControllerRXFIFO(i2c);
    while(!(DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_IDLE)) __nop();
    DL_I2C_flushControllerTXFIFO(i2c);
    while(!(DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_IDLE)) __nop();
    DL_I2C_startControllerTransfer(i2c, DevAddr >> 1, DL_I2C_CONTROLLER_DIRECTION_RX, size);
    //DL_I2C_enableControllerBurst(i2c);

    while(counter < size)
    {
        while (DL_I2C_isControllerRXFIFOEmpty(i2c));
        *buffer = DL_I2C_receiveControllerData(i2c);
        counter++;
    }
}
void LIB_I2CReciveMemPoll(I2C_Regs *i2c, uint8_t DevAddr, uint8_t MemAddr, uint8_t *buffer, uint16_t size)
{
    uint16_t counter = 0;
    while((DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS)) __nop();
    DL_I2C_flushControllerRXFIFO(i2c);
    while(!(DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_IDLE)) __nop();
    DL_I2C_flushControllerTXFIFO(i2c);
    while(!(DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_IDLE)) __nop();
    DL_I2C_transmitControllerData(i2c, MemAddr);
    DL_I2C_startControllerTransfer(i2c, DevAddr >> 1, DL_I2C_CONTROLLER_DIRECTION_TX, 1);
    while((DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_BUSY_BUS)) __nop();
    while(!(DL_I2C_getControllerStatus(i2c) & DL_I2C_CONTROLLER_STATUS_IDLE)) __nop();
    DL_I2C_startControllerTransfer(i2c, DevAddr >> 1, DL_I2C_CONTROLLER_DIRECTION_RX, size);
    //DL_I2C_enableControllerBurst(i2c);

    while(counter < size)
    {
        while (DL_I2C_isControllerRXFIFOEmpty(i2c));
        *buffer = DL_I2C_receiveControllerData(i2c);
        counter++;
    }


}
