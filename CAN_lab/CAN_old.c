/*
 * CAN.c
 *
 *  Created on: Nov 30, 2024
 *      Author: elder
 */
#include "CAN.h"

void CANMasterInit(uint32_t ui32Base, uint32_t ui32BitRate){
    // Enable GPIO and CAN peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // GPIO for CAN0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0); // CAN0 Module

    // Wait until the peripherals are ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_CAN0)) {}

    // Configure GPIO pins for CAN0 (PB4 = RX, PB5 = TX)
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // Initialize the CAN controller
    CANInit(ui32Base);

    // Set the bit rate for CAN communication
    CANBitRateSet(ui32Base, SysCtlClockGet(), ui32BitRate);

    // Enable CAN for operation
    CANEnable(ui32Base);
}

void CANTransmit(uint32_t ui32Base, uint32_t ui32MsgID, uint8_t *pui8Data, uint32_t ui32MsgLen) {
    //    if (ui32MsgLen > 8) {
    //        return; // Invalid length, return without transmitting
    //    }

    tCANMsgObject sMsgObject;
    sMsgObject.ui32MsgID = ui32MsgID;            // Set the message ID
    sMsgObject.ui32MsgIDMask = 0;               // No masking
    sMsgObject.ui32Flags = MSG_OBJ_TX_INT_ENABLE; // Enable TX interrupt
    sMsgObject.ui32MsgLen = ui32MsgLen;         // Set the data length
    sMsgObject.pui8MsgData = pui8Data;          // Set the pointer to the data

    CANMessageSet(ui32Base, 1, &sMsgObject, MSG_OBJ_TYPE_TX);
}


// Slave Initialization (Receiver)
void CANSlaveInit(uint32_t ui32Base, uint32_t ui32BitRate) {
    // Enable GPIO and CAN peripherals
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB); // GPIO for CAN1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_CAN0); // CAN1 Module

    // Wait until the peripherals are ready
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB)) {}
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_CAN0)) {}

    // Configure GPIO pins for CAN1 (PE4 = RX, PE5 = TX)
    GPIOPinConfigure(GPIO_PB4_CAN0RX);
    GPIOPinConfigure(GPIO_PB5_CAN0TX);
    GPIOPinTypeCAN(GPIO_PORTB_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // Initialize the CAN controller
    CANInit(ui32Base);

    // Set the bit rate for CAN communication
    CANBitRateSet(ui32Base, SysCtlClockGet(), ui32BitRate);

    // Enable CAN for operation
    CANEnable(ui32Base);

    // Configure the receiver to accept messages with any ID
    tCANMsgObject sMsgObject;
    sMsgObject.ui32MsgID = 0x14;                   // Accept all IDs
    sMsgObject.ui32MsgIDMask = 0x10;              // No masking
    sMsgObject.ui32Flags = MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER; // Enable RX interrupt
    sMsgObject.ui32MsgLen = 8;                 // Expected message length
    CANMessageSet(ui32Base, 1, &sMsgObject, MSG_OBJ_TYPE_RX);
}


uint32_t ReceiveMessage(uint32_t ui32Base, uint8_t *pui8Buffer) {
    tCANMsgObject sMsgObject;
    sMsgObject.pui8MsgData = pui8Buffer; // Point to the provided buffer

    // Get the message from Message Object 1
    CANMessageGet(ui32Base, 1, &sMsgObject, true);

    // Return the length of the received message
    return sMsgObject.ui32MsgLen;
}
