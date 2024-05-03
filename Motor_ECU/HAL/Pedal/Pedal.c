/**
 * @file Pedal.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-04-28
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "stm32f1xx.h"
ADC_HandleTypeDef ADCHandler =
{
	 .Instance = ADC1,
	 .Init.ScanConvMode = ADC_SCAN_DISABLE,
	 .Init.ContinuousConvMode = ENABLE,
	 .Init.DiscontinuousConvMode = DISABLE,
	 .Init.ExternalTrigConv = ADC_SOFTWARE_START,
	 .Init.DataAlign = ADC_DATAALIGN_RIGHT,
	 .Init.NbrOfConversion = 1,
};

uint32_t Pedal_getRead(void)
{
	HAL_ADC_PollForConversion(&ADCHandler, 1);
	uint32_t ADCRead = HAL_ADC_GetValue(&ADCHandler);

	return ADCRead;
}
