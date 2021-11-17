/*******************************************************************************
* Title                 :   cli 
* Filename              :   cli.c
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

/** @file  cli.c
 *  @brief This is the source template for a cli
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "cli.h"
#include "../ezmDebug/ezmDebug.h"
#include "../helper/linked_list/linked_list.h"

#if (CLI == 1U)

#if (MODULE_DEBUG == 1U) && (CLI_DEBUG == 1U)
    #define CLIPRINT1(a)                    PRINT_DEBUG1(a)               
    #define CLIPRINT2(a,b)                  PRINT_DEBUG2(a,b)             
    #define CLIPRINT3(a,b,c)                PRINT_DEBUG3(a,b,c) 
#else 
    #define CLIPRINT1(a)           
    #define CLIPRINT2(a,b)           
    #define CLIPRINT3(a,b,c)
#endif

#define CLI_INDEX_INVALID

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef struct 
{
    const char * pu8Command; /**< */
    const char * pu8Description;
    CLI_CALLBACK pfnCallback;
    LinkedList stArgumentList;
    LinkedList stValueList;
}CommandMetadata;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
CommandMetadata astMetaData[NUM_OF_CMD];

/******************************************************************************
* Function Definitions
*******************************************************************************/
uint8_t ezmCli_GetFreeInstance(void);
bool    ezmCli_ResetMetaData(uint8_t u8Index);

/* Public function ***********************************************************/

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
void ezmCli_Init(void)
{
    CLIPRINT1("ezmCli_Init() called");
}


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
uint8_t ezmCli_RegisterCommand(const char * pu8Command, const char * pu8Description, CLI_CALLBACK pfnCallback)
{
    CLIPRINT1("ezmCli_RegisterCommand() called");
    return 0;
}


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
bool ezmCli_AddArgument(const char * pu8LongForm, const char * pu8ShortForm, const char * pu8Description)
{
    CLIPRINT1("ezmCli_AddArgument() called");
    return true;
}

/* Private function **********************************************************/

#endif /* CLI */

/* End of file*/