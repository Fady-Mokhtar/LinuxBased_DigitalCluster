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

/**
 * @brief ADC handler for pedal readings.
 *
 * This macro defines the ADC handler used for reading pedal values in the application.
 * When reading pedal values from the ADC, this handler is used to interact with the ADC peripheral.
 * The specific ADC handler (e.g., ADC1, ADC2, etc.) is assigned to this macro.
 */
#define PEDAL_ADC_HANDLER ADC1






/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/




#endif /* PEDAL_PEDAL_CFG_H_ */
