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

#if (SCHEDULER == 1U)
#include "../scheduler/scheduler.h"
#endif

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
* Function : sum
*//** 
* \b Description:
*
* This function initializes the ring buffer
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* \b Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
int sum(int a, int b)
{
    return a + b;
}

void ezmApp_SdkInit(void)
{
#if APP_DEBUG == 1
    ezm_AppPrintActiveModule();
#endif

#if (SCHEDULER == 1U)
    /*must call init function here, but there is problem with the init so check it later*/
    APPPRINT1("Initialize scheduler");
    APPPRINT2("Number of available tasks: %d", NUM_OF_TASK);
#endif
}

static void ezm_AppPrintActiveModule(void)
{
    APPPRINT1("ACTIVE MODULES:");
#if (CMD_PARSER == 1U)
    APPPRINT1("\t[x] CMD_PARSER");
#else
    APPPRINT1("\t[ ] CMD_PARSER");
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

#if (SMALLOC == 1U)
    APPPRINT1("\t[x] SMALLOC");
#else
    APPPRINT1("\t[ ] SMALLOC");
#endif
}
/* End of file*/