
/*******************************************************************************
* Filename:         ez_hal_uart.h
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

/** @file   ez_hal_uart.h
 *  @author Hai Nguyen
 *  @date   25.06.2023
 *  @brief  Public function to use the HAL UART driver
 *  
 *  @details
 * 
 */

#ifndef _EZ_HAL_UART_H
#define _EZ_HAL_UART_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "service/driver/ez_hal_driver_def.h"
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *  
 */


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function : ezHalUart_Initialize
*//**
* @Description:
*
* This function initializes the UART abstraction layer
*
* @param    None
* @return   None
*
*******************************************************************************/
ezDriverStatus_t ezHalUart_Initialize(void);


/******************************************************************************
* Function : ezHalUart_Deinitialize
*//**
* @Description:
*
* This function deinitializes the UART abstraction layer
*
* @param    None
* @return   None
*
*******************************************************************************/
ezDriverStatus_t ezHalUart_Deinitialize(void);


/******************************************************************************
* Function : ezHalUart_GetDriver
*//**
* @Description:
*
* This function returns the driver handle which giving access to the HAL api
*
* @param    (IN)driver_name: Name of the driver in used
* @param    (IN)handle: driver handle to access the driver
* @param    (IN)callback: callback function to handle the event
*
* @return   EZ_DRIVER_OK if success, else ERROR status
*
*******************************************************************************/
ezDriverStatus_t ezHalUart_GetDriver(char *driver_name,
                                     ezDriverHandle_t *handle,
                                     ezDriverCallback callback);


/******************************************************************************
* Function : ezHalUart_Deinitialize
*//**
* @Description:
*
* This function releases the driver which is previously used by
* ezHalUart_GetDriver()
*
* @param    (IN)handle: driver handle
*
* @return   EZ_DRIVER_OK if success, else ERROR status
*
*******************************************************************************/
ezDriverStatus_t ezHalUart_ReleaseDriver(ezDriverHandle_t *handle);


/******************************************************************************
* Function : ezHalUart_WriteBlocking
*//**
* @Description:
*
* This function uses the driver handle to send data to UART bus. This is a
* blocking function.
*
* @param    (IN)handle: driver handle, see ezHalUart_GetDriver()
* @param    (IN)buff: buffer to be written
* @param    (IN)buff_size: size of buff
*
* @return   Number of byte being written.
*
*******************************************************************************/
uint32_t ezHalUart_WriteBlocking(ezDriverHandle_t *handle,
                                 uint8_t *buff,
                                 uint32_t buff_size);


/******************************************************************************
* Function : ezHalUart_ReadBlocking
*//**
* @Description:
*
* This function uses the driver handle to read data from UART bus. This is a
* blocking function.
*
* @param    (IN)handle: driver handle, see ezHalUart_GetDriver()
* @param    (IN)buff: buffer to store read data
* @param    (IN)buff_size: size of buff
*
* @return   Number of byte being read.
*
*******************************************************************************/
uint32_t ezHalUart_ReadBlocking(ezDriverHandle_t *handle,
                                uint8_t *buff,
                                uint32_t buff_size);


/******************************************************************************
* Function : ezHalUart_WriteBlocking
*//**
* @Description:
*
* This function uses the driver handle to send data to UART bus. This is a
* non blocking function.
*
* @param    (IN)handle: driver handle, see ezHalUart_GetDriver()
* @param    (IN)buff: buffer to be written
* @param    (IN)buff_size: size of buff
*
* @return   Number of byte being written.
*
*******************************************************************************/
uint32_t ezHalUart_Write(ezDriverHandle_t *handle,
                         uint8_t *buff,
                         uint32_t buff_size);



/******************************************************************************
* Function : ezHalUart_ReadBlocking
*//**
* @Description:
*
* This function uses the driver handle to read data from UART bus. This is a
* non blocking function.
*
* @param    (IN)handle: driver handle, see ezHalUart_GetDriver()
* @param    (IN)buff: buffer to store read data
* @param    (IN)buff_size: size of buff
*
* @return   Number of byte being read.
*
*******************************************************************************/
uint32_t ezHalUart_Read(ezDriverHandle_t *handle,
                        uint8_t *buff,
                        uint32_t buff_size);


#endif /* _EZ_HAL_UART_H */

/* End of file */

