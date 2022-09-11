
/*******************************************************************************
* Filename:         ezQueue_runner.c
* Author:           Hai Nguyen
* Original Date:    11.09.2022
* Last Update:      11.09.2022
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
* Written by Hai Nguyen 11.09.2022
*
*******************************************************************************/

/** @file   ezQueue_runner.c
 *  @author Hai Nguyen
 *  @date   11.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (CONFIG_EZ_QUEUE_TEST == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ezQueue_runner"       /**< module name */
#include "utilities/logging/logging.h"

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
TEST_GROUP_RUNNER(ezQueue)
{
    RUN_TEST_CASE(ezQueue, CreateQueueFail);
    RUN_TEST_CASE(ezQueue, CreateQueueSuccess);
    RUN_TEST_CASE(ezQueue, PopEmptyQueue);
    RUN_TEST_CASE(ezQueue, PushQueueFail);
    RUN_TEST_CASE(ezQueue, PushQueueSuccess);
    RUN_TEST_CASE(ezQueue, test_GetFrontPop);
    RUN_TEST_CASE(ezQueue, GetBackPop);
    RUN_TEST_CASE(ezQueue, OverflowQueue);
    RUN_TEST_CASE(ezQueue, ezQueue_ReserveElement);
}

#endif /* CONFIG_EZ_QUEUE_TEST == 1U */

/* End of file */