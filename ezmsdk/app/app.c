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
#include "app.h"
#include "app_config.h"
#include "ezmDebug/ezmDebug.h"

#if (SUPPORTED_CHIP == WIN)
#include <time.h>
#endif

#if (CLI == 1U)
#include "cli/cli.h"
#endif

#if (SCHEDULER == 1U)
#include "scheduler/scheduler.h"
#endif /* SCHEDULER */

#if (SMALLOC == 1U)
#include "utilities/smalloc/smalloc.h"
#endif /* SMALLOC */

#if (BIN_PARSER == 1U)
#include "binCmdParser/binCmdParser.h"
#endif /* BIN_PARSER */

#if (LOGGING == 1U)
#define DEBUG_LVL   LVL_INFO  /**< logging level */
#define MOD_NAME    "APP"   /**< module name */
#include "utilities/logging/logging.h"
#endif /* DEBUG */

#if (HELPER_LINKEDLIST == 1U)
#include "utilities/linked_list/linked_list.h"
#endif /* HELPER_LINKEDLIST */

#if (HELPER_HEXDUMP == 1U)
#include "utilities/hexdump/hexdump.h"
#endif /* DEBUG */

#if (RING_BUFFER == 1U)
#include "helper/ring_buffer/ring_buffer.h"
#endif /* RING_BUFFER */

#if (HELPER_ASSERT == 1U)
#include "utilities/ezmAssert/ezmAssert.h"
#endif /* HELPER_ASSERT */

#if (STATEMACHINE == 1U)
#include "statemachine/statemachine.h"
#endif /* STATEMACHINE */

#if (IPC == 1U)
#include "ezmIpc/ezmIpc.h"
#endif

#if (STCMEM==1U)
#include "utilities/stcmem/stcmem.h"
#endif

#if(KERNEL == 1U)
#include "ezmKernel/ezmKernel.h"
#endif

#if (SYSTEM_ERROR == 1U)
#include "utilities/system_error/system_error.h"
#endif

#if (NUM_OF_SUPPORTED_UART)
#include "hal/uart/uart.h"
#endif /* NUM_OF_SUPPORTED_UART */

#if (DRIVERINF == 1U)
#include "ezmDriver/driver.h"
#endif

#if (FLASH_SIM == 1U)
#include "platforms/simulator/flash/flash_simulator.h"
#endif /* FLASH_SIM */

#if (DATA_MODEL == 1U)
#include "data_model/data_model.h"
#endif /* DATA_MODEL == 1U */

#if (MQTT == 1U)
#include "hal/network/mqtt/mqtt.h"
#endif
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

#if(SUPPORTED_CHIP == WIN)
static uint32_t ezmApp_ReturnTimestampMillisvoid(void);
#endif
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

#if (BIN_PARSER == 1U)
    /* BIN_PARSER module has no init function*/
    INFO("Initialize binary command parser module");
    INFO("Module Id: 0x%02x", BIN_PARSER_MOD_ID);
    INFO("payload size: %d bytes", PAYLOAD_MAX_SIZE);
    INFO("CRC size: %d bytes", CRC_SIZE);
#endif /* BIN_PARSER */

#if (HELPER_LINKEDLIST == 1U)
    INFO("Initialize linked list module");
    INFO("Module Id: 0x%02x", HELPER_LINKEDLIST_MOD_ID);
    ezmLL_Initialization();
#endif

#if (HELPER_HEXDUMP == 1U)
    /* DEBUG module has no init function*/
    INFO("Initialize hexdump module");
    INFO("Module Id: 0x%02x", HELPER_HEXDUMP_MOD_ID);
#endif

#if (RING_BUFFER == 1U)
    /* DEBUG module has no init function*/
    INFO("Initialize ring buffer module");
    INFO("Module Id: 0x%02x", RING_BUFFER_MOD_ID);
#endif /* RING_BUFFER */

#if (HELPER_ASSERT == 1U)
    /* DEBUG module has no init function*/
    INFO("Initialize assert module");
    INFO("Module Id: 0x%02x", HELPER_ASSERT_MOD_ID);
#endif

#if (STATEMACHINE == 1U)
    /* DEBUG module has no init function*/
    INFO("Initialize state machine module");
    INFO("Module Id: 0x%02x", STATEMACHINE_MOD_ID);
#endif

#if (IPC == 1U)
    ezmIpc_InitModule();
    INFO("Initialize IPC module");
    INFO("Module Id: 0x%02x", IPC_MOD_ID);
    
#endif

#if (STCMEM==1U)
    ezmStcMem_Initialization();
    INFO("Initialize STCMEM module");
    INFO("Module Id: 0x%02x", STCMEM_MOD_ID);
#endif

#if(DRIVERINF == 1U)
    if (ezmDriver_Init())
    {
        INFO("Initialize DRIVERINF module");
        INFO("Module Id: 0x%02x", DRIVERINF_MOD_ID);
    }
    else
    {
        INFO("Initialize DRIVERINF module error");
    }
#endif

#if (SYSTEM_ERROR == 1U)
    SystemError_Initialize();
    INFO("Initialize SYSTEM_ERROR module");
    INFO("Module Id: 0x%02x", SYSTEM_ERROR_MOD_ID);
#endif /* SYSTEM_ERROR*/

#if (DATA_MODEL == 1U)
    DataModel_Initialization();
    INFO("Initialize DATA_MODEL module");
#endif /* DATA_MODEL == 1U */

#if (FLASH_SIM == 1U)
    if (FlashSim_Initialization())
    {
        INFO("Initialize FLASH_SIM module");
        INFO("Module Id: 0x%02x", FLASH_SIM_MOD_ID);
    }
    else
    {
        INFO("FLASH_SIM module ERROR");
    }
#endif /* SYSTEM_ERROR*/

#if (NUM_OF_SUPPORTED_UART)
    INFO("Initialize UART Driver");
    INFO("Module Id: 0x%02x", UART_MOD_ID);
#endif /* NUM_OF_SUPPORTED_UART */

#if (LOGGING == 1U)
    INFO("Initialize Logging module");
    Logging_DemoFeatures();
#endif /* LOGGING == 1U */

#if (CLI == 1U && HAL_UART == 1U)
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
        INFO("Number of supported command: [command = %d]", NUM_OF_CMD);
        ezmCli_Init(uart_driver);
    }
#endif

#if(SUPPORTED_CHIP == WIN)
    uint64_t execute_time_stamp = ezmApp_ReturnTimestampMillisvoid();
    do
    {
#if (CLI == 1U)
        ezmCli_Run();
#endif
        if (ezmApp_ReturnTimestampMillisvoid() - execute_time_stamp > 1)
        {
            ezmKernel_UpdateClock();
            ezmKernel_Run();
            execute_time_stamp = ezmApp_ReturnTimestampMillisvoid();
        }
    }while (execute_time_stamp);
#endif /* SUPPORTED_CHIP */

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

#if (CLI == 1U)
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

#if (BIN_PARSER == 1U)
    INFO("[x] BIN_PARSER");
#else
    INFO("[ ] BIN_PARSER");
#endif

#if (EZM_DEBUG == 1U)
    INFO("[x] DEBUG");
#else
    INFO("[ ] DEBUG");
#endif

#if (LOGGING == 1U)
    INFO("[x] LOGGING");
#else
    INFO("[ ] LOGGING");
#endif

#if (HELPER_LINKEDLIST == 1U)
    INFO("[x] HELPER_LINKEDLIST");
#else
    INFO("[ ] HELPER_LINKEDLIST");
#endif

#if (HELPER_HEXDUMP == 1U)
    INFO("[x] HELPER_HEXDUMP");
#else
    INFO("[ ] HELPER_HEXDUMP");
#endif

#if (RING_BUFFER == 1U)
    INFO("[x] RING_BUFFER");
#else
    INFO("[ ] RING_BUFFER");
#endif

#if (HELPER_ASSERT == 1U)
    INFO("[x] HELPER_ASSERT");
#else
    INFO("[ ] HELPER_ASSERT");
#endif

#if (STATEMACHINE == 1U)
    INFO("[x] STATEMACHINE");
#else
    INFO("[ ] STATEMACHINE");
#endif

#if (IPC == 1U)
    INFO("[x] IPC");
#else
    INFO("[ ] IPC");
#endif

#if (STCMEM==1U)
    INFO("[x] STCMEM");
#else
    INFO("[ ] STCMEM");
#endif

#if(DRIVERINF == 1U)
    INFO("[x] DRIVERINF");
#else
    INFO("[ ] DRIVERINF");
#endif

#if (SYSTEM_ERROR == 1U)
    INFO("[x] SYSTEM_ERROR");
#else
    INFO("[ ] SYSTEM_ERROR");
#endif /* SYSTEM_ERROR*/

#if (DATA_MODEL == 1U)
    INFO("[x] DATA_MODEL");
#else
    INFO("[ ] DATA_MODEL");
#endif /* DATA_MODEL */

#if (FLASH_SIM == 1U)
    INFO("[x] FLASH_SIM");
#else
    INFO("[ ] FLASH_SIM");
#endif /* FLASH_SIM */

#if (NUM_OF_SUPPORTED_UART)
    INFO("[x] UART");
#else
    INFO("[ ] UART");
#endif /* NUM_OF_SUPPORTED_UART */

    INFO("");
    INFO("******************************************************************************\n\n");

}

static void ezmApp_PrintHeader(void)
{
    INFO("******************************************************************************");
    INFO("* EASY EMBEDDED SOFTWARE DEVELOPMENT KIT");
    INFO("* Author: Quang Hai Nguyen");
    INFO("* Version: 1.0.0");
    INFO("* Build date: 16.01.2022");
#if(SUPPORTED_CHIP == ESP32)
    INFO("* Platform: ESP32");
#elif (SUPPORTED_CHIP == STM32)
    INFO("* Platform: STM32");
#else
    INFO("* Platform: PC");
#endif
    INFO("* Contact: hainguyen.ezm@gmail.com");
    INFO("******************************************************************************\n\n");
}

#if(SUPPORTED_CHIP == WIN)
static uint32_t ezmApp_ReturnTimestampMillisvoid(void)
{
    uint32_t tick_milli;
    clock_t tick;
    tick = clock();
    tick_milli = tick / (CLOCKS_PER_SEC / 1000);

    return tick_milli;
}
#endif
/* End of file*/
