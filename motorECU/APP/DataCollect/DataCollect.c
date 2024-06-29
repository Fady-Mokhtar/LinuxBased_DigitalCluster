/*
 * DataCollect.c
 *
 *  Created on: Jun 27, 2024
 *      Author: abdel
 */

#include "main.h"
#include "Indicators.h"
#include "Encoder.h"
#include "DC_MOTOR.h"
#define LEFT_LED_BIT   			0
#define RIGHT_LED_BIT   		1
#define WAITING_LED_BIT   		2
#define INDICATORS_LED_BIT   	3

extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart1;

uint8_t DataCollect_mcuData[4] = { 0 };

void _DataCollect_100MS_runnable(void)
{
	uint8_t Speed_value = 0x00;
	uint32_t RPM_value = 0x00;
	uint8_t FUEL_TEMP_value = 0x00;
	uint8_t GEAR_value = 0x00;
	static uint8_t Ind_value = 0x00;

	INDICATORS_updatePotValues();
	HAL_Delay(2);

	Speed_value = INDICATORS_getPotValue(POT_PEDAL);
	DC_MOTOR_Set_Speed(0, Speed_value);
	RPM_value = Encoder_GetRPM(&htim1);
	FUEL_TEMP_value = INDICATORS_getPotValue(POT_FUEL_TEMP);
	GEAR_value = INDICATORS_getPotValue(POT_GEAR);

	if (INDICATORS_isButtonPressed(BUTTON_LeftLed)) {
		Ind_value ^= (1 << LEFT_LED_BIT);
	}
	if (INDICATORS_isButtonPressed(BUTTON_RightLed)) {
		Ind_value ^= (1 << RIGHT_LED_BIT);
	}
	if (INDICATORS_isButtonPressed(BUTTON_WaitingLed)) {
		Ind_value ^= (1 << WAITING_LED_BIT);
	}
	if (INDICATORS_isButtonPressed(BUTTON_IndicatorsLed)) {
		Ind_value ^= (1 << INDICATORS_LED_BIT);
	}

	// Map [0-180] to [0-80]
	static uint32_t Sent_RPM;
	if(RPM_value <= 370)
	{
		RPM_value = ((RPM_value) * (80.0)) / (370.0);
		Sent_RPM  = RPM_value;
	}
	DataCollect_mcuData[0] = Sent_RPM;
 	DataCollect_mcuData[1] = FUEL_TEMP_value;
	DataCollect_mcuData[2] = GEAR_value;
	DataCollect_mcuData[3] = Ind_value;
	unsigned char buff[100];
	//unsigned int stsize = sprintf(buff, "RPM : %d\n", Sent_RPM);
	HAL_UART_Transmit(&huart1, DataCollect_mcuData, 4, 0xFF);
}

