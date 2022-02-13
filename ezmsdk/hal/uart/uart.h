/*******************************************************************************
* Title                 :   module
* Filename              :   module.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  21.02.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   module.h
 *  @brief  Header template for a module
 */


#ifndef _HAL_UART_H
#define _HAL_UART_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdint.h"
#include "stdbool.h"
#include "../../app/app_config.h"

#if (NUM_OF_SUPPORTED_UART > 0U)


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of the notification code
 *  
 */
typedef enum
{
    UART_TX_COMPLT,     /**< UART transmit completed */
    UART_RX_COMPLT,     /**< UART receive completed */
    UART_BUFF_FULL,     /**< UART buffer is full */
    UART_UNSUPPORTED,   /**< UART unsupported callback */
}UART_NOTIFY_CODE;

/** @brief definition the callback function pointer 
 *  
 */
typedef uint8_t (*UART_CALLBACK)(uint8_t eCode, void *param1, void *param2);

/** @brief definition of api set for UART
 *  
 */
typedef struct
{
    uint16_t(*ezmUart_Send)         (uint8_t *au8Buffer, uint16_t u16Size);
    uint16_t(*ezmUart_Receive)      (uint8_t * au8Buffer, uint16_t u16Size);
    void(*ezmUart_RegisterCallback) (UART_CALLBACK pfnCallback);
    void(*ezmUart_UnregisterCallback)(void);
}UartDrvApi;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void* GetUart0Driver(void);


#endif /* NUM_OF_SUPPORTED_UART > 0U */
#endif /* _HAL_UART_H */

/* End of file*/
