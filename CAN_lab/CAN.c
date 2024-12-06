/*
 * CAN.c
 *
 *  Created on: Nov 30, 2024
 *      Author: elder
 */
#include "CAN.h"
#include "CAN_CONFIG.h"

void CANInitialize(uint8_t ModuleIndex){
    // Enable GPIO and CAN peripherals
    SysCtlPeripheralEnable(CAN_Modules[ModuleIndex].SYSCTL_PERIPH_GPIO); // GPIO for CAN0
    SysCtlPeripheralEnable(CAN_Modules[ModuleIndex].SYSCTL_PERIPH_CAN); // CAN0 Module

    // Wait until the peripherals are ready
    while (!SysCtlPeripheralReady(CAN_Modules[ModuleIndex].SYSCTL_PERIPH_GPIO)) {}
    while (!SysCtlPeripheralReady(CAN_Modules[ModuleIndex].SYSCTL_PERIPH_CAN)) {}

    // Configure GPIO pins for CAN0 (PB4 = RX, PB5 = TX)
    GPIOPinConfigure(CAN_Modules[ModuleIndex].rxPinConfig);
    GPIOPinConfigure(CAN_Modules[ModuleIndex].txPinConfig);
    GPIOPinTypeCAN(CAN_Modules[ModuleIndex].gpioBase, CAN_Modules[ModuleIndex].rxPin | CAN_Modules[ModuleIndex].txPin);

    // Initialize the CAN controller
    CANInit(CAN_Modules[ModuleIndex].canBase);

    // Set the bit rate for CAN communication
    CANBitRateSet(CAN_Modules[ModuleIndex].canBase, SysCtlClockGet(), CAN_Modules[ModuleIndex].bitRate);

    // Enable CAN for operation
    CANEnable(CAN_Modules[ModuleIndex].canBase);

    if (CAN_Modules[ModuleIndex].InterruptEnable == 1 )
    {
        CANIntEnable(CAN_Modules[ModuleIndex].canBase, CAN_INT_ERROR |CAN_INT_STATUS|CAN_INT_MASTER);
        IntMasterEnable();
    }
    uint8_t i ;
    for (i=0 ; i<CAN_Modules[ModuleIndex].numObjects ; i++)
    {
        tCANMsgObject msgObject;
        msgObject.ui32MsgID = CAN_Modules[ModuleIndex].objects[i].msgID;
        msgObject.ui32MsgIDMask = CAN_Modules[ModuleIndex].objects[i].msgIDMask;
        msgObject.ui32Flags = CAN_Modules[ModuleIndex].objects[i].flags;
        msgObject.ui32MsgLen = CAN_Modules[ModuleIndex].objects[i].msgLen;

        if (CAN_Modules[ModuleIndex].objects[i].role == CAN_ROLE_SEND)
        {
            CANMessageSet(CAN_Modules[ModuleIndex].canBase,
                          CAN_Modules[ModuleIndex].objects[i].objectID,
                          &msgObject,
                          MSG_OBJ_TYPE_TX );
        }
        else
        {
            CANMessageSet(CAN_Modules[ModuleIndex].canBase,
                          CAN_Modules[ModuleIndex].objects[i].objectID,
                          &msgObject,
                          MSG_OBJ_TYPE_RX );
        }
    }
}

void CANTransmit(uint8_t ModuleIndex, uint8_t objectID, uint8_t *pui8Data, uint32_t ui32MsgLen) {
    if (ui32MsgLen > 8) {
        return; // Invalid length, return without transmitting
    }
    tCANMsgObject msgObject;
    msgObject.ui32MsgID = CAN_Modules[ModuleIndex].objects[objectID-1].msgID;
    msgObject.ui32MsgIDMask = CAN_Modules[ModuleIndex].objects[objectID-1].msgIDMask;
    msgObject.ui32Flags = CAN_Modules[ModuleIndex].objects[objectID-1].flags;
    msgObject.ui32MsgLen = ui32MsgLen;
    msgObject.pui8MsgData = pui8Data;
    // Set the pointer to the data


    CANMessageSet(CAN_Modules[ModuleIndex].canBase, objectID, &msgObject, MSG_OBJ_TYPE_TX);
}




uint32_t ReceiveMessage(uint8_t ModuleIndex, uint8_t objectID, uint8_t *pui8Buffer) {
    tCANMsgObject msgObject;
//       msgObject.ui32MsgID = CAN_Modules[ModuleIndex].objects[objectID-1].msgID;
//       msgObject.ui32MsgIDMask = CAN_Modules[ModuleIndex].objects[objectID-1].msgIDMask;
//       msgObject.ui32Flags = CAN_Modules[ModuleIndex].objects[objectID-1].flags;
       msgObject.pui8MsgData = pui8Buffer; // Point to the provided buffer

    // Get the message from Message Object 1
    CANMessageGet(CAN_Modules[ModuleIndex].canBase, objectID, &msgObject, true);

    // Return the length of the received message
    return msgObject.ui32MsgLen;
}
