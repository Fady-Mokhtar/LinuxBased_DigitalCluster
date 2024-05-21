/*
 * Comm.h
 *
 *  Created on: May 4, 2024
 *      Author: abdel
 */

#ifndef SRC_COMM_COMM_H_
#define SRC_COMM_COMM_H_

#include "stm32f1xx_hal.h"

#define INTERFACE  COMM_UART


// Communication interface types
typedef enum {
    COMM_UART,
    COMM_CAN
} Comm_Interface;

// Function prototypes
void Comm_Init(Comm_Interface interface);
HAL_StatusTypeDef Comm_Publish(Comm_Interface interface, uint8_t *data, uint16_t length);
HAL_StatusTypeDef Comm_Collect(Comm_Interface interface, uint8_t *buffer, uint16_t length);



#endif /* SRC_COMM_COMM_H_ */
