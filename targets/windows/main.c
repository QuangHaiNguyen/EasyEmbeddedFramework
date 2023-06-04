
/*******************************************************************************
* Filename:         main.c
* Author:           Hai Nguyen
* Original Date:    02.05.2023
* Last Update:      02.05.2023
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
* Written by Hai Nguyen 02.05.2023
*
*******************************************************************************/

/** @file   main.c
 *  @author Hai Nguyen
 *  @date   02.05.2023
 *  @brief  main file for windows project
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "main"       /**< module name */
#include "utilities/logging/ez_logging.h"

/*the rest of include go here*/
#include "service/kernel/ez_kernel.h"

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
* Function : main
*//** 
* @Description:
*
* main function, entry point of the application
* 
* @param    None
* @return   0: success, else: error
*
*******************************************************************************/
int main(void)
{
    ezSdk_Initialization();
    return 0;
}

/******************************************************************************
* Internal functions
*******************************************************************************/

/* End of file*/
