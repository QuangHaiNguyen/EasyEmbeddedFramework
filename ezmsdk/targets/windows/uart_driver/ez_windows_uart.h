
/*******************************************************************************
* Filename:         ez_windows_uart.h
* Author:           Hai Nguyen
* Original Date:    25.06.2023
* Last Update:      25.06.2023
*
* -----------------------------------------------------------------------------
* Company:          Embedded Easy
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Embedded Easy
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 25.06.2023
*
*******************************************************************************/

/** @file   ez_windows_uart.h
 *  @author Hai Nguyen
 *  @date   25.06.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _EZ_WINDOWS_UART_H
#define _EZ_WINDOWS_UART_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdbool.h"
#include "stdint.h" 
#include "service/driver/hal_uart/ez_target_uart_def.h"
#include "service/driver/ez_hal_driver_def.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* Nonen */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
const struct ezTargetUartDriver *ezWinUart_GetDriver(uint8_t driver_index);


/******************************************************************************
* Function : ezTargetEcho_SetCallback
*//**
* @Description: Set callback function to notify the event to the HAL layer
*
* @param: (IN)driver_index: index of the driver.
* @param: (IN)callback: pointer to callback function
*
* @return: Pointer to the driver or NULL. See ezDriver.
*
*
*******************************************************************************/
void ezWinUart_SetCallback(uint8_t driver_index,
                           ezDriverCallback callback);


/******************************************************************************
* Function : ezTargetEcho_GetConfiguration
*//**
* @Description: Return the configuration of a corresponding driver
*
* @param: (IN)driver_index: index of the driver.
*
* @return   Pointer to the driver or NULL. See ezDriver.
*
*******************************************************************************/
struct ezTargetUartConfiguration *ezWinUart_GetConfiguration(uint8_t driver_index);


#endif /* _EZ_WINDOWS_UART_H */

/* End of file */

