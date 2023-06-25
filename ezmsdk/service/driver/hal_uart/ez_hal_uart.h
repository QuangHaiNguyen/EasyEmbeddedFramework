
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
 *  @brief  This is the source for a module
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
ezDriverStatus_t ezHalUart_Initialize(void);

ezDriverStatus_t ezHalUart_GetDriver(char *driver_name,
                                        ezDriverHandle_t *handle,
                                        ezDriverCallback callback);

ezDriverStatus_t ezHalUart_ReleaseDriver(ezDriverHandle_t *handle);

uint32_t ezHalUart_Write(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size);

uint32_t ezHalUart_Read(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size);


#endif /* _EZ_HAL_UART_H */

/* End of file */

