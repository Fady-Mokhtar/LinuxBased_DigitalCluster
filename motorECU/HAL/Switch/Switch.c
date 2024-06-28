/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "switch.h"
#include "switch_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define IS_SWITCH_NAME_VALID(SW) ((SW) < _NUM_OF_SWITCHES)

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

extern const SWITCH_switchConfig_t Switches[_NUM_OF_SWITCHES];

static uint8_t switchState[_NUM_OF_SWITCHES] = {0};

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void SWITCH_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    for (uint32_t i = 0; i < _NUM_OF_SWITCHES; i++) {
        GPIO_InitStruct.Pin = Switches[i].pin;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = Switches[i].mode;
        HAL_GPIO_Init(Switches[i].port, &GPIO_InitStruct);
    }
}

uint32_t SWITCH_getState(uint32_t switchName)
{
    if (!IS_SWITCH_NAME_VALID(switchName)) {
        return SWITCH_InvalidSwitchName;
    }

    uint32_t pinRead = HAL_GPIO_ReadPin(Switches[switchName].port, Switches[switchName].pin);
    pinRead = pinRead ^ ((Switches[switchName].mode == GPIO_PULLUP) ? 1 : 0);

    return pinRead;
}

void _SWITCH_getState_5MS_runnable(void)
{
    static uint32_t switchPrevState[_NUM_OF_SWITCHES] = {0};
    static uint32_t switchStateCounter[_NUM_OF_SWITCHES] = {0};
    uint32_t iter = 0, currState = 0;

    for (iter = 0; iter < _NUM_OF_SWITCHES; iter++) {
        currState = HAL_GPIO_ReadPin(Switches[iter].port, Switches[iter].pin);
        currState = currState ^ ((Switches[iter].mode == GPIO_PULLUP) ? 1 : 0);

        if (currState == switchPrevState[iter]) {
            switchStateCounter[iter]++;
        } else {
            switchStateCounter[iter] = 0;
        }

        if (switchStateCounter[iter] == 5) {
            switchState[iter] = currState;
            switchStateCounter[iter] = 0;
        }

        switchPrevState[iter] = currState;
    }
}

uint32_t SWITCH_getStateAsync(uint32_t switchName)
{
    return switchState[switchName];
}
