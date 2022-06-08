
/*******************************************************************************
* Filename:         unit_test_event_notifier.h
* Author:           Hai Nguyen
* Original Date:    08.06.2022
* Last Update:      08.06.2022
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
* Written by Hai Nguyen 08.06.2022
*
*******************************************************************************/

#ifndef _UNIT_TEST_EVENT_NOTIFIER
#define _UNIT_TEST_EVENT_NOTIFIER

#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "utilities/event_notifier/event_notifier.h"

#define NUM_SUPORTED_SUB    2U
#define TEST_EVENT_CODE     10U

    uint8_t Sub1EventCount = 0;
    uint32_t Sub1EventCode = 0;

    uint8_t Sub2EventCount = 0;
    uint32_t Sub2EventCode = 0;

    int Sub1EventHandler(uint32_t event_code, void* param1, void* param2)
    {
        Sub1EventCount++;
        Sub1EventCode = event_code;
        return 0;
    }

    int Sub2EventHandler(uint32_t event_code, void* param1, void* param2)
    {
        Sub2EventCount++;
        Sub2EventCode = event_code;
        return 0;
    }
}

namespace 
{
    TEST(event_notifier, init) 
    {
        evntNoti_Initialize();
        evnt_pub publisher;
        evnt_sub subscriber1;

        publisher = evntNoti_CreatePublisher(NUM_SUPORTED_SUB);
        subscriber1 = evntNoti_SubscribeEvent(publisher, Sub1EventHandler);
        ASSERT_NE(publisher, UINT32_MAX);
        ASSERT_NE(subscriber1, UINT32_MAX);
    }

    TEST(event_notifier, max_subscriber)
    {
        evntNoti_Initialize();
        evnt_pub publisher;
        evnt_sub subscriber1;
        evnt_sub subscriber2;
        evnt_sub subscriber3;

        publisher = evntNoti_CreatePublisher(NUM_SUPORTED_SUB);
        subscriber1 = evntNoti_SubscribeEvent(publisher, Sub1EventHandler);
        subscriber2 = evntNoti_SubscribeEvent(publisher, Sub2EventHandler);
        subscriber3 = evntNoti_SubscribeEvent(publisher, Sub2EventHandler);

        ASSERT_NE(publisher, UINT32_MAX);
        ASSERT_NE(subscriber1, UINT32_MAX);
        ASSERT_NE(subscriber2, UINT32_MAX);
        ASSERT_EQ(subscriber3, UINT32_MAX);
    }

    TEST(event_notifier, event_notify)
    {
        evntNoti_Initialize();
        evnt_pub publisher;
        evnt_sub subscriber1;
        evnt_sub subscriber2;

        publisher = evntNoti_CreatePublisher(NUM_SUPORTED_SUB);
        subscriber1 = evntNoti_SubscribeEvent(publisher, Sub1EventHandler);
        subscriber2 = evntNoti_SubscribeEvent(publisher, Sub2EventHandler);

        evntNoti_NotifyEnvent(publisher, TEST_EVENT_CODE, NULL, NULL);

        ASSERT_EQ(Sub1EventCount, 1);
        ASSERT_EQ(Sub1EventCode, TEST_EVENT_CODE);

        ASSERT_EQ(Sub2EventCount, 1);
        ASSERT_EQ(Sub2EventCode, TEST_EVENT_CODE);

        /* reset for next test */
        Sub1EventCount = 0;
        Sub1EventCode = 0;

        Sub2EventCount = 0;
        Sub2EventCode = 0;
    }

    TEST(event_notifier, unsubscribe)
    {
        evntNoti_Initialize();
        evnt_pub publisher;
        evnt_sub subscriber1;
        evnt_sub subscriber2;
        bool success = false;

        publisher = evntNoti_CreatePublisher(NUM_SUPORTED_SUB);
        subscriber1 = evntNoti_SubscribeEvent(publisher, Sub1EventHandler);
        subscriber2 = evntNoti_SubscribeEvent(publisher, Sub2EventHandler);

        success = evntNoti_UnsubscribeEvent(publisher, subscriber1);
        ASSERT_EQ(success, true);
        success = evntNoti_UnsubscribeEvent(publisher, subscriber2);
        ASSERT_EQ(success, true);

        evntNoti_NotifyEnvent(publisher, TEST_EVENT_CODE, NULL, NULL);

        ASSERT_EQ(Sub1EventCount, 0);
        ASSERT_EQ(Sub1EventCode, 0);

        ASSERT_EQ(Sub2EventCount, 0);
        ASSERT_EQ(Sub2EventCode, 0);

    }
}

#endif /* _UNIT_TEST_EVENT_NOTIFIER */
