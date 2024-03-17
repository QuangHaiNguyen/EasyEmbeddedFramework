/*****************************************************************************
* Filename:         ez_uart.h
* Author:           Hai Nguyen
* Original Date:    15.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_uart.h
 *  @author Hai Nguyen
 *  @date   15.03.2024
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 */

#ifndef _EZ_UART_H
#define _EZ_UART_H

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_UART_ENABLE == 1)
#include <stdint.h>
#include <stdbool.h>

#include "ez_driver_def.h"


/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None */


/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/** @brief Parity value
 */
typedef enum
{
    PARITY_NONE,    /**< None */
    PARITY_ODD,     /**< Odd */
    PARITY_EVEN,    /**< Even */
    PARITY_MARK,    /**< Mark */
    PARITY_SPACE,   /**< Space */
}EZ_UART_PARITY_ENUM;


/** @brief Number of stop bit
 */
typedef enum
{
    ONE_BIT,            /**< 1 stop bit */
    ONE_AND_HALF_BIT,   /**< 1.5 stop bit */
    TWO_BITS,           /**< 2 stop bits */
}EZ_UART_NUM_STOP_BIT_ENUM;


/** @brief Number of stop bit
 */
struct ezUartConfiguration
{
    const char                  *port_name; /**< Name of the serial port */
    uint32_t                    baudrate;   /**< Baudrate*/
    EZ_UART_PARITY_ENUM         parity;     /**< Parity */
    EZ_UART_NUM_STOP_BIT_ENUM   stop_bit;   /**< Number of stop bit */
    uint8_t                     byte_size;  /**< Size of data in bit */
};


/** @brief Define Uart Driver Instance
 */
typedef struct ezDrvInstance ezUartDrvInstance_t;


/** @brief
 */
typedef EZ_DRV_STATUS(*ezHwUart_Initialize)(uint8_t index);


/** @brief
 */
typedef EZ_DRV_STATUS(*ezHwUart_Deinitialize)(uint8_t index);


/** @brief
 */
typedef EZ_DRV_STATUS(*ezHwUart_AsyncTransmit)(uint8_t index, uint8_t *tx_buff, uint16_t buff_size);


/** @brief
 */
typedef EZ_DRV_STATUS(*ezHwUart_AsyncReceive)(uint8_t index, uint8_t *rx_buff, uint16_t buff_size);


/** @brief
 */
typedef EZ_DRV_STATUS(*ezHwUart_SyncTransmit)(uint8_t index, uint8_t *tx_buff, uint16_t buff_size, uint32_t timeout_millis);


/** @brief
 */
typedef EZ_DRV_STATUS(*ezHwUart_SyncReceive)(uint8_t index, uint8_t *rx_buff, uint16_t buff_size, uint32_t timeout_millis);


/** @brief
 */
typedef EZ_DRV_STATUS(*ezHwUart_GetConfig)(uint8_t index, struct ezUartConfiguration *config);


/** @brief
 */
typedef EZ_DRV_STATUS(*ezHwUart_UpdateConfig)(uint8_t index);


/** @brief Number of stop bit
 */
struct ezHwUartInterface
{
    uint8_t                 index;          /**< Index of the driver instance */
    ezHwUart_Initialize     initialize;
    ezHwUart_Deinitialize   deinitialize;
    ezHwUart_AsyncTransmit  async_transmit; /**< */
    ezHwUart_AsyncReceive   async_receive;  /**< */
    ezHwUart_SyncTransmit   sync_transmit;  /**< */
    ezHwUart_SyncReceive    sync_receive;   /**< */
    ezHwUart_GetConfig      get_conf;       /**< */
    ezHwUart_UpdateConfig   update_conf;    /**< */
};


/** @brief Number of stop bit
 */
struct ezUartDriver
{
    struct Node                 ll_node;    /* linked list node to link to list of hw driver implmentation */
    struct ezDriverCommon       common;     /* Common data of driver */
    struct ezUartConfiguration  config;     /* Uart related configuration */
    struct ezHwUartInterface    interface;  /* HW API */
};


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/
EZ_DRV_STATUS ezUart_SystemRegisterHwDriver(struct ezUartDriver *hw_uart_driver);
EZ_DRV_STATUS ezUart_SystemUnregisterHwDriver(struct ezUartDriver *hw_uart_driver);

EZ_DRV_STATUS ezUart_RegisterInstance(ezUartDrvInstance_t *inst, const char *driver_name);
EZ_DRV_STATUS ezUart_UnregisterInstance(ezUartDrvInstance_t *inst);
EZ_DRV_STATUS ezUart_Initialize(ezUartDrvInstance_t *inst);
EZ_DRV_STATUS ezUart_Deinitialize(ezUartDrvInstance_t *inst);
EZ_DRV_STATUS ezUart_AsyncTransmit(ezUartDrvInstance_t *inst, uint8_t tx_buff, uint16_t buff_size);
EZ_DRV_STATUS ezUart_AsyncReceive(ezUartDrvInstance_t *inst, uint8_t rx_buff, uint16_t buff_size);
EZ_DRV_STATUS ezUart_SyncTransmit(ezUartDrvInstance_t *inst, uint8_t *tx_buff, uint16_t buff_size, uint32_t timeout_millis);
EZ_DRV_STATUS ezUart_SyncReceive(ezUartDrvInstance_t *inst, uint8_t *tx_buff, uint16_t buff_size, uint32_t timeout_millis);
EZ_DRV_STATUS ezUart_GetConfig(ezUartDrvInstance_t *inst, struct ezUartConfiguration *config);
EZ_DRV_STATUS ezUart_UpdateConfig(ezUartDrvInstance_t *inst);

#endif /* EZ_UART_ENABLE == 1 */
#endif /* _EZ_UART_H */


/* End of file */
