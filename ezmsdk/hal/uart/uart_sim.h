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


#ifndef _UART_SIM_H
#define _UART_SIM_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "uart.h"

#if(HAL_UART)
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1
/**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *  
 */

typedef struct
{
    int a;
    /**< an integer */
    int b;
    /**< an integer */
}aType;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
bool simUart_Init(void);
UartDrvApi *simUart_GetApi(uint8_t hw_uart_index);

/******************************************************************************
* Function Prototypes
*******************************************************************************/
#endif

#endif /* _UART_SIM_H */

/* End of file*/
