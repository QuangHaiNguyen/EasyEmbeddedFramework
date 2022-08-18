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

#if (CONFIG_HELPER_ASSERT == 1U)

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

#endif /* CONFIG_HELPER_ASSERT */
/* End of file*/