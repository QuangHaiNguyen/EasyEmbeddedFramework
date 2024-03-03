
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
#include "utilities/logging/ez_logging.h"

//#include "ezApp/ezSdk_config.h"

#if ((WINDOWS_TARGET + LINUX_TARGET + EMBEDDED_TARGET) > 1)
#error More than one target is activated
#endif


/* Utilities section ********************************************************/

#if (EZ_LINKEDLIST == 1)
#include "ez_linked_list.h"
#endif /* EZ_LINKEDLIST == 1 */

#if (EZ_HEXDUMP == 1)
#include "ez_hexdump.h"
#endif /* EZ_HEXDUMP == 1 */

#if (EZ_RING_BUFFER == 1)
#include "ez_ring_buffer.h"
#endif /* EZ_RING_BUFFER == 1 */

#if (EZ_STATIC_ALLOC == 1)
#include "ez_static_alloc.h"
#endif /* EZ_STATIC_ALLOC == 1 */

#if (EZ_ASSERT == 1)
#include "ez_assert.h"
#endif /* EZ_ASSERT == 1 */

#if (EZ_SYS_ERROR == 1)
#include "ez_system_error.h"
#endif /* EZ_SYS_ERROR == 1 */

#if (EZ_QUEUE == 1)
#include "ez_queue.h"
#endif /* EZ_QUEUE == 1 */

/* Service section **********************************************************/

#if(EZ_KERNEL == 1)
#include "service/kernel/ez_kernel.h"
#endif /*EZ_KERNEL == 1*/

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


#if (CONFIG_STATEMACHINE == 1U)
#include "ezApp/statemachine/statemachine.h"
#endif /* CONFIG_STATEMACHINE */

#if (CONFIG_IPC == 1U)
#include "ezApp/ezmIpc/ezmIpc.h"
#endif /* CONFIG_IPC */


#if(CONFIG_KERNEL == 1U)
#include "ezApp/ezmKernel/ezmKernel.h"
#include "ezApp/ezKernel/ezKernel.h"
#endif /* CONFIG_KERNEL */

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

/* Application section *******************************************************/

#if (DATA_MODEL == 1U)
#include "ez_data_model.h"
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
static void     ezSdk_PrintHeader(void);


/******************************************************************************
* External functions
*******************************************************************************/

void ezSdk_Initialization(void)
{
    ezSdk_PrintHeader();

    /* Utilities Module ******************************************************/
#if (EZ_LOGGING == 1)
    Logging_DemoFeatures();
#endif /* EZ_LOGGING == 1 */

#if (EZ_LINKEDLIST == 1)
    EZINFO("Initialized EZ_LINKEDLIST module");
    //INFO("Module Id: 0x%02x", HELPER_LINKEDLIST_MOD_ID);
    ezmLL_Initialization();
#endif /* EZ_LINKEDLIST == 1 */

#if (EZ_HEXDUMP == 1)
    /* DEBUG module has no init function*/
    EZINFO("Initialized EZ_HEXDUMP module");
    //INFO("Module Id: 0x%02x", HELPER_HEXDUMP_MOD_ID);
#endif /* EZ_HEXDUMP == 1 */

#if (EZ_RING_BUFFER == 1)
    /* DEBUG module has no init function*/
    EZINFO("Initialized EZ_RING_BUFFER module");
    //INFO("Module Id: 0x%02x", RING_BUFFER_MOD_ID);
#endif /* EZ_RING_BUFFER == 1 */

#if (EZ_ASSERT == 1)
    /* DEBUG module has no init function*/
    EZINFO("Initialized EZ_ASSERT module");
    //INFO("Module Id: 0x%02x", HELPER_ASSERT_MOD_ID);
#endif /* EZ_ASSERT == 1 */

#if (EZ_SYS_ERROR == 1)
    SystemError_Initialize();
    EZINFO("Initialized EZ_SYS_ERROR module");
    //INFO("Module Id: 0x%02x", SYSTEM_ERROR_MOD_ID);
#endif /* EZ_SYS_ERROR == 1 */

#if (EZ_QUEUE == 1)
    EZINFO("Initialized EZ_QUEUE module");
#endif /* EZ_QUEUE == 1 */

    /* Service modules *******************************************************/
#if (EZ_EVENT_NOTIFIER == 1)
    EZINFO("Initialized EZ_EVENT_NOTIFIER service");
#endif /* EZ_EVENT_NOTIFIER == 1*/

#if(EZ_KERNEL == 1)
    ezKernel_Initialization();
    EZINFO("Initialized EZ_KERNEL service");
#endif /*EZ_KERNEL == 1*/

    /* Framework application module ******************************************/
#if (DATA_MODEL == 1U)
    DataModel_Initialization();
    EZINFO("Initialized DATA_MODEL application");
#endif /* CONFIG_DATA_MODEL == 1U */


#if (SCHEDULER == 1U)
    /*must call init function here, but there is problem with the init so check it later*/
    EZINFO("Initialize scheduler");
    EZINFO("Module Id: 0x%02x", SCHEDULER_MOD_ID);
    EZINFO("Number of available tasks: %d", NUM_OF_TASK);
#endif

#if (SMALLOC == 1U)
    /* SMALLOC module has no init function*/
    EZINFO("Initialize smalloc");
    EZINFO("Module Id: 0x%02x", SMALLOC_MOD_ID);
    EZINFO("Availalbe memory: %d bytes", STATIC_MEMORY_SIZE);
#endif

#if (CONFIG_BIN_PARSER == 1U)
    /* BIN_PARSER module has no init function*/
    EZINFO("Initialize binary command parser module");
    EZINFO("Module Id: 0x%02x", BIN_PARSER_MOD_ID);
    EZINFO("payload size: %d bytes", CONFIG_PAYLOAD_MAX_SIZE);
    EZINFO("CRC size: %d bytes", CONFIG_CRC_SIZE);
#endif /* CONFIG_BIN_PARSER */

#if (CONFIG_STATEMACHINE == 1U)
    /* DEBUG module has no init function*/
    EZINFO("Initialize state machine module");
    EZINFO("Module Id: 0x%02x", STATEMACHINE_MOD_ID);
#endif /* CONFIG_STATEMACHINE */

#if (CONFIG_IPC == 1U)
    //ezmIpc_InitModule();
    EZINFO("Initialize IPC module");
    EZINFO("Module Id: 0x%02x", IPC_MOD_ID);
#endif /* CONFIG_IPC */

#if(CONFIG_DRIVERINF == 1U)
    //ezDriver_Initialize();
    EZINFO("Initialize DRIVERINF module");
    EZINFO("Module Id: 0x%02x", DRIVERINF_MOD_ID);
#endif /* CONFIG_DRIVERINF */

#if (CONFIG_FLASH_SIM == 1U)
    if (FlashSim_Initialization())
    {
        EZINFO("Initialize FLASH_SIM module");
        EZINFO("Module Id: 0x%02x", FLASH_SIM_MOD_ID);
    }
    else
    {
        INFO("FLASH_SIM module ERROR");
    }
#endif /* CONFIG_FLASH_SIM*/

#if (CONFIG_HAL_UART)
    EZINFO("Initialize UART Driver");
    EZINFO("Module Id: 0x%02x", UART_MOD_ID);
#endif /* CONFIG_HAL_UART */

#if (CONFIG_HAL_I2C == 1U)
    EZINFO("Initialize I2C HAL Driver");
    EZINFO("Module Id: 0x%02x", I2C_HAL_MOD_ID);
#endif /* CONFIG_HAL_I2C */

#if (CONFIG_LOGGING == 1U)
    EZINFO("Initialize Logging module");
    //Logging_DemoFeatures();
#endif /* CONFIG_LOGGING */

#if (CONFIG_KERNEL == 1U)
    //ezKernel_Initialization();
#endif

#if (CONFIG_EMBEDDED_EMULATOR == 1U)
    EZINFO("Initialize embedded emulator application");
    EZINFO("Module Id: 0x%02x", EMBEDDED_EMULATOR_ID);

    if (ezMbedEmulator_Initialization() == ezSUCCESS)
    {
        EZINFO("Initialize success");
    }
    else
    {
        EZINFO("Initialize fail");
    }
#endif

#if (CONFIG_CLI == 1U && CONFIG_HAL_UART == 1U)
    ezDriveHandle cli_uart = NULL;
    cli_uart = ezDriver_GetDriver("cli_uart");

    if (cli_uart == NULL)
    {
        EZINFO("ERROR: Initialize command line interface");
    }
    else
    {
        EZINFO("Initialize command line interface");
        EZINFO("Module Id: 0x%02x", CLI_MOD_ID);
        EZINFO("Number of supported command: [command = %d]", CONFIG_NUM_OF_CMD);
        ezmCli_Init((UartDrvApi*)cli_uart->ll_interface);
    }
#endif

}

/******************************************************************************
* Internal functions
*******************************************************************************/


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
    EZINFO("******************************************************************************");
    EZINFO("* EASY EMBEDDED SOFTWARE DEVELOPMENT KIT");
    EZINFO("* Author: Quang Hai Nguyen");
    EZINFO("* SDK Version: %s", CONFIG_SDK_VERSION_NUMBER);
    EZINFO("* App Version: %s", CONFIG_APP_VERSION_NUMBER);
    EZINFO("* Build date: 16.01.2022");
#if (CONFIG_WIN == 1U)
    EZINFO("* Platform: Windows");
#endif

#if 0
#if(SUPPORTED_CHIP == ESP32)
    EZINFO("* Platform: ESP32");
#elif (SUPPORTED_CHIP == STM32)
    EZINFO("* Platform: STM32");
#else
    EZINFO("* Platform: PC");
#endif
#endif
    EZINFO("* Contact: hainguyen.ezm@gmail.com");
    EZINFO("******************************************************************************\n\n");
}


/* End of file*/

