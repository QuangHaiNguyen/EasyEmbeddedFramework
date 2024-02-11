
/*******************************************************************************
* Filename:         ezEventNotifier_runner.c
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

/** @file   ezEventNotifier_runner.c
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
#include "ezApp/ezSdk_config.h"

#if(EZ_EVENT_NOTIFIER == 1)

#include "unity.h"
#include "unity_fixture.h"



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
TEST_GROUP_RUNNER(ezEventNotifier)
{
    RUN_TEST_CASE(ezEventNotifier, SubscribeToSubject);
    RUN_TEST_CASE(ezEventNotifier, UnsubscribeFromSubject);
    RUN_TEST_CASE(ezEventNotifier, ResetSubject);
    RUN_TEST_CASE(ezEventNotifier, NotifyEvent1);
}

#endif /* CONFIG_EZ_EVENT_NOTIFIER_TEST == 1U */
/* End of file */

