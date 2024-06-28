#ifndef INDICATORS_H_
#define INDICATORS_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "Indicators_cfg.h"

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef enum {
    BUTTON_NOT_PRESSED = 0,
    BUTTON_PRESSED = 1
} Button_state_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
 * @brief Initialize the indicators module.
 */
void INDICATORS_init(void);

/**
 * @brief Get the button state only if it has changed.
 * @param buttonID The ID of the button.
 * @return Button_state_t indicating if the button is pressed (1) or not pressed/still pressed (0).
 */
Button_state_t INDICATORS_isButtonPressed(Button_ID_t buttonID);


/**
 * @brief Update the potentiometer values.
 */
void INDICATORS_updatePotValues(void);

/**
 * @brief Get the potentiometer read value.
 * @param potID The ID of the potentiometer.
 * @return uint8_t The mapped value of the potentiometer (0-100).
 */
uint8_t INDICATORS_getPotValue(POT_ID_t potID);


#endif /* INDICATORS_H_ */
