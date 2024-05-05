/**
 * @file MotorControl.h
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-05
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef MOTORCONTROL_MOTORCONTROL_H_
#define MOTORCONTROL_MOTORCONTROL_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "MotorControl_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/



/**
 * @brief Initialize the motor controller.
 *
 * This function initializes the motor controller, setting up any necessary parameters
 * and configurations for controlling the motor.
 */
void MotorControl_init();

/**
 * @brief Update the motor control.
 *
 * This function updates the motor control algorithm and sends the control signal to the motor.
 * It should be called periodically at a fixed interval defined by MOTOR_CONTROL_SAMPLE_TIME_S.
 *
 * @note This function must be called every MOTOR_CONTROL_SAMPLE_TIME_S to ensure proper motor control.
 *       It adjusts the motor control parameters and sends the control signal to the motor accordingly.
 */
void MotorControl_update();

#endif /* MOTORCONTROL_MOTORCONTROL_H_ */
