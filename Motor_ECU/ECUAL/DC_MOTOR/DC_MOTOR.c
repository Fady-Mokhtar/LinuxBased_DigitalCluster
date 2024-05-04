

#include "../DC_MOTOR/DC_MOTOR.h"
#include "../DC_MOTOR/DC_MOTOR_cfg.h"
#include "../../Inc/main.h"

/* Period value. Calculated during DC_MOTOR_Init */
static uint16_t g_Period;

static void Init_Clock(uint8_t au8_MOTOR_Instance ,TIM_HandleTypeDef* htim)
{

	if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO == GPIOA || DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO == GPIOA)
    {
    	__HAL_RCC_GPIOA_CLK_ENABLE();
    }
    else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO == GPIOB || DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO == GPIOB)
    {
    	__HAL_RCC_GPIOB_CLK_ENABLE();
    }
    else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO == GPIOC || DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO == GPIOC)
    {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }
    else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO == GPIOD || DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO == GPIOD)
    {
        __HAL_RCC_GPIOD_CLK_ENABLE();
    }
    else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO == GPIOE || DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO == GPIOE)
    {
        __HAL_RCC_GPIOE_CLK_ENABLE();
    }


	if(htim_base->Instance==TIM1)
	{
		__HAL_RCC_TIM1_CLK_ENABLE();
	}
	else if(htim_base->Instance==TIM2)
	{
		__HAL_RCC_TIM2_CLK_ENABLE();
	}
	else if(htim_base->Instance==TIM3)
	{
		__HAL_RCC_TIM3_CLK_ENABLE();
	}
}

static void Init_Pins(uint8_t au8_MOTOR_Instance ,TIM_HandleTypeDef* htim)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/*--------[ Configure The 2 Direction Control GPIO Pins ]-------*/
    
	GPIO_InitStruct.Pin = DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO, &GPIO_InitStruct);


	/* Configure the PWM pin */
	/* It's hard coded configured to channel 1 in timer 2, in case of 
	chaning the configuration, this part needs to be modified */
	if(htim->Instance==TIM2)
	{	

		__HAL_RCC_GPIOA_CLK_ENABLE();
	
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* switch(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH)
		{
			case TIM_CHANNEL_1:
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_InitStruct.Pin = GPIO_PIN_0;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
				break;

			case TIM_CHANNEL_2:
				__HAL_RCC_GPIOA_CLK_ENABLE();
				GPIO_InitStruct.Pin = GPIO_PIN_1;
				HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	
				break;

			case TIM_CHANNEL_3:
				GPIO_InitStruct.Pin = GPIO_PIN_10;
				HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
				break;

			case TIM_CHANNEL_4:
				GPIO_InitStruct.Pin = GPIO_PIN_11;
				HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);	
				break;

			default:
				break;
		} */
		
	}

}

void DC_MOTOR_Init(uint8_t au8_MOTOR_Instance)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_HandleTypeDef htim;
    uint32_t PSC_Value = 0;
    uint32_t ARR_Value = 0;
    uint8_t i = 0;


	/*--------[ Start Clocks before configuring ]-------*/

	Init_Clock(au8_MOTOR_Instance ,&htim);

	
	/* Configure the Direction and PWM pins */
	Init_Pins(au8_MOTOR_Instance, &htim);

	/* Ensure the valuse of direction pins are initialzied to 0  */
	HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_PIN, 0);
	HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_PIN, 0);

	/*--------[ Calculate The PSC & ARR Values To Set PWM Resolution And Approx. The F_pwm ]-------*/

	/* Those Equations Sets The PWM Resolution & Approximates The F_pwm */
	ARR_Value = 1;
	for(i=0; i<DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_RES_BITS; i++)
	{
		ARR_Value *= 2;
	}
	PSC_Value = (uint32_t) ((DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_CLK_MHz*1000000) / (ARR_Value*DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_FREQ_Hz));
	PSC_Value--;
	ARR_Value -= 2;
	g_Period = ARR_Value;
	/*--------[ Configure The DC Motor PWM Timer Channel ]-------*/

	htim.Instance = DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance;
	htim.Init.Prescaler = PSC_Value;
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = g_Period;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
/* 	HAL_TIM_Base_Init(&htim); */	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim, &sClockSourceConfig);
	HAL_TIM_PWM_Init(&htim);
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim, &sConfigOC, DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH);
	/* HAL_TIM_MspPostInit(&htim); */

	/*--------[ Start The PWM Channel ]-------*/

	HAL_TIM_PWM_Start(&htim, DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH);
}


void DC_MOTOR_Start(uint8_t au8_MOTOR_Instance, uint8_t au8_DIR, uint8_t au8_SPEED)
{
	uint32_t loadValue; 

	if(au16_SPEED > 100)
	{

	}
	else
	{
		/* Write To The 2 Direction Control Pins */
		if(au8_DIR == DIR_CW)
		{
			HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_PIN, 1);
			HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_PIN, 0);
		}
		else if(au8_DIR == DIR_CCW)
		{
			HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_PIN, 0);
			HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_PIN, 1);
		}


		loadValue = (au16_SPEED * g_Period) / 100;

		/* Write The Speed Value To The PWM CH DutyCycle Register */
		if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_1)
		{
			DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR1 = loadValue;
		}
		else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_2)
		{
			DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR2 = loadValue;
		}
		else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_3)
		{
			DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR3 = loadValue;
		}
		else
		{
			DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR4 = loadValue;
		}
	}
}


void DC_MOTOR_Set_Speed(uint8_t au8_MOTOR_Instance, uint8_t au8_SPEED)
{

	uint32_t loadValue; 

	if(au16_SPEED > 100)
	{

	}
	else
	{
		loadValue = (au16_SPEED * g_Period) / 100;

		/* Write The Speed Value To The PWM CH DutyCycle Register */
		if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_1)
		{
			DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR1 = loadValue;
		}
		else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_2)
		{
			DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR2 = loadValue;
		}
		else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_3)
		{
			DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR3 = loadValue;
		}
		else
		{
			DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR4 = loadValue;
		}
	}
}


void DC_MOTOR_Set_Dir(uint8_t au8_MOTOR_Instance, uint8_t au8_DIR)
{
	/* Write To The 2 Direction Control Pins */
	if(au8_DIR == DIR_CW)
	{
		HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_PIN, 1);
		HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_PIN, 0);
	}
	else if(au8_DIR == DIR_CCW)
	{
		HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_PIN, 0);
		HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_PIN, 1);
	}
}


void DC_MOTOR_Stop(uint8_t au8_MOTOR_Instance)
{
	/* Write To The 2 Direction Control Pins */
	HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR1_PIN, 0);
	HAL_GPIO_WritePin(DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_GPIO, DC_MOTOR_CfgParam[au8_MOTOR_Instance].DIR2_PIN, 0);

	/* Write ZERO To The PWM Ch DutyCycle Register */
	if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_1)
	{
		DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR1 = 0;
	}
	else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_2)
	{
		DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR2 = 0;
	}
	else if(DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_TIM_CH == TIM_CHANNEL_3)
	{
		DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR3 = 0;
	}
	else
	{
		DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_Instance->CCR4 = 0;
	}
}


uint32_t DC_MOTOR_Get_MaxFreq(uint8_t au8_MOTOR_Instance)
{
	uint32_t ARR_Value = 1;
    uint8_t i = 0;

	for(i=0; i<DC_MOTOR_CfgParam[au8_MOTOR_Instance].PWM_RES_BITS; i++)
	{
		ARR_Value *= 2;
	}
	return ((DC_MOTOR_CfgParam[au8_MOTOR_Instance].TIM_CLK_MHz*1000000)/ARR_Value);
}
