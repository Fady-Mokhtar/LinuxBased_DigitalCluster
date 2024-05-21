/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

/* Include Libraries  */

/* Driver Libraries   */
#include "STK.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/* By default systick using AHB/8 prescaler in STK-CTRL so that the clock is equal to 2_MHZ */

#ifndef F_CPU
#define F_CPU   16000000UL            // MP_tick      
#endif

/* 
    Define a structure named SYSTICK to represent the SysTick registers.
    The structure contains four volatile uint32_t members:
    - STK_CTRL: SysTick Reload Value Register
    - STK_LOAD: SysTick Control and Status Register
    - STK_VAL: SysTick Current Value Register
    - STK_CALIB: SysTick Calibration Register
    
    Each member is accompanied by a comment indicating its offset and access type.
*/
typedef struct
{
    volatile uint32_t STK_CTRL;               /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */                                                              
    volatile uint32_t STK_LOAD;               /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
    volatile uint32_t STK_VAL;                /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
    volatile const uint32_t STK_CALIB;        /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
}SYSTICK;

// #define SCS_BASE            (0xE000E000UL)                            /*!< System Control Space Base Address */
// #define SysTick_BASE        (SCS_BASE +  0x0010UL)                    /*!< SysTick Base Address */
// #define SysTick             ((SysTick_Type   *)     SysTick_BASE  )   /*!< SysTick configuration struct */


#define STK_BASE_ADDR   0xE000E010UL
#define STK     ((volatile SYSTICK* const)(STK_BASE_ADDR))

/* SysTick Control/Status Register Definitions */
#define STK_MASK_CTRL_COUNTFLAG_Pos         16U                                            /*!< SysTick CTRL: COUNTFLAG Position */
#define STK_MASK_CTRL_COUNTFLAG_Msk         (1UL << STK_MASK_CTRL_COUNTFLAG_Pos)           /*!< SysTick CTRL: COUNTFLAG Mask */
#define STK_MASK_CTRL_CLKSOURCE_Pos          2U                                            /*!< SysTick CTRL: CLKSOURCE Position */
#define STK_MASK_CTRL_CLKSOURCE_Msk         (1UL << STK_MASK_CTRL_CLKSOURCE_Pos)           /*!< SysTick CTRL: CLKSOURCE Mask */
#define STK_MASK_CTRL_TICKINT_Pos            1U                                            /*!< SysTick CTRL: TICKINT Position */
#define STK_MASK_CTRL_TICKINT_Msk           (1UL << STK_MASK_CTRL_TICKINT_Pos)             /*!< SysTick CTRL: TICKINT Mask */
#define STK_MASK_CTRL_ENABLE_Pos             0U                                            /*!< SysTick CTRL: ENABLE Position */
#define STK_MASK_CTRL_ENABLE_Msk            (1UL /*<< SysTick_CTRL_DISABLE_Pos*/)          /*!< SysTick CTRL: ENABLE Mask */
#define STK_MASK_CTRL_DISABLE_Pos            0U                                            /*!< SysTick CTRL: DISABLE Position */
#define STK_MASK_CTRL_DISABLE_Msk           (0UL /*<< SysTick_CTRL_DISABLE_Pos*/)          /*!< SysTick CTRL: DISABLE Mask */
/* SysTick load Register Definitions */
#define STK_MASK_LOAD_RELOAD_Pos             0U                                            /*!< SysTick LOAD: RELOAD Position */
#define STK_MASK_LOAD_RELOAD_Msk            (0xFFFFFFUL /*<< SysTick_LOAD_RELOAD_Pos*/)    /*!< SysTick LOAD: RELOAD Mask */
/* SysTick current Register Definitions */
#define STK_MASK_VAL_CURRENT_Pos             0U                                            /*!< SysTick VAL: CURRENT Position */
#define STK_MASK_VAL_CURRENT_Msk            (0xFFFFFFUL /*<< SysTick_VAL_CURRENT_Pos*/)    /*!< SysTick VAL: CURRENT Mask */
/* SysTick calibration Register Definitions */
#define STK_MASK_CALIB_NOREF_Pos            31U                                            /*!< SysTick CALIB: NOREF Position */
#define STK_MASK_CALIB_NOREF_Msk            (1UL << STK_MASK_CALIB_NOREF_Pos)               /*!< SysTick CALIB: NOREF Mask */
#define STK_MASK_CALIB_SKEW_Pos             30U                                            /*!< SysTick CALIB: SKEW Position */
#define STK_MASK_CALIB_SKEW_Msk             (1UL << STK_MASK_CALIB_SKEW_Pos)                /*!< SysTick CALIB: SKEW Mask */
#define STK_MASK_CALIB_TENMS_Pos             0U                                            /*!< SysTick CALIB: TENMS Position */
#define STK_MASK_CALIB_TENMS_Msk            (0xFFFFFFUL /*<< SysTick_CALIB_TENMS_Pos*/)    /*!< SysTick CALIB: TENMS Mask */


/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/* 
    Define an enumeration named STK_ExpireState_t to represent the possible states of SysTick expiration.
    It contains two values:
    - STK_Expire: Represents the state when SysTick has expired.
    - STK_NotExpire: Represents the state when SysTick has not expired.
*/
typedef enum   
{
    STK_NotExpire,
    STK_Expire
} STK_ExpireState_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

/* 
    Define a static variable g_app_cbf of type STK_CBF_t.
    This variable is initialized with the value NULL_t, indicating that it currently does not point to any function.
    It is declared as static, meaning it has file scope and retains its value between function calls.
*/
static STK_CBF_t g_app_cbf = NULL_t;

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

/**
  \brief   Set Time in milliseconds
  \details Sets the time for the SysTick timer in milliseconds.
           The maximum time that can be reached is 8 seconds.   ()
  \param [in]      Copy_TimeReqMS  Time to set in milliseconds.
  \return                          Status indicating whether the operation was successful or not.
                                    - MCAL_OK: Operation successful.
                                    - MCAL_WRONG_INPUTS: Invalid input parameters.
*/
MCALStatus_t STK_SetTime_ms(uint32_t Copy_TimeReqMS)
{
    MCALStatus_t Loc_STKErrorState = MCAL_OK;

    /* Disable the SysTick clock source (AHB/8)*/
    STK->STK_CTRL &= ~(STK_MASK_CTRL_CLKSOURCE_Msk);

    /* Enable the SysTick Interrupt */
    STK->STK_CTRL |= (STK_MASK_CTRL_TICKINT_Msk);

    /* Calculate the preload value for the SysTick timer */
    /*!< Tick_Time is equal to 1/2 MicroSec */
    /*!< That means we can have a time reach up to 8 sec only */
    /*!< Copy_TimeRequired *1000 to be micro then *2 instead of divide/0.5 then -1 for the tickless when return Hw constrain*/
    uint32_t Loc_PreloadVal = ((Copy_TimeReqMS * 2000) - 0x01);             

    /* Check if the preload value is within valid range */
    if (Loc_PreloadVal > STK_MASK_LOAD_RELOAD_Msk || Loc_PreloadVal < 0x0)
    {
        Loc_STKErrorState = MCAL_WRONG_INPUTS;
    }
    else    
    {
        /* Set the preload value for the SysTick timer */
        STK->STK_LOAD = Loc_PreloadVal;
    }
    
    return Loc_STKErrorState;
}

/**
  \brief   Start SysTick Timer
  \details Starts the SysTick timer.
  \return  Status indicating whether the operation was successful or not.
           - MCAL_OK: Operation successful.
*/
MCALStatus_t STK_Start(void)
{
    MCALStatus_t Loc_STKErrorState = MCAL_OK;

    STK->STK_VAL = 0;
    /* Enable the SysTick timer */
    STK->STK_CTRL |= STK_MASK_CTRL_ENABLE_Msk;

    return Loc_STKErrorState;
}

/**
  \brief   Stop SysTick Timer
  \details Stops the SysTick timer.
  \return  Status indicating whether the operation was successful or not.
           - MCAL_OK: Operation successful.
*/
MCALStatus_t STK_Stop(void)
{
    MCALStatus_t Loc_STKErrorState = MCAL_OK;

    /* Disable the SysTick timer */
    STK->STK_CTRL &= (~STK_MASK_CTRL_DISABLE_Msk);

    return Loc_STKErrorState;
}

/**
  \brief   Check SysTick Expiration
  \details Checks if the SysTick timer has expired.
  \param [out]     Copy_ExpireStatus  Pointer to store the expire status.
  \return                            Status indicating whether the operation was successful or not.
                                     - MCAL_OK: Operation successful.
*/
MCALStatus_t STK_IsExpire(uint32_t *Copy_ExpireStatus)
{
    MCALStatus_t Loc_STKErrorState = MCAL_OK;

    /* Check if the COUNTFLAG bit is set in the SysTick control register */
    if ((STK->STK_CTRL & STK_MASK_CTRL_COUNTFLAG_Msk) != 0)
    {
        *Copy_ExpireStatus = STK_Expire;
    }
    else 
    {
        *Copy_ExpireStatus = STK_NotExpire;
    }

    return Loc_STKErrorState;
}

/**
  \brief   Set SysTick Callback
  \details Sets a callback function to be called in the SysTick interrupt handler.
  \param [in]      Copy_CallBackAddr  Pointer to the callback function.
  \return                            Status indicating whether the operation was successful or not.
                                     - MCAL_OK: Operation successful.
*/
MCALStatus_t STK_SetCallBack(STK_CBF_t Copy_CallBackAddr)
{
    MCALStatus_t Loc_STKErrorState = MCAL_OK;

    /* Set the global callback function pointer */
    g_app_cbf = Copy_CallBackAddr;

    return Loc_STKErrorState;
}

/**
  \brief   SysTick Interrupt Handler
  \details Handles the SysTick interrupt and calls the registered callback function if available.
  \return  None
*/
void SysTick_Handler(void)
{
    /* Check if a callback function is registered */
    if (g_app_cbf)
    {
        /* Call the registered callback function */
        g_app_cbf();
    }
}
