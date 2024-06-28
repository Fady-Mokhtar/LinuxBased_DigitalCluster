/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "switch.h"
#include "switch_cfg.h"

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

const SWITCH_switchConfig_t Switches[_NUM_OF_SWITCHES] =
{
    [SWITCH_LeftLed] = {
        .port = GPIOA,
        .pin = GPIO_PIN_4,
        .mode = SWITCH_MODE_PULLUP
    },
    [SWITCH_RightLed] = {
        .port = GPIOA,
        .pin = GPIO_PIN_5,
        .mode = SWITCH_MODE_PULLUP
    },
    [SWITCH_WaitingLed] = {
        .port = GPIOA,
        .pin = GPIO_PIN_6,
        .mode = SWITCH_MODE_PULLUP
    },
    [SWITCH_indicatorsLed] = {
        .port = GPIOA,
        .pin = GPIO_PIN_7,
        .mode = SWITCH_MODE_PULLUP
    }
};
