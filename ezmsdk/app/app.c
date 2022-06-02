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

#define MOD_NAME "APPL"

#if (MODULE_DEBUG == 1U) && (APP_DEBUG == 1U)
    #define APPPRINT(a)                     PRINT_DEBUG(MOD_NAME, a)
    #define APPPRINT1(a,b)                  PRINT_DEBUG1(MOD_NAME,a,b)
    #define APPPRINT2(a,b,c)                PRINT_DEBUG2(MOD_NAME,a,b,c)
    #define APPPRINT3(a,b,c,d)              PRINT_DEBUG3(MOD_NAME,a,b,c,d)
    #define APPPRINT4(a,b,c,d,e)            PRINT_DEBUG3(MOD_NAME,a,b,c,d,e)
#else 
    #define APPPRINT(a)
    #define APPPRINT1(a,b)
    #define APPPRINT2(a,b,c)
    #define APPPRINT3(a,b,c,d)
    #define APPPRINT4(a,b,c,d,e)
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
    APPPRINT1("Initialize scheduler");
    APPPRINT2("Module Id: 0x%02x", SCHEDULER_MOD_ID);
    APPPRINT2("Number of available tasks: %d", NUM_OF_TASK);
#endif

#if (SMALLOC == 1U)
    /* SMALLOC module has no init function*/
    APPPRINT("Initialize smalloc");
    APPPRINT1("Module Id: 0x%02x", SMALLOC_MOD_ID);
    APPPRINT1("Availalbe memory: %d bytes", STATIC_MEMORY_SIZE);
#endif

#if (BIN_PARSER == 1U)
    /* BIN_PARSER module has no init function*/
    APPPRINT("Initialize binary command parser module");
    APPPRINT1("Module Id: 0x%02x", BIN_PARSER_MOD_ID);
    APPPRINT1("payload size: %d bytes", PAYLOAD_MAX_SIZE);
    APPPRINT1("CRC size: %d bytes", CRC_SIZE);
#endif /* BIN_PARSER */

#if (DEBUG == 1U)
    /* DEBUG module has no init function*/
    APPPRINT("Initialize debug module");
    APPPRINT1("Module Id: 0x%02x", DEBUG_MOD_ID);
#endif

#if (HELPER_LINKEDLIST == 1U)
    APPPRINT("Initialize linked list module");
    APPPRINT1("Module Id: 0x%02x", HELPER_LINKEDLIST_MOD_ID);
    ezmLL_Initialization();
#endif

#if (HELPER_HEXDUMP == 1U)
    /* DEBUG module has no init function*/
    APPPRINT("Initialize hexdump module");
    APPPRINT1("Module Id: 0x%02x", HELPER_HEXDUMP_MOD_ID);
#endif

#if (RING_BUFFER == 1U)
    /* DEBUG module has no init function*/
    APPPRINT("Initialize ring buffer module");
    APPPRINT1("Module Id: 0x%02x", RING_BUFFER_MOD_ID);
#endif /* RING_BUFFER */

#if (HELPER_ASSERT == 1U)
    /* DEBUG module has no init function*/
    APPPRINT("Initialize assert module");
    APPPRINT1("Module Id: 0x%02x", HELPER_ASSERT_MOD_ID);
#endif

#if (STATEMACHINE == 1U)
    /* DEBUG module has no init function*/
    APPPRINT1("Initialize state machine module");
    APPPRINT2("Module Id: 0x%02x", STATEMACHINE_MOD_ID);
#endif

#if (IPC == 1U)
    ezmIpc_InitModule();
    APPPRINT("Initialize IPC module");
    APPPRINT1("Module Id: 0x%02x", IPC_MOD_ID);
    
#endif

#if (STCMEM==1U)
    ezmStcMem_Initialization();
    APPPRINT("Initialize STCMEM module");
    APPPRINT1("Module Id: 0x%02x", STCMEM_MOD_ID);
#endif

#if(DRIVERINF == 1U)
    if (ezmDriver_Init())
    {
        APPPRINT("Initialize DRIVERINF module");
        APPPRINT1("Module Id: 0x%02x", DRIVERINF_MOD_ID);
    }
    else
    {
        APPPRINT("Initialize DRIVERINF module error");
    }
#endif

#if (SYSTEM_ERROR == 1U)
    SystemError_Initialize();
    APPPRINT("Initialize SYSTEM_ERROR module");
    APPPRINT1("Module Id: 0x%02x", SYSTEM_ERROR_MOD_ID);
#endif /* SYSTEM_ERROR*/

#if (FLASH_SIM == 1U)
    if (FlashSim_Initialization())
    {
        APPPRINT("Initialize FLASH_SIM module");
        APPPRINT1("Module Id: 0x%02x", FLASH_SIM_MOD_ID);
    }
    else
    {
        APPPRINT("FLASH_SIM module ERROR");
    }
#endif /* SYSTEM_ERROR*/

#if (NUM_OF_SUPPORTED_UART)
    APPPRINT("Initialize UART Driver");
    APPPRINT1("Module Id: 0x%02x", UART_MOD_ID);
#endif /* NUM_OF_SUPPORTED_UART */

#if (LOGGING == 1U)
    APPPRINT("Initialize Logging module");
    Logging_DemoFeatures();
#endif /* LOGGING == 1U */

#if (CLI == 1U)

    UartDrvApi* uart_driver;
    ezmDriver_GetDriverInstance(UART0_DRIVER, (void*)(&uart_driver));
    if (uart_driver == NULL)
    {
        APPPRINT("ERROR: Initialize command line interface");
    }
    else
    {
        APPPRINT("Initialize command line interface");
        APPPRINT1("Module Id: 0x%02x", CLI_MOD_ID);
        APPPRINT1("Number of supported command: [command = %d]", NUM_OF_CMD);
        ezmCli_Init(uart_driver);
    }
#endif

#if(SUPPORTED_CHIP == WIN)
    uint64_t execute_time_stamp = ezmApp_ReturnTimestampMillisvoid();
    do
    {
        ezmCli_Run();
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
    APPPRINT("ACTIVE MODULES:");

#if (CLI == 1U)
    APPPRINT("[x] CLI");
#else
    APPPRINT("[ ] CLI");
#endif

#if (SCHEDULER == 1U)
    APPPRINT("[x] SCHEDULER");
#else
    APPPRINT("[ ] SCHEDULER");
#endif

#if (SMALLOC == 1U)
    APPPRINT("[x] SMALLOC");
#else
    APPPRINT("[ ] SMALLOC");
#endif

#if (BIN_PARSER == 1U)
    APPPRINT("[x] BIN_PARSER");
#else
    APPPRINT("[ ] BIN_PARSER");
#endif

#if (EZM_DEBUG == 1U)
    APPPRINT("[x] DEBUG");
#else
    APPPRINT("[ ] DEBUG");
#endif

#if (LOGGING == 1U)
    APPPRINT("[x] LOGGING");
#else
    APPPRINT("[ ] LOGGING");
#endif

#if (HELPER_LINKEDLIST == 1U)
    APPPRINT("[x] HELPER_LINKEDLIST");
#else
    APPPRINT("[ ] HELPER_LINKEDLIST");
#endif

#if (HELPER_HEXDUMP == 1U)
    APPPRINT("[x] HELPER_HEXDUMP");
#else
    APPPRINT("[ ] HELPER_HEXDUMP");
#endif

#if (RING_BUFFER == 1U)
    APPPRINT("[x] RING_BUFFER");
#else
    APPPRINT("[ ] RING_BUFFER");
#endif

#if (HELPER_ASSERT == 1U)
    APPPRINT("[x] HELPER_ASSERT");
#else
    APPPRINT("[ ] HELPER_ASSERT");
#endif

#if (STATEMACHINE == 1U)
    APPPRINT("[x] STATEMACHINE");
#else
    APPPRINT("[ ] STATEMACHINE");
#endif

#if (IPC == 1U)
    APPPRINT("[x] IPC");
#else
    APPPRINT("[ ] IPC");
#endif

#if (STCMEM==1U)
    APPPRINT("[x] STCMEM");
#else
    APPPRINT("[ ] STCMEM");
#endif

#if(DRIVERINF == 1U)
    APPPRINT("[x] DRIVERINF");
#else
    APPPRINT("[ ] DRIVERINF");
#endif

#if (SYSTEM_ERROR == 1U)
    APPPRINT("[x] SYSTEM_ERROR");
#else
    APPPRINT("[ ] SYSTEM_ERROR");
#endif /* SYSTEM_ERROR*/

#if (FLASH_SIM == 1U)
    APPPRINT("[x] FLASH_SIM");
#else
    APPPRINT("[ ] FLASH_SIM");
#endif /* FLASH_SIM */

#if (NUM_OF_SUPPORTED_UART)
    APPPRINT("[x] UART");
#else
    APPPRINT("[ ] UART");
#endif /* NUM_OF_SUPPORTED_UART */

 APPPRINT("");
 APPPRINT("******************************************************************************\n\n");

}

static void ezmApp_PrintHeader(void)
{
    PRINT_INFO("******************************************************************************");
    PRINT_INFO("* EASY EMBEDDED SOFTWARE DEVELOPMENT KIT");
    PRINT_INFO("* Author: Quang Hai Nguyen");
    PRINT_INFO("* Version: 1.0.0");
    PRINT_INFO("* Build date: 16.01.2022");
#if(SUPPORTED_CHIP == ESP32)
    PRINT_INFO("* Platform: ESP32");
#elif (SUPPORTED_CHIP == STM32)
    PRINT_INFO("* Platform: STM32");
#else
    PRINT_INFO("* Platform: PC");
#endif
    PRINT_INFO("* Contact: hainguyen.ezm@gmail.com");
    PRINT_INFO("******************************************************************************\n\n");
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
