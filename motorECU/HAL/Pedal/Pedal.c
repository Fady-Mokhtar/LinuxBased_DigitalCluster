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

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "stm32f1xx.h"
#include "Pedal.h"


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
 * @brief Minimum ADC value corresponding to the minimum pedal press.
 *
 * This macro defines the minimum ADC value corresponding to the minimum amount of pressure required on the pedal.
 * It represents the lowest possible value that the ADC can output.
 */
#define PEDAL_LOWER_BOUND_ADC 0

/**
 * @brief Maximum ADC value corresponding to the maximum pedal press.
 *
 * This macro defines the maximum ADC value corresponding to the maximum amount of pressure allowed on the pedal.
 * It represents the highest possible value that the ADC can output given its 12-bit resolution (4095).
 */
#define PEDAL_UPPER_BOUND_ADC 4095


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

ADC_HandleTypeDef ADCHandler =
{
	 .Instance = PEDAL_ADC_HANDLER,
	 .Init.ScanConvMode = ADC_SCAN_DISABLE,
	 .Init.ContinuousConvMode = ENABLE,
	 .Init.DiscontinuousConvMode = DISABLE,
	 .Init.ExternalTrigConv = ADC_SOFTWARE_START,
	 .Init.DataAlign = ADC_DATAALIGN_RIGHT,
	 .Init.NbrOfConversion = 1,
};


/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

uint32_t Pedal_getRead(void)
{
	HAL_ADC_PollForConversion(&ADCHandler, 1);
	uint32_t ADCRead = HAL_ADC_GetValue(&ADCHandler);

    /* Map ADC value to the defined pedal press range */
    uint32_t pedalPress = ((ADCRead - PEDAL_LOWER_BOUND_ADC) * (PEDAL_UPPER_BOUND - PEDAL_LOWER_BOUND)) /
                          (PEDAL_UPPER_BOUND_ADC - PEDAL_LOWER_BOUND_ADC) + PEDAL_LOWER_BOUND;

	return ADCRead;
}
