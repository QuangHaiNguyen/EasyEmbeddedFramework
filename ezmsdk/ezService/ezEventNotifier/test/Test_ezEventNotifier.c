
/*******************************************************************************
* Filename:         ezEventNotifier.c
* Author:           Hai Nguyen
* Original Date:    10.04.2023
* Last Update:      10.04.2023
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
* Written by Hai Nguyen 10.04.2023
*
*******************************************************************************/

/** @file   ezEventNotifier.c
 *  @author Hai Nguyen
 *  @date   10.04.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ezEventNotifier"       /**< module name */
#include "ezUtilities/logging/logging.h"

#include "unity_test_platform/unity.h"
#include "unity_test_platform/unity_fixture.h"
#include <stdint.h>
#include <stdbool.h>



TEST_GROUP(ezEventNotifier);


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
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/
/* None */


/******************************************************************************
* Internal functions
*******************************************************************************/


TEST_SETUP(ezEventNotifier)
{
}


TEST_TEAR_DOWN(ezEventNotifier)
{
}


TEST(ezEventNotifier, TestTempPlate)
{
    TEST_ASSERT_EQUAL(false, false);
}

/* End of file */

