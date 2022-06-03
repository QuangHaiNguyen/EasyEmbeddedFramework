
/*******************************************************************************
* Filename:         logging.c
* Author:           Hai Nguyen
* Original Date:    01.06.2022
* Last Update:      01.06.2022
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
* Written by Hai Nguyen 01.06.2022
*
*******************************************************************************/

/** @file   logging.c
 *  @author Hai Nguyen
 *  @date   01.06.2022
 *  @brief  This is the source for a logging module
 *  
 *  @details this source file will be reserve for porting logging module to
 *  other interface. At the moment, it prints the supported logging level and
 *  format
 * 
 */



/******************************************************************************
* Includes
*******************************************************************************/
#define DEBUG_LVL   LVL_TRACE  /**< logging level */
#define MOD_NAME    "LOGGING"   /**< module name */
#include "logging.h"

#if (LOGGING == 1U)

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : Logging_DemoFeatures
*//** 
* @Description:
*
* This function demostrates some features of logging module
* 
* @param    None
* @return   None
*
*******************************************************************************/
void Logging_DemoFeatures(void)
{
    INFO("\n\nsupported logging level and format:");
    TRACE("This is a test message, [integer = %d], [float = %f], [char = %c]", 
                10, 3.14, 'a');
    DEBUG("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    INFO("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    WARNING("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    ERROR("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    CRITICAL("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    INFO("\n\n");
}

/******************************************************************************
* Internal functions
*******************************************************************************/

#endif /* LOGGING == 1U */
/* End of file*/

