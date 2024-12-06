/*
 * CAN_CONFIG.c
 *
 *  Created on: Dec 6, 2024
 *      Author: elder
 */


/*************************************************************Includes**************************************************************/
#include "CAN_CONFIG.h"


 CAN_ErrorStatus CAN_Errors = {0};

 CAN_MessageObjectConfig MessagObjects[]=
{
 {
  1, //the number of the object
  0x27, //the msgID
  0x20, //the msgIDMask
  MSG_OBJ_RX_INT_ENABLE | MSG_OBJ_USE_ID_FILTER, //the msgFlag
  8, //the msgLEN
  CAN_ROLE_RECIEVE//the role of object whether master of slave
 }


};


 CAN_ModuleConfig CAN_Modules[] = {
{
 SYSCTL_PERIPH_CAN0,
 SYSCTL_PERIPH_GPIOB,
 CAN0_BASE,       // Base address of the CAN module (e.g., CAN0_BASE)
 GPIO_PORTB_BASE,      // GPIO port base address (e.g., GPIO_PORTB_BASE)
 GPIO_PIN_4,         // RX pin number
 GPIO_PIN_5,         // TX pin number
 GPIO_PB4_CAN0RX,   // RX pin configuration
 GPIO_PB5_CAN0TX,   // TX pin configuration
 0,
 500000,
 1,
 MessagObjects,
 &CAN_Errors // Pointer to error status
}
};
