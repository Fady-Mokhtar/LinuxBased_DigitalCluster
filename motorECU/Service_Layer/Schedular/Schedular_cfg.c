/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */

/* Driver Libraries   */
#include "Schedular.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/



/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

extern void Traffic_Green(void);
extern void Traffic_Yellow(void);
extern void Traffic_Red(void);


/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

//	Traffic_Lights

runnableStr_t Loc_arrStrRunnables[LED_TASKS] = {
        [0] = 
        {
            .name = "green_runnable_1",
			.first_delayMS = 0,
			.periodicity = 500,
			.priority = 0,
			.callback = Traffic_Green
        },

        [1] = 
        {
            .name = "yellow_runnable_2",
			.first_delayMS = 0,
			.periodicity = 1000,
			.priority = 0,
			.callback = Traffic_Yellow
        },
		[2] =
		{
			.name = "red_runnable_3",
			.first_delayMS = 0,
			.periodicity = 3000,
			.priority = 0,
			.callback = Traffic_Red
		}

};


