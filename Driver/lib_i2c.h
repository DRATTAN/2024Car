/*
 * lib_i2c.h
 *
 *  Created on: 2024Äê7ÔÂ24ÈÕ
 *      Author: RATTAN
 */

#ifndef DRIVER_LIB_I2C_H_
#define DRIVER_LIB_I2C_H_

#include "ti_msp_dl_config.h"

void LIB_I2CTransmitPoll(I2C_Regs *i2c, uint8_t DevAddr, uint8_t *buffer, uint16_t size);
void LIB_I2CTransmitMemPoll(I2C_Regs *i2c, uint8_t DevAddr, uint8_t MemAddr, uint8_t *buffer, uint16_t size);
void LIB_I2CRecivePoll(I2C_Regs *i2c, uint8_t DevAddr, uint8_t *buffer, uint16_t size);;
void LIB_I2CReciveMemPoll(I2C_Regs *i2c, uint8_t DevAddr, uint8_t MemAddr, uint8_t *buffer, uint16_t size);
#endif /* DRIVER_LIB_I2C_H_ */
