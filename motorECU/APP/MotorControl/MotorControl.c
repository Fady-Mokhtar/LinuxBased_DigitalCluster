/**
 * @file MotorControl.c
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MotorControl.h"
#include "PID.h"
#include "Pedal.h"
#include <stdint.h>
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
PIDController pid =
{
    .Kp         = MOTOR_CONTROL_PID_KP,
    .Ki         = MOTOR_CONTROL_PID_KI,
    .Kd         = MOTOR_CONTROL_PID_KD,
    .tau        = MOTOR_CONTROL_PID_TAU,
    .limMin     = MOTOR_CONTROL_PID_LIM_MIN,
    .limMax     = MOTOR_CONTROL_PID_LIM_MAX,
    .limMinInt  = MOTOR_CONTROL_PID_LIM_MIN_INT,
    .limMaxInt  = MOTOR_CONTROL_PID_LIM_MAX_INT,
    .T          = MOTOR_CONTROL_SAMPLE_TIME_S
};


/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void MotorControl_init()
{
   PIDController_Init(&pid);
}

void MotorControl_update()
{

	/* Get Pedal Read */
	uint32_t PedalRead = Pedal_getRead();

	/* Determine Target speed */
	float targetSpeed = (PedalRead * MOTOR_MAX_SPEED) / 100.0;

	/* Get Current speed */
	/*encoder code*/
	float currentSpeed;
	/* Get output signal using PID */
	float controlSignal = PIDController_Update(&pid, targetSpeed, currentSpeed);

	/* Send the output signal to the Motor Driver


}





