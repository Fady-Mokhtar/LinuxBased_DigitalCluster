/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "Indicators.h"

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

typedef struct {
    uint32_t currentState;
    uint32_t previousState;
} ButtonStatus_t;

static ButtonStatus_t ButtonStatus[_NUM_OF_BUTTONS];
extern ADC_HandleTypeDef hadc1;
/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void INDICATORS_init(void)
{
    // Initialize the SWITCH module
    SWITCH_init();

    // Initialize button states
    for (uint32_t i = 0; i < _NUM_OF_BUTTONS; i++) {
        ButtonStatus[i].currentState = SWITCH_getState(ButtonSwitchMap[i]);
        ButtonStatus[i].previousState = ButtonStatus[i].currentState;
    }

    // Initialize ADC for potentiometers
    INDICATORS_updatePotValues();
}

Button_state_t INDICATORS_isButtonPressed(Button_ID_t buttonID)
{
    if (buttonID >= _NUM_OF_BUTTONS) {
        return BUTTON_NOT_PRESSED; // Invalid button ID
    }

    // Read the current state of the button
    ButtonStatus[buttonID].currentState = SWITCH_getState(ButtonSwitchMap[buttonID]);

    // Determine if the button state has changed from not pressed to pressed
    Button_state_t state = (ButtonStatus[buttonID].currentState == BUTTON_PRESSED) &&
                           (ButtonStatus[buttonID].previousState == BUTTON_NOT_PRESSED);

    // Update the previous state
    ButtonStatus[buttonID].previousState = ButtonStatus[buttonID].currentState;

    return state;
}

void INDICATORS_updatePotValues(void)
{
    HAL_ADC_Start_DMA(&hadc1, PotentioBuffer, _NUM_OF_POTS);
}

uint8_t INDICATORS_getPotValue(POT_ID_t potID)
{
    if (potID >= _NUM_OF_POTS) {
        return 0; // Invalid pot ID
    }

    /* Map [0-4026] to [0-100] */
    return (uint8_t)((PotentioBuffer[potID] * 100) / 4026);
}

