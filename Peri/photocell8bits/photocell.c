/*
 * photocell.c
 *
 *  Created on: 2023/07/11
 *      Author: RATTAN
 */

#include "photocell.h"


uint8_t Photocell_ReadDigitalValue()
{
    uint8_t res = 0;

    res = res << 1;
    res |= (DL_GPIO_readPins(GPIO_GRP_0_R4_PORT, GPIO_GRP_0_R4_PIN) == 0)?0:1;

    res = res << 1;
    res |= (DL_GPIO_readPins(GPIO_GRP_0_R3_PORT, GPIO_GRP_0_R3_PIN) == 0)?0:1;

    res = res << 1;
    res |= (DL_GPIO_readPins(GPIO_GRP_0_R2_PORT, GPIO_GRP_0_R2_PIN) == 0)?0:1;

    res = res << 1;
    res |= (DL_GPIO_readPins(GPIO_GRP_0_R1_PORT, GPIO_GRP_0_R1_PIN) == 0)?0:1;

    res = res << 1;
    res |= (DL_GPIO_readPins(GPIO_GRP_0_L4_PORT, GPIO_GRP_0_L4_PIN) == 0)?0:1;

    res = res << 1;
    res |= (DL_GPIO_readPins(GPIO_GRP_0_L3_PORT, GPIO_GRP_0_L3_PIN) == 0)?0:1;

    res = res << 1;
    res |= (DL_GPIO_readPins(GPIO_GRP_0_L2_PORT, GPIO_GRP_0_L2_PIN) == 0)?0:1;

    res = res << 1;
    res |= (DL_GPIO_readPins(GPIO_GRP_0_L1_PORT, GPIO_GRP_0_L1_PIN) == 0)?0:1;

    return res;
}

void Photocell_ReadAnalogValue(uint8_t * BufferPoint)
{
    //LIB_I2CReciveMemPoll(I2c_Addr, GW_GRAY_ADDR_DEF, 0xB0, BufferPoint, 0x08);
}
