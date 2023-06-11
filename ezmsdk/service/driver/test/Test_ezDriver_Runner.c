
/*******************************************************************************
* Filename:         ezDriver_runner.c
* Author:           Hai Nguyen
* Original Date:    24.09.2022
* Last Update:      24.09.2022
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
* Written by Hai Nguyen 24.09.2022
*
*******************************************************************************/

/** @file   ezDriver_runner.c
 *  @author Hai Nguyen
 *  @date   24.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/

#include "ezApp/ezSdk_config.h"

#if (CONFIG_DRIVERINF_TEST == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ezDriver_runner"       /**< module name */
#include "ezUtilities/logging/logging.h"

#include "unity_test_platform/unity.h"
#include "unity_test_platform/unity_fixture.h"
#include <stdint.h>
#include <stdbool.h>



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
TEST_GROUP_RUNNER(ezDriver)
{
    RUN_TEST_CASE(ezDriver, Test_ezDriver_GetDriver_WrongDriverName);
    RUN_TEST_CASE(ezDriver, Test_ezDriver_GetDriver_CorrectDriverName);
    RUN_TEST_CASE(ezDriver, Test_ezDriver_Write);
    RUN_TEST_CASE(ezDriver, Test_ezDriver_Read);
}

#endif /*CONFIG_DRIVERINF_TEST == 1U */

/* End of file */
