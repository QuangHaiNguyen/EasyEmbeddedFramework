
/*******************************************************************************
* Filename:         ezRpc.c
* Author:           Hai Nguyen
* Original Date:    31.08.2022
* Last Update:      31.08.2022
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
* Written by Hai Nguyen 31.08.2022
*
*******************************************************************************/

/** @file   ezRpc.c
 *  @author Hai Nguyen
 *  @date   31.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezRpc.h"

#if (CONFIG_RPC == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ezRpc"       /**< module name */
#include "utilities/logging/logging.h"

/*the rest of include go here*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

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
* External functions
*******************************************************************************/

/******************************************************************************
* Function : sum
*//** 
* @Description:
*
* This function initializes the ring buffer
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
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

/******************************************************************************
* Internal functions
*******************************************************************************/

#endif /* CONFIG_RPC == 1U */
/* End of file*/

