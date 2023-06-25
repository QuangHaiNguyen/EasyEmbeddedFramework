
/*******************************************************************************
* Filename:         ez_hal_uart_target_def.h
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

/** @file   ez_hal_uart_target_def.h
 *  @author Hai Nguyen
 *  @date   25.06.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _EZ_HAL_UART_TARGET_DEF_H
#define _EZ_HAL_UART_TARGET_DEF_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "string.h"
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
 */
typedef enum
{
    UART_TX_CMPLT,
    UART_RX_CMPLT,
    UART_ERROR,
}ezHalUartCallbackCode_t;


/** @brief Number of stop bit
 */
typedef enum
{
    ONE_BIT,            /**< 1 stop bit */
    ONE_AND_HALF_BIT,   /**< 1.5 stop bit */
    TWO_BITS,           /**< 2 stop bits */
}ezHalUartStopBit_t;


/** @brief Parity value
 */
typedef enum
{
    NONE,               /**< None */
    ODD,                /**< Odd */
    EVEN,               /**< Even */
    MARK,               /**< Mark */
    SPACE,              /**< Space */
}ezHalUartParity_t;


/** @brief Target configuration data
 */
struct ezTargetUartConfiguration
{
    char                *port_name; /**< */
    uint8_t             byte_size;  /**< */
    uint32_t            baudrate;   /**< */
    ezHalUartParity_t   parity;     /**< */
    ezHalUartStopBit_t  stop_bit;   /**< */
};


typedef void (*ezTargetUart_Initialize)(uint8_t driver_index);


typedef uint32_t(*ezTargetUart_Write)(uint8_t driver_index,
                                        uint8_t *data,
                                        uint32_t data_size);


typedef uint32_t(*ezTargetUart_Read)(uint8_t driver_index,
                                        uint8_t *data,
                                        uint32_t data_size);


typedef uint32_t(*ezTargetUart_WriteBlocking)(uint8_t driver_index,
                                                uint8_t *data,
                                                uint32_t data_size);


typedef uint32_t(*ezTargetUart_ReadBlocking)(uint8_t driver_index,
                                                uint8_t *data,
                                                uint32_t data_size);


/** @brief  Hold the functions must be implemented by the target driver.
 *          These functions are expected by the Uart HAL layer
 */
struct ezTargetUartApi
{
    ezTargetUart_Initialize Initialize; /**< Poiter to the initialized function */
    ezTargetUart_Write Write;           /**< Pointer to the write function */
    ezTargetUart_Read Read;              /**< Pointer to the read function */
};


/** @brief Hold the data and api, which must be implemented by the targer driver.
 *         This block is the boiler plate for the implementation of the driver.
 */
struct ezTargetUartDriver
{
    bool initialized;                       /**< Driver inialized completeted flag */
    uint8_t index;                          /**< Index value of the driver */
    struct ezHalEchoConfiguration config;   /**< Configuration of the target driver */
    struct ezTargetEchoApi api;             /**< Pointer to the API implementation */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* None */

#endif /* _EZ_HAL_UART_TARGET_DEF_H */

/* End of file */

