/**
 * @file Pedal_cfg.h
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2024-05-03
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef PEDAL_PEDAL_CFG_H_
#define PEDAL_PEDAL_CFG_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/


/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/**
 * @brief Lower bound of the pedal press range.
 *
 * This macro defines the lower bound of the pedal press range used in the application.
 * When mapping ADC values to the pedal press range, the minimum ADC value is mapped to this lower bound.
 */
#define PEDAL_LOWER_BOUND 0

/**
 * @brief Upper bound of the pedal press range.
 *
 * This macro defines the upper bound of the pedal press range used in the application.
 * When mapping ADC values to the pedal press range, the maximum ADC value is mapped to this upper bound.
 */
#define PEDAL_UPPER_BOUND 100





/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/




#endif /* PEDAL_PEDAL_CFG_H_ */
