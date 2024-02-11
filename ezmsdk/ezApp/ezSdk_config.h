
/*******************************************************************************
* Filename:         ezSdk.h
* Author:           Hai Nguyen
* Original Date:    21.10.2022
* Last Update:      21.10.2022
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
* Written by Hai Nguyen 21.10.2022
*
*******************************************************************************/

/** @file   ezSdk.h
 *  @author Hai Nguyen
 *  @date   21.10.2022
 *  @brief  Header file of the ezSdk module
 *  
 *  @details This module is responsible for initializing the Embedded easy sdk
 * 
 */

#ifndef _EZSDK_CONFIG_H
#define _EZSDK_CONFIG_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezConfig/sdkconfig.h"
#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define SCHEDULER               0U  /**< Legacy, for compatible only */
#define SMALLOC                 0U  /**< Legacy, for compatible only */
#define EZM_PRINTF              0U  /**< Legacy, for compatible only */
#define WIFI_CONTROLLER         0U  /**< Legacy, for compatible only */
#define MQTT                    0U  /**< Legacy, for compatible only */

/* SCHEDULER SECTION **********************************************************/
#if (SCHEDULER == 1U)

#define NUM_OF_TASK                 5U  /**< number of supported task, maximum is 255 tasks*/
#define TASK_DIAGNOSE               0U  /**< turn on task timing debug out*/

#endif /* SCHEDULER */

/* SMALLOC SECTION ************************************************************/
#if (SMALLOC == 1U)
#if (CONFIG_HELPER_LINKEDLIST == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif

#define STATIC_MEMORY_SIZE          512U    /**< number of bytes reserved for the buffer*/
#define ENABLE_STATS                0U      /**< Enable statisic*/

#endif /* SMALLOC */


/* EZM_PRINTF SECTION *********************************************************/
#if (EZM_PRINTF == 1U)
#define PRINTF_BUFF_SIZE        256U
#endif

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/* @brief return status of the Easy Embedded SDK API
 *
 */
typedef enum
{
    ezFAIL,     /**< operation fail */
    ezSUCCESS,  /**< operation success */
}ezSTATUS;


/** @brief definition of status of the executed task
 *
 */
typedef enum
{
    TASK_STATUS_OK,         /**< status OK, task will be removed from list */
    TASK_STATUS_EXEC_AGAIN, /**< task will be executed again with the same interval */
    TASK_STATUS_ERROR,      /**< task error, will be moved from list */
}KERNEL_TASK_STATUS;


/* @brief 
 *
 */
typedef enum
{
    /* Application module */
#if (CONFIG_CLI == 1U)
    CLI_MOD_ID,
#endif /* CLI */

#if (SCHEDULER == 1U)
    SCHEDULER_MOD_ID,
#endif /* SCHEDULER*/

#if (SMALLOC == 1U)
    SMALLOC_MOD_ID,
#endif /* SMALLOC */

#if (CONFIG_BIN_PARSER == 1U)
    BIN_PARSER_MOD_ID,
#endif /* CONFIG_BIN_PARSER */

#if (CONFIG_LOGGING == 1U)
    DEBUG_MOD_ID,
#endif /* CONFIG_LOGGING */

#if (CONFIG_HELPER_LINKEDLIST == 1U)
    HELPER_LINKEDLIST_MOD_ID,
#endif /* CONFIG_HELPER_LINKEDLIST */

#if (CONFIG_HELPER_HEXDUMP == 1U)
    HELPER_HEXDUMP_MOD_ID,
#endif /* CONFIG_HELPER_HEXDUMP */

#if (CONFIG_RING_BUFFER == 1U)
    RING_BUFFER_MOD_ID,
#endif /* CONFIG_RING_BUFFER */

#if (CONFIG_HELPER_ASSERT == 1U)
    HELPER_ASSERT_MOD_ID,
#endif

#if (CONFIG_STATEMACHINE == 1U)
    STATEMACHINE_MOD_ID,
#endif /* CONFIG_STATEMACHINE */

#if (CONFIG_IPC == 1U)
    IPC_MOD_ID,
#endif /* CONFIG_IPC */

#if (CONFIG_STCMEM == 1U)
    STCMEM_MOD_ID,
#endif /* CONFIG_STCMEM */

#if (CONFIG_KERNEL == 1U)
    KERNEL_MOD_ID,
#endif /* KERNEL */

#if (CONFIG_DRIVERINF == 1U)
    DRIVERINF_MOD_ID,
#endif /* CONFIG_DRIVERINF */

#if (CONFIG_SYSTEM_ERROR == 1U)
    SYSTEM_ERROR_MOD_ID,
#endif /* SYSTEM_ERROR */

#if (CONFIG_FLASH_SIM == 1U)
    FLASH_SIM_MOD_ID,
#endif /* CONFIG_FLASH_SIM */

#if (CONFIG_HAL_UART == 1U)
    UART_MOD_ID,
#endif /* CONFIG_HAL_UART */

#if (CONFIG_HAL_I2C == 1U)
    I2C_HAL_MOD_ID,
#endif /* CONFIG_HAL_I2C */

#if (WIFI_CONTROLLER == 1U)
    WIFI_CONTROLLER_ID,
#endif /* WIFI_CONTROLLER */

#if (CONFIG_VIRTUAL_COM == 1U)
    VIRTUAL_COM_MOD_ID,
#endif /* CONFIG_VIRTUAL_COM */

#if ( CONFIG_EMBEDDED_EMULATOR == 1U)
    EMBEDDED_EMULATOR_ID,
#endif /* CONFIG_EMBEDDED_EMULATOR */
    /* Application module */
    NUM_OF_MODULE,
}MODULE_ID;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */


/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* None */


#endif /* #define _EZSDK_CONFIG_H */

/* End of file */

