#ifndef E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_INCLUDE_SERVICE_SCHEDULAR_H_
#define E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_INCLUDE_SERVICE_SCHEDULAR_H_



/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */
#include "Errors.h"
#include "STD_TYPES.h"

#include "STK.h"

/* Driver Libraries   */
#include "Schedular_cfg.h"
/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

typedef void (*runnable_t) (void);

// for any configurations configurable by the use
typedef struct
{
	char *name;
	uint32_t first_delayMS;             // time until the task is first executed
	uint32_t periodicity;
	uint32_t priority;
	runnable_t callback;
	
}runnableStr_t;



/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

// To enable timers
void sched_init(void);

// all tasks must be created between the init and the start
// Although our implementation will allow tasks to be run after starting sched
void sched_start(void);

//Res_t sched_registerrunnable(runnable_t *runnable);


#endif // E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_INCLUDE_SERVICE_SCHEDULAR_H_