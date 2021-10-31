/*******************************************************************************
* Title                 :   serial component
* Filename              :   serial.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.01.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  24.01.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   serial.h
 *  @brief  Header file for the serial module
 */


#ifndef _SERIAL_H
#define _SERIAL_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "hal_uart_config.h"

#if defined (SIMULATION)
#warning running simulation
#include <stdio.h>
#include "debug.h"
#include <stdbool.h>

#elif defined (EMBEDDED) 
/*embedded dependent libraries*/

#else
/*no platform is defined*/
#error unknown platform
#endif

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/*Window and linux dependent libraries*/



/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
//None

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/* Init serial module*/
HAL_Status Serial_Init(void);

/* checkif serial modula has initialized*/
bool Serial_IsInit(void);

/*serial module receive data*/
HAL_Status Serial_Receive(HAL_UART *uart, uint8_t size);

/*serial transmit data*/
HAL_Status Serial_Transmit(HAL_UART *uart, uint8_t size);

/*serial get status*/
HAL_Status Serial_GetStatus(void);

/*de-init serial module*/
HAL_Status Serial_DeInit(void);

#endif /* _SERIAL_H */

/* End of file*/