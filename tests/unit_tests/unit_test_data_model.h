
/*******************************************************************************
* Filename:         unit_test_data_model.h
* Author:           Hai Nguyen
* Original Date:    19.06.2022
* Last Update:      19.06.2022
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
* Written by Hai Nguyen 19.06.2022
*
*******************************************************************************/

#ifndef _UNIT_TEST_DATA_MODEL
#define _UNIT_TEST_DATA_MODEL

#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#if(TEST_DATA_MODEL == 1U)
#include "data_model/data_model.h"
#include "utilities/logging/logging.h"
    uint32_t test_data_1 = 0;
    char test_data_2[16] = { 0 };

    int DataPointModifyCallback(uint32_t event_code, void* param1, void* param2)
    {
        switch ((DATA_POINT_EVENT)event_code)
        {
        case DATA_MODIFY:
            dbg_print("[address = %p], [size = %d]\n", param1, *(uint32_t*)param2);
            break;
        default:
            break;
        }
        return 0;
    }
#endif
}

namespace 
{
#if(TEST_DATA_MODEL == 1U)
    TEST(test_case_name, test_name) 
    {
        DataModel_Initialization();

        bool is_success = true;

        DataPoint data_point_1;
        DataPoint data_point_2;

        event_observer obs1;
        event_observer obs2;

        uint32_t helper_1;
        char helper_2[16] = { 0 };

        data_point_1 = DataModel_CreateDataPoint((void*)&test_data_1, sizeof(test_data_1));
        ASSERT_NE(data_point_1, DATA_POINT_INVALID);

        data_point_2 = DataModel_CreateDataPoint((void*)test_data_2, sizeof(test_data_2));
        ASSERT_NE(data_point_2, DATA_POINT_INVALID);

        evntNoti_CreateObserver(&obs1, DataPointModifyCallback);
        evntNoti_CreateObserver(&obs2, DataPointModifyCallback);
        DataModel_SubscribeDataPointEvent(data_point_1, &obs1);
        DataModel_SubscribeDataPointEvent(data_point_2, &obs2);

        helper_1 = 0xFF;
        is_success = DataModel_WriteDataPoint(data_point_1, (void*)&helper_1, sizeof(helper_1));
        ASSERT_EQ(is_success, true);
        ASSERT_EQ(test_data_1, helper_1);

        is_success = DataModel_WriteDataPoint(data_point_1, (void*)helper_2, sizeof(helper_2));
        ASSERT_EQ(is_success, false);

        helper_1 = 0x0;
        is_success = DataModel_ReadDataPoint(data_point_1, (void*)&helper_1, sizeof(helper_1));
        ASSERT_EQ(is_success, true);
        ASSERT_EQ(test_data_1, helper_1);

        memset(helper_2, 0xAA, sizeof(helper_2));
        is_success = DataModel_WriteDataPoint(data_point_2, (void*)helper_2, sizeof(helper_2));
        ASSERT_EQ(is_success, true);
        ASSERT_EQ(memcmp(test_data_2, helper_2, sizeof(helper_2)), 0);

        memset(helper_2, 0x00, sizeof(helper_2));
        is_success = DataModel_ReadDataPoint(data_point_2, (void*)helper_2, sizeof(helper_2));
        ASSERT_EQ(is_success, true);
        ASSERT_EQ(memcmp(test_data_2, helper_2, sizeof(helper_2)), 0);
    }
#endif
}

#endif /* _UNIT_TEST_DATA_MODEL */
