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

// For any internal configuration
typedef struct{
	runnableStr_t *runnable;
	uint32_t remainTimeMS;
}runnableInfo_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

extern runnableStr_t Loc_arrStrRunnables[MAX_SUPPORTED_TASKS];

static runnableInfo_t g_arrRuninfo[MAX_SUPPORTED_TASKS]; 

// variable to determie if we should call sched
static uint32_t g_pendingTicks;


/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/

static void tickcb(void)
{
	g_pendingTicks++;
}

static void sched(void)
{
	uint32_t idx;
	for(idx = 0; idx < MAX_SUPPORTED_TASKS; idx++)
	{
		// if task not initialized, calling cb will cause a crash as cb is null
		if(g_arrRuninfo[idx].runnable && g_arrRuninfo[idx].runnable->callback && g_arrRuninfo[idx].remainTimeMS == 0)
		{

			g_arrRuninfo[idx].runnable->callback();

			g_arrRuninfo[idx].remainTimeMS = g_arrRuninfo[idx].runnable->periodicity;
		}

		g_arrRuninfo[idx].remainTimeMS -= TICK_TIME; // One MS passed

	}
}

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


void sched_init(void)
{
	// init vars (if needed)
	uint32_t i;
	for (i = 0; i < MAX_SUPPORTED_TASKS; i++)
	{
		g_arrRuninfo[i].runnable = &Loc_arrStrRunnables[i];
		g_arrRuninfo[i].remainTimeMS = Loc_arrStrRunnables[i].first_delayMS;
	}
	
	// systick configure
	STK_SetTime_ms(TICK_TIME);
	STK_SetCallBack(tickcb);

	//we start systick at sched_strat
	
}

void sched_start(void)
{
	STK_Start();
	while(1)
	{
		// if pendingTicks is greater than one, it means
		// that cpu load is over 100%, because two syscalls
		// occurred when sched was already running
		if(g_pendingTicks)
		{
			g_pendingTicks--;
			sched();
		}
	}
}

/*
Res_t sched_registerrunnable(runnable_t* r)
{
	if(r && (g_arrRuninfo[r->priority].runnable == NULL_t))
	{
		g_arrRuninfo[r->priority].runnable = r;
		g_arrRuninfo[r->priority].remainTimeMS = r->delayMS;
	}		
}
*/
