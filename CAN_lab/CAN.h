/*
 * CAN.h
 *
 *      ModuleName: CAN
 *      Author: Ahmed Mohamed
 *      Purpose: Header file for CAN Driver
 */

#ifndef MCAL_CAN_H_
#define MCAL_CAN_H_

/*************************************************************Includes**************************************************************/


#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"

#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"


/*************************************************************Defines**************************************************************/

// Error Codes
#define CAN_NO_ERROR                0x00 // No error
#define CAN_ERROR_GPIO_INIT         0x01 // Error in GPIO initialization
#define CAN_ERROR_CAN_INIT          0x02 // Error in CAN module initialization
#define CAN_ERROR_TRANSMIT          0x03 // Error in transmitting data
#define CAN_ERROR_RECEIVE           0x04 // Error in receiving data
#define CAN_ERROR_INVALID_OBJECT    0x05 // Invalid message object
#define CAN_ERROR_BITRATE           0x06 // Error in setting bit rate


#define CAN_ROLE_SEND             0x01
#define CAN_ROLE_RECIEVE              0x02

/*************************************************************Structs**************************************************************/

// Structure to hold error status
typedef struct {
    uint8_t gpioInitError;       // GPIO initialization error
    uint8_t canInitError;        // CAN module initialization error
    uint8_t transmitError;       // Transmission error
    uint8_t receiveError;        // Reception error
    uint8_t bitRateError;        // Bit rate setting error
    uint8_t invalidObjectError;  // Invalid object error
} CAN_ErrorStatus;

/* Structure for CAN message object configuration
(to declare many objects and choose the role whether
to be sender or receiver and some other information
about the object)*/
typedef struct {
    uint8_t objectID;   //the object id
    uint32_t msgID;     //the message ID that is wanted to send or received
    uint32_t msgIDMask; //MSGID mask
    uint32_t flags;     //whether sending or receiving
    uint8_t msgLen;     //msg length in bytes
    uint8_t role;       //is this object sending or receiving
} CAN_MessageObjectConfig;

// Structure for CAN module configuration
typedef struct {
    uint32_t SYSCTL_PERIPH_CAN;         // To activate clock to the can module
    uint32_t SYSCTL_PERIPH_GPIO;        // To activate clock to the GPIO module
    uint32_t canBase;                   // Base address of the CAN module (e.g., CAN0_BASE)
    uint32_t gpioBase;                  // GPIO port base address (e.g., GPIO_PORTB_BASE)
    uint32_t rxPin;                     // RX pin number
    uint32_t txPin;                     // TX pin number
    uint32_t rxPinConfig;               // RX pin configuration
    uint32_t txPinConfig;               // TX pin configuration
    uint8_t InterruptEnable;            // if interrupt needed
    uint32_t bitRate;                   //baud rate
    uint8_t numObjects;                 //Number of objects in this can base
    CAN_MessageObjectConfig* objects;   //struct to enter the data of each object
    CAN_ErrorStatus* errors;            // Pointer to error status
} CAN_ModuleConfig;


/*******************************************************Function Prototypes**********************************************************/

void CANInitialize(uint8_t ModuleIndex);
void CANTransmit(uint8_t ModuleIndex, uint8_t objectID, uint8_t *pui8Data, uint32_t ui32MsgLen);
uint32_t ReceiveMessage(uint8_t ModuleIndex, uint8_t objectID,uint8_t *pui8Buffer);

#endif /* MCAL_CAN_H_ */
