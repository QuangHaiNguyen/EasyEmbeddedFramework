
/*******************************************************************************
* Filename:         ezSdk.c
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

/** @file   ezSdk.c
 *  @author Hai Nguyen
 *  @date   21.10.2022
 *  @brief  This is the source file of the SDK initialization
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezSdk.h"

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ezSdk"       /**< module name */
#include "ezUtilities/logging/logging.h"

#include "ezApp/ezSdk_config.h"
#include "unity_test_platform/unity.h"

#if (CONFIG_WIN == 1U)
#include <time.h>
#endif

#if (CONFIG_CLI == 1U)
#include "ezApp/cli/cli.h"
#endif

#if (SCHEDULER == 1U)
#include "scheduler/scheduler.h"
#endif /* SCHEDULER */

#if (SMALLOC == 1U)
#include "utilities/smalloc/smalloc.h"
#endif /* SMALLOC */

#if (CONFIG_BIN_PARSER == 1U)
#include "binCmdParser/binCmdParser.h"
#endif /* CONFIG_BIN_PARSER */

#if (CONFIG_HELPER_LINKEDLIST == 1U)
#include "ezUtilities/linked_list/linked_list.h"
#endif /* CONFIG_HELPER_LINKEDLIST */

#if (CONFIG_HELPER_HEXDUMP == 1U)
#include "ezUtilities/hexdump/hexdump.h"
#endif /* CONFIG_HELPER_HEXDUMP */

#if (CONFIG_RING_BUFFER == 1U)
#include "ezUtilities/ring_buffer/ring_buffer.h"
#endif /* CONFIG_RING_BUFFER */

#if (CONFIG_HELPER_ASSERT == 1U)
#include "ezUtilities/ezmAssert/ezmAssert.h"
#endif /* CONFIG_HELPER_ASSERT */

#if (CONFIG_STATEMACHINE == 1U)
#include "ezApp/statemachine/statemachine.h"
#endif /* CONFIG_STATEMACHINE */

#if (CONFIG_IPC == 1U)
#include "ezApp/ezmIpc/ezmIpc.h"
#endif /* CONFIG_IPC */

#if (CONFIG_STCMEM==1U)
#include "ezUtilities/stcmem/stcmem.h"
#endif /* CONFIG_STCMEM */

#if(CONFIG_KERNEL == 1U)
#include "ezApp/ezmKernel/ezmKernel.h"
#include "ezApp/ezKernel/ezKernel.h"
#endif /* CONFIG_KERNEL */

#if (CONFIG_SYSTEM_ERROR == 1U)
#include "ezUtilities/system_error/system_error.h"
#endif /* CONFIG_SYSTEM_ERROR */

#if (CONFIG_HAL_UART)
#include "ezHal/uart/uart.h"
#endif /* CONFIG_HAL_UART */

#if (CONFIG_HAL_I2C == 1U)
#include "ezHal/hal_i2c/hal_i2c.h"
#endif /* CONFIG_HAL_I2C */

#if (CONFIG_DRIVERINF == 1U)
#include "ezApp/ezDriver/ezDriver.h"
#endif /* CONFIG_DRIVERINF */

#if (CONFIG_FLASH_SIM == 1U)
#include "platforms/simulator/flash/flash_simulator.h"
#endif /* CONFIG_FLASH_SIM */

#if (DATA_MODEL == 1U)
#include "ezApp/data_model/data_model.h"
#endif /* CONFIG_DATA_MODEL == 1U */

#if (CONFIG_MQTT == 1U)
#include "hal/network/mqtt/mqtt.h"
#endif /* CONFIG_MQTT */


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
* Function Definitions
*******************************************************************************/
static void     ezSdk_PrintActiveModule(void);
static void     ezSdk_PrintHeader(void);

/******************************************************************************
* External functions
*******************************************************************************/


void ezSdk_Initialization(void)
{
    ezSdk_PrintHeader();
#if (DEBUG_LVL >= LVL_INFO)
    ezSdk_PrintActiveModule();
#endif

#if (SCHEDULER == 1U)
    /*must call init function here, but there is problem with the init so check it later*/
    INFO("Initialize scheduler");
    INFO("Module Id: 0x%02x", SCHEDULER_MOD_ID);
    INFO("Number of available tasks: %d", NUM_OF_TASK);
#endif

#if (SMALLOC == 1U)
    /* SMALLOC module has no init function*/
    INFO("Initialize smalloc");
    INFO("Module Id: 0x%02x", SMALLOC_MOD_ID);
    INFO("Availalbe memory: %d bytes", STATIC_MEMORY_SIZE);
#endif

#if (CONFIG_BIN_PARSER == 1U)
    /* BIN_PARSER module has no init function*/
    INFO("Initialize binary command parser module");
    INFO("Module Id: 0x%02x", BIN_PARSER_MOD_ID);
    INFO("payload size: %d bytes", CONFIG_PAYLOAD_MAX_SIZE);
    INFO("CRC size: %d bytes", CONFIG_CRC_SIZE);
#endif /* CONFIG_BIN_PARSER */

#if (CONFIG_HELPER_LINKEDLIST == 1U)
    INFO("Initialize linked list module");
    INFO("Module Id: 0x%02x", HELPER_LINKEDLIST_MOD_ID);
    ezmLL_Initialization();
#endif /* CONFIG_HELPER_LINKEDLIST */

#if (CONFIG_HELPER_HEXDUMP == 1U)
    /* DEBUG module has no init function*/
    INFO("Initialize hexdump module");
    INFO("Module Id: 0x%02x", HELPER_HEXDUMP_MOD_ID);
#endif /* CONFIG_HELPER_HEXDUMP */

#if (CONFIG_RING_BUFFER == 1U)
    /* DEBUG module has no init function*/
    INFO("Initialize ring buffer module");
    INFO("Module Id: 0x%02x", RING_BUFFER_MOD_ID);
#endif /* CONFIG_RING_BUFFER */

#if (CONFIG_HELPER_ASSERT == 1U)
    /* DEBUG module has no init function*/
    INFO("Initialize assert module");
    INFO("Module Id: 0x%02x", HELPER_ASSERT_MOD_ID);
#endif /* CONFIG_HELPER_ASSERT */

#if (CONFIG_STATEMACHINE == 1U)
    /* DEBUG module has no init function*/
    INFO("Initialize state machine module");
    INFO("Module Id: 0x%02x", STATEMACHINE_MOD_ID);
#endif /* CONFIG_STATEMACHINE */

#if (CONFIG_IPC == 1U)
    ezmIpc_InitModule();
    INFO("Initialize IPC module");
    INFO("Module Id: 0x%02x", IPC_MOD_ID);
#endif /* CONFIG_IPC */

#if (CONFIG_STCMEM==1U)
    ezmStcMem_Initialization();
    ezmStcMem_Initialization();
    INFO("Initialize STCMEM module");
    INFO("Module Id: 0x%02x", STCMEM_MOD_ID);
#endif /* CONFIG_STCMEM */

#if(CONFIG_DRIVERINF == 1U)
    ezDriver_Initialize();
    INFO("Initialize DRIVERINF module");
    INFO("Module Id: 0x%02x", DRIVERINF_MOD_ID);

#endif /* CONFIG_DRIVERINF */

#if (CONFIG_SYSTEM_ERROR == 1U)
    SystemError_Initialize();
    INFO("Initialize SYSTEM_ERROR module");
    INFO("Module Id: 0x%02x", SYSTEM_ERROR_MOD_ID);
#endif /* CONFIG_SYSTEM_ERROR*/

#if (DATA_MODEL == 1U)
    DataModel_Initialization();
    INFO("Initialize DATA_MODEL module");
#endif /* CONFIG_DATA_MODEL == 1U */

#if (CONFIG_FLASH_SIM == 1U)
    if (FlashSim_Initialization())
    {
        INFO("Initialize FLASH_SIM module");
        INFO("Module Id: 0x%02x", FLASH_SIM_MOD_ID);
    }
    else
    {
        INFO("FLASH_SIM module ERROR");
    }
#endif /* CONFIG_FLASH_SIM*/

#if (CONFIG_HAL_UART)
    INFO("Initialize UART Driver");
    INFO("Module Id: 0x%02x", UART_MOD_ID);
#endif /* CONFIG_HAL_UART */

#if (CONFIG_HAL_I2C == 1U)
    INFO("Initialize I2C HAL Driver");
    INFO("Module Id: 0x%02x", I2C_HAL_MOD_ID);
#endif /* CONFIG_HAL_I2C */

#if (CONFIG_LOGGING == 1U)
    INFO("Initialize Logging module");
    Logging_DemoFeatures();
#endif /* CONFIG_LOGGING */

#if (CONFIG_KERNEL == 1U)
    ezKernel_Initialization();
#endif

#if (CONFIG_EMBEDDED_EMULATOR == 1U)
    INFO("Initialize embedded emulator application");
    INFO("Module Id: 0x%02x", EMBEDDED_EMULATOR_ID);

    if (ezMbedEmulator_Initialization() == ezSUCCESS)
    {
        INFO("Initialize success");
    }
    else
    {
        INFO("Initialize fail");
    }
#endif

#if (CONFIG_CLI == 1U && CONFIG_HAL_UART == 1U)
    ezDriveHandle cli_uart = NULL;
    cli_uart = ezDriver_GetDriver("cli_uart");

    if (cli_uart == NULL)
    {
        INFO("ERROR: Initialize command line interface");
    }
    else
    {
        INFO("Initialize command line interface");
        INFO("Module Id: 0x%02x", CLI_MOD_ID);
        INFO("Number of supported command: [command = %d]", CONFIG_NUM_OF_CMD);
        ezmCli_Init((UartDrvApi*)cli_uart->ll_interface);
    }
#endif

}

/******************************************************************************
* Internal functions
*******************************************************************************/

/******************************************************************************
* Function : ezSdk_PrintActiveModule
*//**
* \b Description:
*
* Show the activated modules
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   None
*
*******************************************************************************/
static void ezSdk_PrintActiveModule(void)
{
    INFO("ACTIVE MODULES:");

#if (CONFIG_CLI == 1U)
    INFO("[x] CLI");
#else
    INFO("[ ] CLI");
#endif

#if (SCHEDULER == 1U)
    INFO("[x] SCHEDULER");
#else
    INFO("[ ] SCHEDULER");
#endif

#if (SMALLOC == 1U)
    INFO("[x] SMALLOC");
#else
    INFO("[ ] SMALLOC");
#endif

#if (CONFIG_BIN_PARSER == 1U)
    INFO("[x] BIN_PARSER");
#else
    INFO("[ ] BIN_PARSER");
#endif /* CONFIG_BIN_PARSER */

#if (EZM_DEBUG == 1U)
    INFO("[x] DEBUG");
#else
    INFO("[ ] DEBUG");
#endif

#if (CONFIG_LOGGING == 1U)
    INFO("[x] LOGGING");
#else
    INFO("[ ] LOGGING");
#endif

#if (CONFIG_HELPER_LINKEDLIST == 1U)
    INFO("[x] HELPER_LINKEDLIST");
#else
    INFO("[ ] HELPER_LINKEDLIST");
#endif /* CONFIG_HELPER_LINKEDLIST */

#if (CONFIG_HELPER_HEXDUMP == 1U)
    INFO("[x] HELPER_HEXDUMP");
#else
    INFO("[ ] HELPER_HEXDUMP");
#endif

#if (CONFIG_RING_BUFFER == 1U)
    INFO("[x] RING_BUFFER");
#else
    INFO("[ ] RING_BUFFER");
#endif /* CONFIG_RING_BUFFER */

#if (CONFIG_HELPER_ASSERT == 1U)
    INFO("[x] HELPER_ASSERT");
#else
    INFO("[ ] HELPER_ASSERT");
#endif

#if (CONFIG_STATEMACHINE == 1U)
    INFO("[x] STATEMACHINE");
#else
    INFO("[ ] STATEMACHINE");
#endif /* CONFIG_STATEMACHINE */

#if (CONFIG_IPC == 1U)
    INFO("[x] IPC");
#else
    INFO("[ ] IPC");
#endif /* CONFIG_IPC */

#if (CONFIG_STCMEM==1U)
    INFO("[x] STCMEM");
#else
    INFO("[ ] STCMEM");
#endif /* CONFIG_STCMEM */

#if(CONFIG_DRIVERINF == 1U)
    INFO("[x] DRIVERINF");
#else
    INFO("[ ] DRIVERINF");
#endif

#if (CONFIG_SYSTEM_ERROR == 1U)
    INFO("[x] SYSTEM_ERROR");
#else
    INFO("[ ] SYSTEM_ERROR");
#endif /* CONFIG_SYSTEM_ERROR */

#if (DATA_MODEL == 1U)
    INFO("[x] DATA_MODEL");
#else
    INFO("[ ] DATA_MODEL");
#endif /* CONFIG_DATA_MODEL */

#if (CONFIG_FLASH_SIM == 1U)
    INFO("[x] FLASH_SIM");
#else
    INFO("[ ] FLASH_SIM");
#endif /* CONFIG_FLASH_SIM */

#if (CONFIG_HAL_UART == 1U)
    INFO("[x] UART");
#else
    INFO("[ ] UART");
#endif /* CONFIG_HAL_UART */

#if (CONFIG_HAL_I2C == 1U)
    INFO("[x] HAL I2C");
#else
    INFO("[ ] HAL I2C");
#endif /* CONFIG_HAL_I2C == 1U */

#if (CONFIG_VIRTUAL_COM)
    INFO("[x] VIRTUAL_COM");
#else
    INFO("[ ] VIRTUAL_COM");
#endif /* CONFIG_VIRTUAL_COM */

    INFO("******************************************************************************\n\n");
}


/******************************************************************************
* Function : ezSdk_PrintHeader
*//**
* \b Description:
*
* Show fancy header whenever the sdk is started
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   None
*
*******************************************************************************/
static void ezSdk_PrintHeader(void)
{
    INFO("******************************************************************************");
    INFO("* EASY EMBEDDED SOFTWARE DEVELOPMENT KIT");
    INFO("* Author: Quang Hai Nguyen");
    INFO("* SDK Version: %s", CONFIG_SDK_VERSION_NUMBER);
    INFO("* App Version: %s", CONFIG_APP_VERSION_NUMBER);
    INFO("* Build date: 16.01.2022");
#if (CONFIG_WIN == 1U)
    INFO("* Platform: Windows");
#endif

#if 0
#if(SUPPORTED_CHIP == ESP32)
    INFO("* Platform: ESP32");
#elif (SUPPORTED_CHIP == STM32)
    INFO("* Platform: STM32");
#else
    INFO("* Platform: PC");
#endif
#endif
    INFO("* Contact: hainguyen.ezm@gmail.com");
    INFO("******************************************************************************\n\n");
}


/* End of file*/

