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


#define CLI                     0U
#define SCHEDULER               0U
#define SMALLOC                 0U
#define BIN_PARSER              1U
#define HELPER_LINKEDLIST       1U
#define HELPER_HEXDUMP          1U
#define EZM_DEBUG               1U
#define LOGGING                 1U
#define RING_BUFFER             0U
#define HELPER_ASSERT           1U
#define STATEMACHINE            0U
#define IPC                     1U
#define STCMEM                  1U
#define KERNEL                  1U
#define DRIVERINF               1U
#define EZM_PRINTF              1U
#define HAL_UART                0U
#define SYSTEM_ERROR            1U
<<<<<<< Updated upstream
#define FLASH_SIM               1U
#define EVENT_NOTIFIER          1U
#define WIFI_CONTROLLER         1U
=======
#define FLASH_SIM               0U
>>>>>>> Stashed changes

/*Module ID section*/

/* CLI SECTION ****************************************************************/
#if (CLI == 1U)
#if(DRIVERINF == 0U)
    #error DRIVERINF must be acivated to use this module
#endif
#define CLI_MOD_ID                  0x01U
#define NUM_OF_CMD                  10U
#define NUM_OF_ARG                  4U
#endif

/* SCHEDULER SECTION **********************************************************/
#if (SCHEDULER == 1U)
#define SCHEDULER_MOD_ID            0x02U
/**< ID of scheduler module*/

#define NUM_OF_TASK                 5U
/**< number of supported task, maximum is 255 tasks*/

#define TASK_DIAGNOSE               0U
/**< turn on task timing debug out*/
#endif /* SCHEDULER */

/* SMALLOC SECTION ************************************************************/
#if (SMALLOC == 1U)
#if (HELPER_LINKEDLIST == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif
#define SMALLOC_MOD_ID              0x03U
/**< ID of smaloc module*/

#define STATIC_MEMORY_SIZE          512U
/**< number of bytes reserved for the buffer*/

#define ENABLE_STATS                0U
/**< Enable statisic*/

#endif /* SMALLOC */

/* BIN_PARSER SECTION *********************************************************/
#if (BIN_PARSER == 1U)

#if (STCMEM == 0U)
#error module STCMEM must be activated
#endif /* STCMEM */

#define BIN_PARSER_MOD_ID           0x04U
/**< ID of bin cmd parser module*/

#define PAYLOAD_MAX_SIZE            32U
/**< maximum size of the payload, in binary*/

#define CRC_SIZE                    0x02U
/**< size of the crc, in byte*/

#endif /* BIN_PARSER */

/* DEBUG SECTION **************************************************************/
#if (EZM_DEBUG == 1U)
#define DEBUG_MOD_ID                0x05U
#endif /* DEBUG */

/* DEBUG SECTION **************************************************************/
#if (LOGGING == 1U)
#define DEBUG_MOD_ID                0x05U
#define USING_COLOR                 1U
#endif /* DEBUG */

/* HELPER_LINKEDLIST SECTION **************************************************/
#if (HELPER_LINKEDLIST == 1U)
#define HELPER_LINKEDLIST_MOD_ID    0x06U
#endif /* HELPER_LINKEDLIST */


/* HELPER_HEXDUMP SECTION *****************************************************/
#if (HELPER_HEXDUMP == 1U)
#define HELPER_HEXDUMP_MOD_ID       0x07U

#if (EZM_DEBUG == 0U)
#error DEBUG module must be activated
#endif /* DEBUG */

#endif /* HELPER_HEXDUMP */

/* HELPER_HEXDUMP SECTION *****************************************************/
#if (RING_BUFFER == 1U)
#define RING_BUFFER_MOD_ID          0x08U
#endif /* RING_BUFFER */


/* HELPER_ASSERT SECTION ******************************************************/
#if (HELPER_ASSERT == 1U)
#define HELPER_ASSERT_MOD_ID        0x09U
#endif /* HELPER_ASSERT */

/* STATEMACHINE SECTION *******************************************************/
#if (STATEMACHINE == 1U)
#define STATEMACHINE_MOD_ID        0x0AU
#endif /* STATEMACHINE */

/* IPC SECTION ****************************************************************/
#if (IPC == 1U)
#define IPC_MOD_ID                  0x0BU
#define NUM_OF_IPC_INSTANCE         5U

#define USING_MODULE_NAME           1U /* future feature */
/**< turn on/off module name in string*/
#endif /* IPC */

/* STCMEM SECTION *************************************************************/
#if (STCMEM == 1U)
#define STCMEM_MOD_ID               0x0CU
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
#define DRIVERINF_MOD_ID            0x0EU
#define NUM_OF_DRIVERINF            5U

#if (HELPER_LINKEDLIST == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif

#endif /* DRIVERINF */

/* SYSTEM ERROR SECTION *******************************************************/
#if (SYSTEM_ERROR == 1U)
#define SYSTEM_ERROR_MOD_ID         0x0FU
#endif /* SYSTEM_ERROR */

/******************************************************************************/
/* SIMULATOR SECTION                                                          */
/******************************************************************************/

/* FLASH_SIMULATOR ************************************************************/
#if (FLASH_SIM == 1U)
#define FLASH_SIM_MOD_ID            0x10U
#define NUM_OF_FLASH                1U

#ifndef FLASH_LOCATION
#define FLASH_LOCATION              "G:/SDK/ezmSDK/ezmsdk/platforms/simulator/flash/"
#endif
#endif /* FLASH_SIM */

/******************************************************************************/
/* DRIVER SECTION                                                             */
/******************************************************************************/

/* MICROCONTROLER SECTION *****************************************************/
#define NO_CHIP             1U
#define WIN                 2U
#define LINUX               3U
#define ESP32               4U
#define STM32               5U

#define SUPPORTED_CHIP      ESP32

/* UART SECTION ***************************************************************/
#if (HAL_UART == 1U)
#define UART_MOD_ID             0x30U
#if(SUPPORTED_CHIP == WIN)
    #define NUM_OF_SUPPORTED_UART   1U    /**< Number of supported Uart*/
    #define SIM_UART0               0U

    /* Alias name, easy to use */
    #define CLI_UART                SIM_UART0
#else
    #define NUM_OF_SUPPORTED_UART   1U    /**< Number of supported Uart*/
    #define HAL_UART0               0U

    /* Alias name, easy to use */
    #define CLI_UART                HAL_UART0
#endif /*SUPPORTED_CHIP*/

#endif /* HAL_UART */

/* WIFI_CONTROLLER SECTION ****************************************************/
#if WIFI_CONTROLLER == 1U

#endif /* WIFI_CONTROLLER */

#endif /* _APP_CONFIG_H */
