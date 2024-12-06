/*
 * CAN.h
 *
 *  Created on: Nov 30, 2024
 *      Author: elder
 */

#ifndef MCAL_CAN_H_
#define MCAL_CAN_H_



#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"

void CANMasterInit(uint32_t ui32Base, uint32_t ui32BitRate);
void CANTransmit(uint32_t ui32Base, uint32_t ui32MsgID, uint8_t *pui8Data, uint32_t ui32MsgLen);
void CANSlaveInit(uint32_t ui32Base, uint32_t ui32BitRate);
uint32_t ReceiveMessage(uint32_t ui32Base, uint8_t *pui8Buffer);

#endif /* MCAL_CAN_H_ */
