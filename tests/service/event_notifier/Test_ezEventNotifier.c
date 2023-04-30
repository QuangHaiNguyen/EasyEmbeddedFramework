
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
#include "ezConfig/sdkconfig.h"

#if(CONFIG_EZ_EVENT_NOTIFIER_TEST == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ezEventNotifier"       /**< module name */
#include "ezUtilities/logging/logging.h"

#include "unity_test_platform/unity.h"
#include "unity_test_platform/unity_fixture.h"
#include <stdint.h>
#include <stdbool.h>
#include "ezService/ezEventNotifier/ezEventNotifier.h"


TEST_GROUP(ezEventNotifier);


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define NOTIFY_CODE_1   1
#define NUM_OF_TEST_OBSERVER    2


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static ezSubject test_subject;
static ezObserver observer1;
static ezObserver observer2;

static uint32_t observer1_notiffy_code;
static uint32_t observer2_notiffy_code;

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
int Observer1_Callback(uint32_t event_code, void *param1, void *param2)
{
    observer1_notiffy_code = event_code;
    return 0;
}


int Observer2_Callback(uint32_t event_code, void *param1, void *param2)
{
    observer2_notiffy_code = event_code;
    return 0;
}


TEST_SETUP(ezEventNotifier)
{
    bool success = true;

    success = (ezSUCCESS == ezEventNotifier_CreateSubject(&test_subject));
    success &= (ezSUCCESS == ezEventNotifier_CreateObserver(&observer1, Observer1_Callback));
    success &= (ezSUCCESS == ezEventNotifier_CreateObserver(&observer2, Observer2_Callback));

    if (!success)
    {
        ERROR("Cannot setup test properly");
    }
    else
    {
        observer1_notiffy_code = 0;
        observer2_notiffy_code = 0;
        success &= (ezSUCCESS == ezEventNotifier_SubscribeToSubject(&test_subject, &observer1));
        success &= (ezSUCCESS == ezEventNotifier_SubscribeToSubject(&test_subject, &observer2));

        if (!success)
        {
            ERROR("Connect subscribe to a subject");
        }
    }
}


TEST_TEAR_DOWN(ezEventNotifier)
{
    ezEventNotifier_ResetSubject(&test_subject);
}


TEST(ezEventNotifier, SubscribeToSubject)
{
    uint16_t num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, NUM_OF_TEST_OBSERVER);
}


TEST(ezEventNotifier, UnsubscribeFromSubject)
{
    uint16_t num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, NUM_OF_TEST_OBSERVER);

    TEST_ASSERT_EQUAL(ezSUCCESS, ezEventNotifier_UnsubscribeFromSubject(&test_subject, &observer1));
    num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, NUM_OF_TEST_OBSERVER - 1);

    TEST_ASSERT_EQUAL(ezSUCCESS, ezEventNotifier_UnsubscribeFromSubject(&test_subject, &observer2));
    num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, 0);
}


TEST(ezEventNotifier, ResetSubject)
{
    uint16_t num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, NUM_OF_TEST_OBSERVER);

    ezEventNotifier_ResetSubject(&test_subject);
    num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, 0);
}


TEST(ezEventNotifier, NotifyEvent1)
{
    uint16_t num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, NUM_OF_TEST_OBSERVER);

    ezEventNotifier_NotifyEvent(&test_subject, NOTIFY_CODE_1, NULL, NULL);
    TEST_ASSERT_EQUAL(NOTIFY_CODE_1, observer1_notiffy_code);
    TEST_ASSERT_EQUAL(NOTIFY_CODE_1, observer2_notiffy_code);
}

#endif /* CONFIG_EZ_EVENT_NOTIFIER_TEST == 1U */

/* End of file */

