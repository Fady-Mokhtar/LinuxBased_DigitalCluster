#ifndef E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_LIB_ERRORS_H_
#define E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_LIB_ERRORS_H_

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/**
 * @brief Enumeration defining error types for MCAL.
 */
// typedef enum
// {
//     MCAL_OK = 0x00U,

//     MCAL_NOK = 0x01U,

//     MCAL_NULL_PTR = 0x02U,

//     MCAL_WRONG_INPUTS = 0x03U,

//     MCAL_ERROR

// } MCAL_ErrorStatus_t;

typedef enum
{
    MCAL_OK = 0x00U,

    MCAL_ERROR,

    MCAL_BUSY,

    MCAL_TIMEOUT,

    MCAL_NULL_PTR,

    MCAL_WRONG_INPUTS

   

} MCALStatus_t;

/**
 * @brief Enumeration defining error types for HAL.
 */
// typedef enum
// {
//     HAL_OK = 0x00U,

//     HAL_NOK = 0x01U,

//     HAL_NULL_PTR = 0x02U,

//     HAL_WRONG_INPUTS = 0x03U,

//     HAL_LCD_FULL_BUFFER = 0x04U

// } HAL_ErrorStatus_t;

/**
 * @brief Enumeration defining error types for HAL.
 */
/*
typedef enum
{
    HAL_OK = 0x00U,

    HAL_NOK = 0x01U,

    HAL_NULL_PTR = 0x02U,

    HAL_WRONG_INPUTS = 0x03U,

    HAL_LCD_FULL_BUFFER = 0x04U

} HALStatus_t;*/

// /**
//  * @brief Enumeration defining error types for UART.
//  */
// typedef enum
// {
//     USART_OK = 0,
//     USART_NULL_PTR,
//     USART_VALUE_EXCEED,
//     USART_BUSY,
//     USART_WRONG_INPUTS,
//     USART_ERROR

// } USART_ErrorStatus_t;

// /**
//  * @brief Enumeration defining error types for DMA.
//  */
// typedef enum
// {
//     DMA_OK = 0x00U,

//     DMA_NOK = 0x01U,

//     DMA_NULL_PTR = 0x02U,

//     DMA_WRONG_INPUTS = 0x03U

// } DMA_ErrorStatus_t;



#endif // E__ITI_FADY_ARM_STM32F401CC_BLACKPILL_DRIVERS_LIB_ERRORS_H_
