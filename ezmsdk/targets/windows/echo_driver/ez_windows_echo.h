
/*******************************************************************************
* Filename:         ez_windows_echo.h
* Author:           Hai Nguyen
* Original Date:    13.06.2023
* Last Update:      13.06.2023
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
* Written by Hai Nguyen 13.06.2023
*
*******************************************************************************/

/** @file   ez_windows_echo.h
 *  @author Hai Nguyen
 *  @date   13.06.2023
 *  @brief  Implementation fo the Echo driver
 *  
 *  @details: Provide the functions to bind the implementation of the Echo
 *            driver to the HAL driver
 * 
 */

#ifndef _EZ_WINDOWS_ECHO_H
#define _EZ_WINDOWS_ECHO_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdbool.h"
#include "stdint.h" 
#include "service/driver/hal_echo/ez_hal_echo_target_def.h"
#include "service/driver/ez_hal_driver_def.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */


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

/******************************************************************************
* Function : ezTargetEcho_GetDriver
*//**
* @Description: Return the deriver handle the HAL layer.
*
* @param    (IN)driver_index: index of the driver.
*
* @return   Pointer to the driver or NULL.
*
*******************************************************************************/
const struct ezHalEchoDriver *ezTargetEcho_GetDriver(uint8_t driver_index);


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
void ezTargetEcho_SetCallback(uint8_t driver_index,
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
struct ezHalEchoConfiguration *ezTargetEcho_GetConfiguration(uint8_t driver_index);

#endif /* _EZ_WINDOWS_ECHO_H */

/* End of file */
