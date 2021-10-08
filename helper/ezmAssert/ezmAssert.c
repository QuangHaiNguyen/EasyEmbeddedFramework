/*******************************************************************************
* Title                 :   ezmAssert 
* Filename              :   ezmAssert.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.02.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 24.05.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  ezmAssert.c
 *  @brief This is the source template for a ezmAssert
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezmAssert.h"

#include "../../ezmDebug/ezmDebug.h"

#if (MODULE_DEBUG == 1U) && (ASSERT_DBG == 1U)
    #define ASSERTPRINT1(a)             PRINT_DEBUG1(a)               
    #define ASSERTPRINT2(a,b)           PRINT_DEBUG2(a,b)             
    #define ASSERTPRINT3(a,b,c)         PRINT_DEBUG3(a,b,c)
#else 
    #define ASSERTPRINT1(a)           
    #define ASSERTPRINT2(a,b)           
    #define ASSERTPRINT3(a,b,c)

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
/* None */

/******************************************************************************
* Function : ezmAssert_AssertFailHandler
*//** 
* \b Description:
*
* This function handles when assert fail. it must be adapted to the hardware. For example
* log fail code, reset system... 
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    u32ModuleId: (IN)id of the module
* @param    u32LineNumer: (IN)line where assert fails
* @return   None
*
* \b Example Example:
* @code
* int var = 10;
* ASSERT(var > 10);
* @endcode
*
*******************************************************************************/
void ezmAssert_AssertFailHandler(int u32ModuleId, int u32LineNumer)
{
    ASSERTPRINT3("assert fail add module id: %d, line: %d", u32ModuleId, u32LineNumer);

    /*Further action must be adapted to hardware*/
}
/* End of file*/