#ifndef ENCODER_H
#define ENCODER_H

#include "stm32f1xx_hal.h"

// Define the number of pulses per revolution (PPR)
#define PPR 1024

// Function to initialize the encoder interface
void Encoder_Init(TIM_HandleTypeDef *htim);

// Function to get the current encoder counter value
uint32_t Encoder_GetCounter(TIM_HandleTypeDef *htim);

// Function to calculate RPM based on the encoder readings
uint32_t Encoder_GetRPM(TIM_HandleTypeDef *htim);

#endif // ENCODER_H
