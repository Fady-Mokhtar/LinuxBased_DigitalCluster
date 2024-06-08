/**
 * @file Pedal.h
 * @author Ziad Gamalelden (ziad.gamalelden@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-04-28
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef PEDAL_PEDAL_H_
#define PEDAL_PEDAL_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include <stdint.h>
#include "Pedal_cfg.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/**
 * @brief Macro to map a value from one range to another.
 *
 * This macro maps an input value from an input range defined by input minimum and maximum
 * to an output range defined by output minimum and maximum.
 *
 * @param VAL The value to be mapped.
 * @param IN_MIN The minimum value of the input range.
 * @param IN_MAX The maximum value of the input range.
 * @param OUT_MIN The minimum value of the output range.
 * @param OUT_MAX The maximum value of the output range.
 * @return The mapped value within the specified output range.
 */
#define MAP_VALUE(VAL, IN_MIN, IN_MAX, OUT_MIN, OUT_MAX) \
    (((VAL) - (IN_MIN)) * ((OUT_MAX) - (OUT_MIN)) / ((IN_MAX) - (IN_MIN)) + (OUT_MIN))

/**
 * @brief Macro to map an ADC value to the defined pedal press range.
 *
 * This macro maps an ADC value to the pedal press range defined by PEDAL_LOWER_BOUND and PEDAL_UPPER_BOUND.
 */
#define MAP_ADC_TO_PEDAL(ADC_VAL) \
    MAP_VALUE((ADC_VAL), PEDAL_LOWER_BOUND_ADC, PEDAL_UPPER_BOUND_ADC, PEDAL_LOWER_BOUND, PEDAL_UPPER_BOUND)



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/



uint32_t Pedal_getRead(void);


#endif /* PEDAL_PEDAL_H_ */
