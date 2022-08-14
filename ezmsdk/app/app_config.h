/*******************************************************************************
* Title                 :   app_config
* Filename              :   app_config.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   15.01.2022
* Version               :   1.0.0
*
*******************************************************************************/

/** @file   app_config.h
 *  @brief  Header for complete application configuration
 */

#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H

#include "configuration/sdkconfig.h"

#define SCHEDULER               0U
#define SMALLOC                 0U
#define BIN_PARSER              1U
#define HELPER_LINKEDLIST       1U
#define HELPER_HEXDUMP          1U
#define LOGGING                 1U
#define RING_BUFFER             0U
#define HELPER_ASSERT           1U
#define STATEMACHINE            0U
#define IPC                     1U
#define STCMEM                  1U
#define KERNEL                  1U
#define DRIVERINF               1U
#define EZM_PRINTF              0U
#define HAL_UART                1U
#define SYSTEM_ERROR            1U
#define FLASH_SIM               0U
#define EVENT_NOTIFIER          1U
#define WIFI_CONTROLLER         1U
#define DATA_MODEL              1U
#define MQTT                    0U
#define VIRTUAL_COM             1U

/* MICROCONTROLER SECTION *****************************************************/
#define NO_CHIP             1U
#define WIN                 2U
#define LINUX               3U
#define ESP32               4U
#define STM32               5U

#define SUPPORTED_CHIP      WIN

/*Module ID section*/


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

#if (BIN_PARSER == 1U)
    BIN_PARSER_MOD_ID,
#endif

#if (LOGGING == 1U)
    DEBUG_MOD_ID,
#endif /* LOGGING */

#if (HELPER_LINKEDLIST == 1U)
    HELPER_LINKEDLIST_MOD_ID,
#endif /* HELPER_LINKEDLIST */

#if (HELPER_HEXDUMP == 1U)
    HELPER_HEXDUMP_MOD_ID,
#endif /* HELPER_HEXDUMP */

#if (RING_BUFFER == 1U)
    RING_BUFFER_MOD_ID,
#endif /* RING_BUFFER */

#if (HELPER_ASSERT == 1U)
    HELPER_ASSERT_MOD_ID,
#endif

#if (STATEMACHINE == 1U)
    STATEMACHINE_MOD_ID,
#endif /* STATEMACHINE */

#if (IPC == 1U)
    IPC_MOD_ID,
#endif /* IPC */

#if (STCMEM == 1U)
    STCMEM_MOD_ID,
#endif /* STCMEM */

#if (KERNEL == 1U)
    KERNEL_MOD_ID,
#endif /* KERNEL */

#if (DRIVERINF)
    DRIVERINF_MOD_ID,
#endif /* DRIVERINF */

#if (SYSTEM_ERROR == 1U)
    SYSTEM_ERROR_MOD_ID,
#endif /* SYSTEM_ERROR */

#if (FLASH_SIM == 1U)
    FLASH_SIM_MOD_ID,
#endif /* FLASH_SIM */

#if (HAL_UART == 1U)
    UART_MOD_ID,
#endif

#if (WIFI_CONTROLLER == 1U)
    WIFI_CONTROLLER_ID,
#endif /* WIFI_CONTROLLER */

#if (VIRTUAL_COM == 1U)
    VIRTUAL_COM_MOD_ID,
#endif /* VIRTUAL_COM */

    /* Application module */

}MODULE_ID;


/* SCHEDULER SECTION **********************************************************/
#if (SCHEDULER == 1U)

#define NUM_OF_TASK                 5U  /**< number of supported task, maximum is 255 tasks*/
#define TASK_DIAGNOSE               0U  /**< turn on task timing debug out*/

#endif /* SCHEDULER */

/* SMALLOC SECTION ************************************************************/
#if (SMALLOC == 1U)
#if (HELPER_LINKEDLIST == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif

#define STATIC_MEMORY_SIZE          512U    /**< number of bytes reserved for the buffer*/
#define ENABLE_STATS                0U      /**< Enable statisic*/

#endif /* SMALLOC */

/* BIN_PARSER SECTION *********************************************************/
#if (BIN_PARSER == 1U)

#if (STCMEM == 0U)
#error module STCMEM must be activated
#endif /* STCMEM */

#define PAYLOAD_MAX_SIZE            32U     /**< maximum size of the payload, in binary*/
#define CRC_SIZE                    0x02U   /**< size of the crc, in byte*/

#endif /* BIN_PARSER */

/* DEBUG SECTION **************************************************************/
#if (LOGGING == 1U)
#define DEBUG_MOD_ID                0x05U
#define USING_COLOR                 1U
#endif /* DEBUG */


/* HELPER_HEXDUMP SECTION *****************************************************/
#if (HELPER_HEXDUMP == 1U)

#if (LOGGING == 0U)
#error DEBUG module must be activated
#endif /* DEBUG */

#endif /* HELPER_HEXDUMP */

/* IPC SECTION ****************************************************************/
#if (IPC == 1U)
#define NUM_OF_IPC_INSTANCE         5U  /**< */
#define USING_MODULE_NAME           1U  /**< turn on/off module name in string*/
#endif /* IPC */

/* STCMEM SECTION *************************************************************/
#if (STCMEM == 1U)
#define NUM_OF_MEM_BLOCK            100U

#if (HELPER_LINKEDLIST == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif

#endif /*STCMEM*/

/* KERNEL SECTION *************************************************************/
#if (KERNEL == 1U)
#define KERNEL_MOD_ID               0x0DU
#define NUM_OF_PROC                 5U

#if (HELPER_LINKEDLIST == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif
#endif /* KERNEL */

/* EZM_PRINTF SECTION *********************************************************/
#if (EZM_PRINTF == 1U)
#define PRINTF_BUFF_SIZE        256U
#endif

/* DRIVERINF SECTION***********************************************************/
#if (DRIVERINF == 1U)
#define NUM_OF_DRIVERINF            5U

#if (HELPER_LINKEDLIST == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif

#endif /* DRIVERINF */


/******************************************************************************/
/* SIMULATOR SECTION                                                          */
/******************************************************************************/

/* FLASH_SIMULATOR ************************************************************/
#if (FLASH_SIM == 1U)
#define NUM_OF_FLASH            1U

#ifndef FLASH_LOCATION
#define FLASH_LOCATION          "G:/SDK/ezmSDK/ezmsdk/platforms/simulator/flash/"
#endif
#endif /* FLASH_SIM */

/******************************************************************************/
/* DRIVER SECTION                                                             */
/******************************************************************************/

/* UART SECTION ***************************************************************/
#if (HAL_UART == 1U)

#if(SUPPORTED_CHIP == WIN)
    #if(VIRTUAL_COM == 1)
        #define NUM_OF_SUPPORTED_UART   2U    /**< Number of supported Uart*/
        #define SIM_UART0               0U
        #define VCP_UART                1U

        /* Alias name, easy to use */
        #define CLI_UART                SIM_UART0
    #else
        #define NUM_OF_SUPPORTED_UART   1U    /**< Number of supported Uart*/
        #define SIM_UART0               0U

        /* Alias name, easy to use */
        #define CLI_UART                SIM_UART0
    #endif

#else
    #define NUM_OF_SUPPORTED_UART   1U    /**< Number of supported Uart*/
    #define HAL_UART0               0U

    /* Alias name, easy to use */
    #define CLI_UART                HAL_UART0
#endif /*SUPPORTED_CHIP*/

#endif /* HAL_UART */

/* WIFI_CONTROLLER SECTION ****************************************************/
#if WIFI_CONTROLLER == 1U
#define WIFI_CONTROLLER_ID      0x31U

#endif /* WIFI_CONTROLLER */



#endif /* _APP_CONFIG_H */
