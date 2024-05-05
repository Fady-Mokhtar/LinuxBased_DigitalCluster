#ifndef MOTORCONTROL_MOTORCONTROL_CFG_H_
#define MOTORCONTROL_MOTORCONTROL_CFG_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/* Motor Parameters */

#define MOTOR_MAX_SPEED 100
/* Controller parameters */

/**
 * @brief Proportional gain (Kp) of the PID controller.
 *
 * The proportional gain determines how aggressively the controller responds to the current error.
 */
#define MOTOR_CONTROL_PID_KP  2.0f

/**
 * @brief Integral gain (Ki) of the PID controller.
 *
 * The integral gain accounts for past errors and helps to eliminate steady-state errors.
 */
#define MOTOR_CONTROL_PID_KI  0.5f

/**
 * @brief Derivative gain (Kd) of the PID controller.
 *
 * The derivative gain predicts future behavior of the error based on its current rate of change.
 */
#define MOTOR_CONTROL_PID_KD  0.25f

/**
 * @brief Time constant (Tau) of the derivative low-pass filter.
 *
 * The time constant represents the time interval over which the derivative term is averaged,
 * acting as a low-pass filter to reduce noise and high-frequency oscillations in the derivative term.
 */
#define MOTOR_CONTROL_PID_TAU 0.02f

/**
 * @brief Minimum output limit of the PID controller.
 *
 * This sets the lower boundary for the control signal to prevent the controller from driving the system
 * beyond acceptable limits.
 */
#define MOTOR_CONTROL_PID_LIM_MIN -10.0f

/**
 * @brief Maximum output limit of the PID controller.
 *
 * This sets the upper boundary for the control signal to prevent the controller from driving the system
 * beyond acceptable limits.
 */
#define MOTOR_CONTROL_PID_LIM_MAX  10.0f

/**
 * @brief Minimum integral windup limit of the PID controller.
 *
 * This sets the lower boundary for the integral term to prevent excessive accumulation of integral errors.
 */
#define MOTOR_CONTROL_PID_LIM_MIN_INT -5.0f

/**
 * @brief Maximum integral windup limit of the PID controller.
 *
 * This sets the upper boundary for the integral term to prevent excessive accumulation of integral errors.
 */
#define MOTOR_CONTROL_PID_LIM_MAX_INT  5.0f

/**
 * @brief Sample time of the PID controller.
 *
 * The sample time represents the time interval between successive controller updates.
 */
#define MOTOR_CONTROL_SAMPLE_TIME_S 0.01f













#endif /* MOTORCONTROL_MOTORCONTROL_CFG_H_ */
