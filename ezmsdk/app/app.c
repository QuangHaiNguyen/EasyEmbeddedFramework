/*******************************************************************************
* Title                 :   app 
* Filename              :   app.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 21.02.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  app.c
 *  @brief This is the source template for a app
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <string.h>

#include "app.h"
#include "app_config.h"
#include "ezmDebug/ezmDebug.h"
#include "unity_test_platform/unity.h"

#if (CONFIG_WIN == 1U)
#include <time.h>
#endif

#if (CONFIG_CLI == 1U)
#include "cli/cli.h"
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

#if (CONFIG_LOGGING == 1U)
#define DEBUG_LVL   LVL_INFO  /**< logging level */
#define MOD_NAME    "APP"   /**< module name */
#include "utilities/logging/logging.h"
#endif /* CONFIG_LOGGING */

#if (CONFIG_HELPER_LINKEDLIST == 1U)
#include "utilities/linked_list/linked_list.h"
#endif /* CONFIG_HELPER_LINKEDLIST */

#if (CONFIG_HELPER_HEXDUMP == 1U)
#include "utilities/hexdump/hexdump.h"
#endif /* CONFIG_HELPER_HEXDUMP */

#if (CONFIG_RING_BUFFER == 1U)
#include "utilities/ring_buffer/ring_buffer.h"
#endif /* CONFIG_RING_BUFFER */

#if (CONFIG_HELPER_ASSERT == 1U)
#include "utilities/ezmAssert/ezmAssert.h"
#endif /* CONFIG_HELPER_ASSERT */

#if (CONFIG_STATEMACHINE == 1U)
#include "statemachine/statemachine.h"
#endif /* CONFIG_STATEMACHINE */

#if (CONFIG_IPC == 1U)
#include "ezmIpc/ezmIpc.h"
#endif /* CONFIG_IPC */

#if (CONFIG_STCMEM==1U)
#include "utilities/stcmem/stcmem.h"
#endif /* CONFIG_STCMEM */

#if(CONFIG_KERNEL == 1U)
#include "ezmKernel/ezmKernel.h"
#endif /* CONFIG_KERNEL */

#if (CONFIG_SYSTEM_ERROR == 1U)
#include "utilities/system_error/system_error.h"
#endif /* CONFIG_SYSTEM_ERROR */

#if (CONFIG_HAL_UART)
#include "hal/uart/uart.h"
#endif /* CONFIG_HAL_UART */


#if (CONFIG_DRIVERINF == 1U)
#include "ezmDriver/driver.h"
#endif /* CONFIG_DRIVERINF */

#if (CONFIG_FLASH_SIM == 1U)
#include "platforms/simulator/flash/flash_simulator.h"
#endif /* CONFIG_FLASH_SIM */

#if (CONFIG_DATA_MODEL == 1U)
#include "data_model/data_model.h"
#endif /* CONFIG_DATA_MODEL == 1U */

#if (CONFIG_MQTT == 1U)
#include "hal/network/mqtt/mqtt.h"
#endif /* CONFIG_MQTT */

#if(CONFIG_EMBEDDED_EMULATOR == 1U)
#include "app/app_embedded_emulator.h"
#endif /* CONFIG_EMBEDDED_EMULATOR */

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
static void     ezm_AppPrintActiveModule(void);
static void     ezmApp_PrintHeader(void);


/******************************************************************************
* Function : ezmApp_SdkInit
*//** 
* \b Description:
*
* Init the modules of the SDK
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    None
* @return   None
*
* \b Example Example:
* @code
* ezmApp_SdkInit()
* @endcode
*
*******************************************************************************/
void ezmApp_SdkInit(void)
{
    ezmApp_PrintHeader();
#if (APP_DEBUG == 1)
    ezm_AppPrintActiveModule();
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
    INFO("Initialize STCMEM module");
    INFO("Module Id: 0x%02x", STCMEM_MOD_ID);
#endif /* CONFIG_STCMEM */

#if(CONFIG_DRIVERINF == 1U)
    if (ezmDriver_Init())
    {
        INFO("Initialize DRIVERINF module");
        INFO("Module Id: 0x%02x", DRIVERINF_MOD_ID);
    }
    else
    {
        INFO("Initialize DRIVERINF module error");
    }
#endif /* CONFIG_DRIVERINF */

#if (CONFIG_SYSTEM_ERROR == 1U)
    SystemError_Initialize();
    INFO("Initialize SYSTEM_ERROR module");
    INFO("Module Id: 0x%02x", SYSTEM_ERROR_MOD_ID);
#endif /* CONFIG_SYSTEM_ERROR*/

#if (CONFIG_DATA_MODEL == 1U)
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

#if (CONFIG_LOGGING == 1U)
    INFO("Initialize Logging module");
    Logging_DemoFeatures();
#endif /* CONFIG_LOGGING */

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
    UartDrvApi* uart_driver;
    ezmDriver_GetDriverInstance(UART0_DRIVER, (void*)(&uart_driver));
    if (uart_driver == NULL)
    {
        INFO("ERROR: Initialize command line interface");
    }
    else
    {
        INFO("Initialize command line interface");
        INFO("Module Id: 0x%02x", CLI_MOD_ID);
        INFO("Number of supported command: [command = %d]", CONFIG_NUM_OF_CMD);
        ezmCli_Init(uart_driver);
    }
#endif

}


/******************************************************************************
* Function : ezm_AppPrintActiveModule
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
* \b Example Example:
* @code
* ezm_AppPrintActiveModule()
* @endcode
*
*******************************************************************************/
static void ezm_AppPrintActiveModule(void)
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

#if (CONFIG_DATA_MODEL == 1U)
    INFO("[x] DATA_MODEL");
#else
    INFO("[ ] DATA_MODEL");
#endif /* CONFIG_DATA_MODEL */

#if (CONFIG_FLASH_SIM == 1U)
    INFO("[x] FLASH_SIM");
#else
    INFO("[ ] FLASH_SIM");
#endif /* CONFIG_FLASH_SIM */

#if (CONFIG_HAL_UART)
    INFO("[x] UART");
#else
    INFO("[ ] UART");
#endif /* CONFIG_HAL_UART */

#if (CONFIG_VIRTUAL_COM)
    INFO("[x] VIRTUAL_COM");
#else
    INFO("[ ] VIRTUAL_COM");
#endif /* CONFIG_VIRTUAL_COM */


    INFO("");
    INFO("******************************************************************************\n\n");

}


static void ezmApp_PrintHeader(void)
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
