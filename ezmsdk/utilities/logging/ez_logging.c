
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
#include "ez_logging.h"

#if (EZ_LOGGING == 1U)

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
    EZINFO("supported logging level and format:");
    EZTRACE("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    EZDEBUG("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    EZINFO("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    EZWARNING("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    EZERROR("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');
    EZCRITICAL("This is a test message, [integer = %d], [float = %f], [char = %c]",
                10, 3.14, 'a');

    EZINFO("Test hex dump");
    EZHEXDUMP("this is a test", sizeof("this is a test"));
}

/******************************************************************************
* Internal functions
*******************************************************************************/
/* None */


#endif /* CONFIG_LOGGING */
/* End of file*/

