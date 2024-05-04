
#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#define HAL_TIM_MODULE_ENABLED

#include "stm32f1xx_hal.h"

// DC Motor Rotation Directions
#define DIR_CW    0
#define DIR_CCW   1

// DC Motor PWM Properties
#define DC_MOTOR_PWM_RES  10
#define DC_MOTOR_F_PWM    500

// The Number OF DC MOTORs To Be Used In The Project
#define DC_MOTOR_UNITS  1


/**
 * @brief Configuration Struct for the Motor.
 * 
 * DIR1_GPIO : The Port to which DIR1 is connected to
 * DIR2_GPIO : The Port to which DIR2 is connected to
 * DIR1_PIN : The Pin to which DIR1 is connected to
 * DIR2_PIN : The Pin to which DIR2 is connected to
 * TIM_Instance : Pointer to Timer instance (Registers Struct) as
 *                defined in the stm32 HAL which is TIM1, TIM2...
 * PWM_TIM_CH : The Timer channel which will generate PWM signal for the motor driver
 * TIM_CLK_MHz : The Timer Clock in Mhz
 * PWM_FREQ_Hz : The desired PWM Frequency in Hz
 * PWM_RES_BITS : The resolution of PWM
 * 
 */
typedef struct
{
	GPIO_TypeDef * DIR1_GPIO;
	GPIO_TypeDef * DIR2_GPIO;
	uint16_t       DIR1_PIN;
	uint16_t       DIR2_PIN;
	TIM_TypeDef*   TIM_Instance;
	uint32_t       PWM_TIM_CH;
	uint16_t       TIM_CLK_MHz;
	uint32_t       PWM_FREQ_Hz;
	uint8_t        PWM_RES_BITS;
}DC_MOTOR_CfgType;


/*-----[ Prototypes For All Functions ]-----*/

/**
 * @brief Initializes all the PINs used by the Motor Driver and The PWM to be used as
 * configured in DC_MOTOR_cfg.c
 * 
 * @param au8_MOTOR_Instance Instance number as defined in DC_MOTOR_cfg.c
 */
void DC_MOTOR_Init(uint8_t au8_MOTOR_Instance);

/**
 * @brief Start the Motor with the given initial speed
 * 
 * @param au8_MOTOR_Instance Instance number as defined in DC_MOTOR_cfg.c
 * @param au8_DIR Motor Direction DIR_CW or DIR_CCW
 * @param au16_SPEED Motor Speed
 */
void DC_MOTOR_Start(uint8_t au8_MOTOR_Instance, uint8_t au8_DIR, uint16_t au16_SPEED);

/**
 * @brief Sets the Motor speed to the given value
 * 
 * @param au8_MOTOR_Instance Instance number as defined in DC_MOTOR_cfg.c
 * @param au16_SPEED Motor Speed
 */
void DC_MOTOR_Set_Speed(uint8_t au8_MOTOR_Instance, uint16_t au16_SPEED);

/**
 * @brief Sets the Rotation direction of the motor to the given direction
 * 
 * @param au8_MOTOR_Instance Instance number as defined in DC_MOTOR_cfg.c
 * @param au8_DIR Motor Direction DIR_CW or DIR_CCW
 */
void DC_MOTOR_Set_Dir(uint8_t au8_MOTOR_Instance, uint8_t au8_DIR);

/**
 * @brief Stops the Motor
 * 
 * @param au8_MOTOR_Instance Instance number as defined in DC_MOTOR_cfg.c
 */
void DC_MOTOR_Stop(uint8_t au8_MOTOR_Instance);

/**
 * @brief Returns the Maximum operating frequency in Hz for the PWM channel at the
 * selected PWM resolution in the config structure for that specific motor
 * 
 * @param au8_MOTOR_Instance Instance number as defined in DC_MOTOR_cfg.c
 * @return uint32_t Thre returned Maximum frequency
 */
uint32_t DC_MOTOR_Get_MaxFreq(uint8_t au8_MOTOR_Instance);


#endif /* DC_MOTOR_H_ */
