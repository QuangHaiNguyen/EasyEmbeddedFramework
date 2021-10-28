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

#if (SCHEDULER == 1U)
#include "../scheduler/scheduler.h"
#endif /* SCHEDULER */

#if (SMALLOC == 1U)
#include "../helper/smalloc/smalloc.h"
#endif /* SMALLOC */

#if (BIN_PARSER == 1U)
#include "../binCmdParser/binCmdParser.h"
#endif /* BIN_PARSER */

#if (MODULE_DEBUG == 1U) && (APP_DEBUG == 1U)
    #define APPPRINT1(a)                    PRINT_DEBUG1(a)               
    #define APPPRINT2(a,b)                  PRINT_DEBUG2(a,b)             
    #define APPPRINT3(a,b,c)                PRINT_DEBUG3(a,b,c) 
#else 
    #define APPPRINT1(a)           
    #define APPPRINT2(a,b)           
    #define APPPRINT3(a,b,c)
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
static void ezm_AppPrintActiveModule(void);

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
    APPPRINT1("Initialize smalloc");
    APPPRINT2("Module Id: 0x%02x", SMALLOC_MOD_ID);
    APPPRINT2("Availalbe memory: %d bytes", STATIC_MEMORY_SIZE);
#endif

#if (BIN_PARSER == 1U)
    /* BIN_PARSER module has no init function*/
    APPPRINT1("Initialize binary command parser module");
    APPPRINT2("Module Id: 0x%02x", BIN_PARSER_MOD_ID);
    APPPRINT2("payload size: %d bytes", PAYLOAD_MAX_SIZE);
    APPPRINT2("CRC size: %d bytes", CRC_SIZE);
    APPPRINT2("Start of frame: 0x%02x ", SOF);
#endif /* BIN_PARSER */

#if (DEBUG == 1U)
    /* DEBUG module has no init function*/
    APPPRINT1("Initialize debug module");
    APPPRINT2("Module Id: 0x%02x", DEBUG_MOD_ID);
#endif

#if (HELPER_LINKEDLIST == 1U)
    /* DEBUG module has no init function*/
    APPPRINT1("Initialize linked list module");
    APPPRINT2("Module Id: 0x%02x", HELPER_LINKEDLIST_MOD_ID);
#endif

#if (HELPER_HEXDUMP == 1U)
    /* DEBUG module has no init function*/
    APPPRINT1("Initialize hexdump module");
    APPPRINT2("Module Id: 0x%02x", HELPER_HEXDUMP_MOD_ID);
#endif

#if (RING_BUFFER == 1U)
    /* DEBUG module has no init function*/
    APPPRINT1("Initialize ring buffer module");
    APPPRINT2("Module Id: 0x%02x", RING_BUFFER_MOD_ID);

#if (STATIC_MEM == 1)
    APPPRINT2("Using static memory, buffer size of each ring %d bytes", STATIC_MEM_SIZE);
#else
    APPPRINT1("Using dynamic memory allocation with SMALLOC module");
#endif /*STATIC_MEM*/
#endif /* RING_BUFFER */

#if (HELPER_ASSERT == 1U)
    /* DEBUG module has no init function*/
    APPPRINT1("Initialize assert module");
    APPPRINT2("Module Id: 0x%02x", HELPER_ASSERT_MOD_ID);
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
    APPPRINT1("ACTIVE MODULES:");

#if (SCHEDULER == 1U)
    APPPRINT1("\t[x] SCHEDULER");
#else
    APPPRINT1("\t[ ] SCHEDULER");
#endif

#if (SMALLOC == 1U)
    APPPRINT1("\t[x] SMALLOC");
#else
    APPPRINT1("\t[ ] SMALLOC");
#endif

#if (BIN_PARSER == 1U)
    APPPRINT1("\t[x] BIN_PARSER");
#else
    APPPRINT1("\t[ ] BIN_PARSER");
#endif

#if (DEBUG == 1U)
    APPPRINT1("\t[x] DEBUG");
#else
    APPPRINT1("\t[ ] DEBUG");
#endif

#if (HELPER_LINKEDLIST == 1U)
    APPPRINT1("\t[x] HELPER_LINKEDLIST");    
#else
    APPPRINT1("\t[ ] HELPER_LINKEDLIST");
#endif

#if (HELPER_HEXDUMP == 1U)
    APPPRINT1("\t[x] HELPER_HEXDUMP");
#else
    APPPRINT1("\t[ ] HELPER_HEXDUMP");
#endif

#if (RING_BUFFER == 1U)
    APPPRINT1("\t[x] RING_BUFFER");
#else
    APPPRINT1("\t[ ] RING_BUFFER");
#endif

#if (HELPER_ASSERT == 1U)
    APPPRINT1("\t[x] HELPER_ASSERT");
#else
    APPPRINT1("\t[ ] HELPER_ASSERT");
#endif

}
/* End of file*/