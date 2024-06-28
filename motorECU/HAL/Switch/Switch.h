#ifndef SWITCH_H_
#define SWITCH_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "stm32f1xx_hal.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

#define SWITCH_PRESSED             0U
#define SWITCH_RELEASED            1U

#define SWITCH_MODE_NOPULL         GPIO_NOPULL
#define SWITCH_MODE_PULLUP         GPIO_PULLUP
#define SWITCH_MODE_PULLDOWN       GPIO_PULLDOWN

#define SWITCH_ACTIVE_STATE_HIGH   0U
#define SWITCH_ACTIVE_STATE_LOW    1U

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t mode;
} SWITCH_switchConfig_t;

typedef enum {
    SWITCH_Ok,
    SWITCH_Nok,
    SWITCH_NullPtr,
    SWITCH_InvalidSwitchName,
} SWITCH_errorState_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

void SWITCH_init(void);

uint32_t SWITCH_getState(uint32_t switchName);

uint32_t SWITCH_getStateAsync(uint32_t switchName);

#endif /* SWITCH_H_ */