

#include "../DC_MOTOR/DC_MOTOR.h"

const DC_MOTOR_CfgType DC_MOTOR_CfgParam[DC_MOTOR_UNITS] =
{
	// DC MOTOR 1 Configurations
    {
	    GPIOB,
		GPIOB,
		GPIO_PIN_12,
		GPIO_PIN_13,
		TIM2,
		TIM_CHANNEL_1,
		8,
		DC_MOTOR_F_PWM,
		DC_MOTOR_PWM_RES
	}
};
