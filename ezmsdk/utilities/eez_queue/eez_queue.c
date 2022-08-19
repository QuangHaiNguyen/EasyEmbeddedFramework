
/*******************************************************************************
* Filename:         eez_queue.c
* Author:           Hai Nguyen
* Original Date:    20.08.2022
* Last Update:      20.08.2022
*
* -----------------------------------------------------------------------------
* Comany:           Easy Embedded
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Easy Embedded
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 20.08.2022
*
*******************************************************************************/

/** @file   eez_queue.c
 *  @author Hai Nguyen
 *  @date   20.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "eez_queue.h"

#if (CONFIG_EEZ_QUEUE == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "eez_queue"       /**< module name */
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

#endif /* CONFIG_EEZ_QUEUE == 1U */
/* End of file*/

