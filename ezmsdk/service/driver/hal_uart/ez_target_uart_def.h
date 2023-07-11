
/*******************************************************************************
* Filename:         ez_hal_uart_target_def.h
* Author:           Hai Nguyen
* Original Date:    25.06.2023
* Last Update:      07.03.2023
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
 *  @brief  This file provide the boilerplate for every implementation of the 
 *          UART driver.
 *
 *  @details The Uart driver in the target device must be implemented following
 *           this template to guarantee a smooth operation between the target
 *           and the HAL driver
 */

#ifndef _EZ_TARGET_UART_DEF_H
#define _EZ_TARGET_UART_DEF_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "service/driver/ez_hal_driver_def.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#ifndef NUM_OF_UART_INTERFACE
#define NUM_OF_UART_INTERFACE   2   /**< Number of the supported devices*/
#endif /* NUM_OF_UART_INTERFACE */

#if (NUM_OF_UART_INTERFACE > 0)
    #ifndef UART0_NAME
    #define UART0_NAME  "uart1"  /**< Name of the first UART */
    #endif /* UART0_NAME */

    #ifndef UART0_INDEX
    #define UART0_INDEX 0  /**< index of the first UART */
    #endif /* UART0_INDEX */
#endif /* (NUM_OF_UART_INTERFACE > 0) */

#if (NUM_OF_UART_INTERFACE > 1)
    #ifndef UART1_NAME
    #define UART1_NAME  "uart2"  /**< Name of the second UART */
    #endif /* UART1_NAME */

    #ifndef UART1_INDEX
    #define UART1_INDEX 1  /**< index of the second UART */
    #endif /* UART1_INDEX */
#endif /* (NUM_OF_UART_INTERFACE > 1) */

#if(WINDOWS_TARGET)
    #ifndef UART_PORT
    #define UART_PORT   "COM1"          /**< Connected port of the first driver */
    #endif /* UART_PORT */
#endif /* WINDOWS_TARGET */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Event code from the UART driver
 */
typedef enum
{
    UART_TX_CMPLT,  /**< Transmit completed */
    UART_RX_CMPLT,  /**< Reception completed */
    UART_ERROR,     /**< Error occured*/
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
    PARITY_NONE,    /**< None */
    PARITY_ODD,     /**< Odd */
    PARITY_EVEN,    /**< Even */
    PARITY_MARK,    /**< Mark */
    PARITY_SPACE,   /**< Space */
}ezHalUartParity_t;


/** @brief Target configuration data
 */
struct ezTargetUartConfiguration
{
    char                *port_name; /**< Name of the serial port */
    uint8_t             byte_size;  /**< Size of data in bit */
    uint32_t            baudrate;   /**< Baudrate*/
    ezHalUartParity_t   parity;     /**< Parity */
    ezHalUartStopBit_t  stop_bit;   /**< Number of Stop bit */
};


/** @brief Define callback for uart driver
 */
typedef ezDriverCallback ezUartCallback;


/******************************************************************************
* Function : ezTargetUart_Initialize
*//**
* @Description: Initialize the target driver
*
* @param (IN)driver_index: index of the driver
*
* @return true if success, else false
*
*******************************************************************************/
typedef bool(*ezTargetUart_Initialize)(uint8_t driver_index);


/******************************************************************************
* Function : ezTargetUart_Deinitialize
*//**
* @Description: Deinitialize the target driver
*
* @param (IN)driver_index: index of the driver
*
* @return true if success, else false
*
*******************************************************************************/
typedef bool(*ezTargetUart_Deinitialize)(uint8_t driver_index);


/******************************************************************************
* Function : ezTargetUart_Write
*//**
* @Description: Write data to targer UART bus. Non blocking operation.
*               If the operation is comnpleted. Callback function is triggered
*               with the return code UART_TX_CMPLT.
*
* @param (IN)driver_index: index of the driver
* @param (IN)data: data to be written
* @param (IN)data_size: number of byte to be written
*
* @return Number of bytes are actually written on the bus
*
*******************************************************************************/
typedef uint32_t(*ezTargetUart_Write)(uint8_t driver_index,
                                      uint8_t *data,
                                      uint32_t data_size);


/******************************************************************************
* Function : ezTargetUart_Read
*//**
* @Description: Read data to targer UART bus. Non blocking operation.
*               If the operation is comnpleted. Callback function is triggered
*               with the return code UART_RX_CMPLT.
*
* @param (IN)driver_index: index of the driver
* @param (IN)data_size: number of byte to be read
*
* @return Number of bytes are actually read from the bus
*
*******************************************************************************/
typedef uint32_t(*ezTargetUart_Read)(uint8_t driver_index,
                                     uint8_t *data,
                                     uint32_t data_size);


/******************************************************************************
* Function : ezTargetUart_WriteBlocking
*//**
* @Description: Write data to targer UART bus. Blocking operation.
*
* @param (IN)driver_index: index of the driver
* @param (IN)data: data to be written
* @param (IN)data_size: number of byte to be written
*
* @return Number of bytes are actually written on the bus
*
*******************************************************************************/
typedef uint32_t(*ezTargetUart_WriteBlocking)(uint8_t driver_index,
                                              uint8_t *data,
                                              uint32_t data_size);


/******************************************************************************
* Function : ezTargetUart_ReadBlocking
*//**
* @Description: Read data to targer UART bus. Blocking operation.
*
* @param (IN)driver_index: index of the driver
* @param (IN)data: buffer containing read data
* @param (IN)data_size: number of byte to be read
*
* @return Number of bytes are actually read from the bus
*
*******************************************************************************/
typedef uint32_t(*ezTargetUart_ReadBlocking)(uint8_t driver_index,
                                             uint8_t *data,
                                             uint32_t data_size);


/** @brief  Hold the functions must be implemented by the target driver.
 *          These functions are expected by the Uart HAL layer
 */
struct ezTargetUartApi
{
    ezTargetUart_Write          write;          /**< Pointer to the write function */
    ezTargetUart_WriteBlocking  write_blocking; /**< Pointer to the blocking write function */
    ezTargetUart_Read           read;           /**< Pointer to the read function */
    ezTargetUart_ReadBlocking   read_blocking;  /**< Pointer to the blocking read function */
};


/** @brief Hold the data and api, which must be implemented by the targer driver.
 *         This block is the boiler plate for the implementation of the driver.
 */
struct ezTargetUartDriver
{
    struct ezDriverCommon               common; /**< Common data and function, MUST BE THE FIRST ELEMENT OF STRUCT */
    struct ezTargetUartApi              api;    /**< Pointer to the API implementation */
    struct ezTargetUartConfiguration    config; /**< Configuration of the target driver */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* None */

#endif /* _EZ_TARGET_UART_DEF_H */

/* End of file */

