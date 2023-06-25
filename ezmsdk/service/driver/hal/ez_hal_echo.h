
/*******************************************************************************
* Filename:         ez_hal_echo.h
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

/** @file   ez_hal_echo.h
 *  @author Hai Nguyen
 *  @date   13.06.2023
 *  @brief  Public functions to use the echo driver
 *
 *  @details -
 *
 */

#ifndef _EZ_HAL_ECHO_H
#define _EZ_HAL_ECHO_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "service/driver/ez_hal_driver_def.h"
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define ECHO_INTERFACE_1_NAME   "echo_interface_1"
#define ECHO_INTERFACE_2_NAME   "echo_interface_2"


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function : ezHalEcho_Initialize
*//**
* @Description: Initialize the HAL Echo driver.
*
*               Internally, this function bind the HAL driver to the target
*               driver and initialize the target driver
*
* @param: -
*
* @return EZ_DRIVER_OK is success, else error code
*
*******************************************************************************/
ezDriverStatus_t ezHalEcho_Initialize(void);


/******************************************************************************
* Function : ezHalEcho_GetDriver
*//**
* @Description: Get the driver handle. Additionall, register a callback for
*               receiving event
*
* @param (IN)driver_name: name of the driver
* @param (OUT)handle: driver handle
* @param (IN)callback: callback to receive events from the HAL
*
* @return EZ_DRIVER_OK is success, else EZ_DRIVER_ERR_NO_DRIVER
*
*******************************************************************************/
ezDriverStatus_t ezHalEcho_GetDriver(char *driver_name,
                                     ezDriverHandle_t *handle,
                                     ezDriverCallback callback);


/******************************************************************************
* Function : ezHalEcho_ReleaseDriver
*//**
* @Description: Release the driver. Release the resources
*
* @param (IN)handle: pointer to driver handle
*
* @return Always return EZ_DRIVER_OK
*
*******************************************************************************/
ezDriverStatus_t ezHalEcho_ReleaseDriver(ezDriverHandle_t *handle);


/******************************************************************************
* Function : ezHalEcho_Write
*//**
* @Description: Write data to the echo device
*
* @param (IN)handle: name of the driver
* @param (IN)buff: data to be written
* @param (IN)buff_size: size of written buffer
*
* @return Number of bytes written in the echo device
*
*******************************************************************************/
uint32_t ezHalEcho_Write(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size);


/******************************************************************************
* Function : ezHalEcho_Read
*//**
* @Description: Read data from the echo device
*
* @param (IN)handle: name of the driver
* @param (OUT)buff: data to be read
* @param (IN)buff_size: size of read buffer
*
* @return Number of bytes read from the echo device
*
*******************************************************************************/
uint32_t ezHalEcho_Read(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size);


#endif /* _EZ_HAL_ECHO_H */

/* End of file */

