/*****************************************************************************
* Filename:         unittest_ez_event_notifier.c
* Author:           Hai Nguyen
* Original Date:    06.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   unittest_ez_event_notifier.c
 *  @author Hai Nguyen
 *  @date   06.03.2024
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "unity.h"
#include "unity_fixture.h"
#include "ez_event_notifier.h"

TEST_GROUP(ez_event_notifier);

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define NOTIFY_CODE_1           1
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
static void RunAllTests(void);
int Observer1_Callback(uint32_t event_code, void *param1, void *param2);
int Observer2_Callback(uint32_t event_code, void *param1, void *param2);


/******************************************************************************
* External functions
*******************************************************************************/
int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}


TEST_SETUP(ez_event_notifier)
{
    bool success = true;

    success = (ezSUCCESS == ezEventNotifier_CreateSubject(&test_subject));
    success &= (ezSUCCESS == ezEventNotifier_CreateObserver(&observer1, Observer1_Callback));
    success &= (ezSUCCESS == ezEventNotifier_CreateObserver(&observer2, Observer2_Callback));

    if (success)
    {
        observer1_notiffy_code = 0;
        observer2_notiffy_code = 0;
        success &= (ezSUCCESS == ezEventNotifier_SubscribeToSubject(&test_subject, &observer1));
        success &= (ezSUCCESS == ezEventNotifier_SubscribeToSubject(&test_subject, &observer2));
    }
}


TEST_TEAR_DOWN(ez_event_notifier)
{
    ezEventNotifier_ResetSubject(&test_subject);
}


TEST_GROUP_RUNNER(ez_event_notifier)
{
    RUN_TEST_CASE(ez_event_notifier, SubscribeToSubject);
    RUN_TEST_CASE(ez_event_notifier, UnsubscribeFromSubject);
    RUN_TEST_CASE(ez_event_notifier, ResetSubject);
    RUN_TEST_CASE(ez_event_notifier, NotifyEvent1);
}


TEST(ez_event_notifier, SubscribeToSubject)
{
    uint16_t num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, NUM_OF_TEST_OBSERVER);
}


TEST(ez_event_notifier, UnsubscribeFromSubject)
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


TEST(ez_event_notifier, ResetSubject)
{
    uint16_t num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, NUM_OF_TEST_OBSERVER);

    ezEventNotifier_ResetSubject(&test_subject);
    num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, 0);
}


TEST(ez_event_notifier, NotifyEvent1)
{
    uint16_t num_of_observers = ezEventNotifier_GetNumOfObservers(&test_subject);
    TEST_ASSERT_EQUAL(num_of_observers, NUM_OF_TEST_OBSERVER);

    ezEventNotifier_NotifyEvent(&test_subject, NOTIFY_CODE_1, NULL, NULL);
    TEST_ASSERT_EQUAL(NOTIFY_CODE_1, observer1_notiffy_code);
    TEST_ASSERT_EQUAL(NOTIFY_CODE_1, observer2_notiffy_code);
}


/******************************************************************************
* Internal functions
*******************************************************************************/
static void RunAllTests(void)
{
    RUN_TEST_GROUP(ez_event_notifier);
}


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


/* End of file */
