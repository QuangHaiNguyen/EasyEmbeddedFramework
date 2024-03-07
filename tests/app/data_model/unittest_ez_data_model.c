/*****************************************************************************
* Filename:         unittest_ez_data_model.c
* Author:           Hai Nguyen
* Original Date:    07.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   unittest_ez_data_model.c
 *  @author Hai Nguyen
 *  @date   07.03.2024
 *  @brief  Unit test for data model component
 *
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "unity.h"
#include "unity_fixture.h"
#include "ez_data_model.h"
#include "ez_event_notifier.h"

TEST_GROUP(ez_data_model);

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
static uint32_t test_data_1 = 0;
static char test_data_2[16] = { 0 };


/******************************************************************************
* Function Definitions
*******************************************************************************/
static void RunAllTests(void);
int DataPointModifyCallback(uint32_t event_code, void* param1, void* param2);


/******************************************************************************
* External functions
*******************************************************************************/
int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}


TEST_SETUP(ez_data_model)
{
    DataModel_Initialization();
}


TEST_TEAR_DOWN(ez_data_model)
{
}


TEST_GROUP_RUNNER(ez_data_model)
{
    RUN_TEST_CASE(ez_data_model, simple_read_write_test);
}


TEST(ez_data_model, simple_read_write_test) 
{
    bool is_success = true;

    DataPoint data_point_1;
    DataPoint data_point_2;

    ezObserver obs1;
    ezObserver obs2;

    uint32_t helper_1;
    char helper_2[16] = { 0 };

    data_point_1 = DataModel_CreateDataPoint((void*)&test_data_1, sizeof(test_data_1));
    TEST_ASSERT_NOT_EQUAL(data_point_1, DATA_POINT_INVALID);

    data_point_2 = DataModel_CreateDataPoint((void*)test_data_2, sizeof(test_data_2));
    TEST_ASSERT_NOT_EQUAL(data_point_2, DATA_POINT_INVALID);

    ezEventNotifier_CreateObserver(&obs1, DataPointModifyCallback);
    ezEventNotifier_CreateObserver(&obs2, DataPointModifyCallback);
    DataModel_SubscribeDataPointEvent(data_point_1, &obs1);
    DataModel_SubscribeDataPointEvent(data_point_2, &obs2);

    helper_1 = 0xFF;
    is_success = DataModel_WriteDataPoint(data_point_1, (void*)&helper_1, sizeof(helper_1));
    TEST_ASSERT_EQUAL(is_success, true);
    TEST_ASSERT_EQUAL(test_data_1, helper_1);

    is_success = DataModel_WriteDataPoint(data_point_1, (void*)helper_2, sizeof(helper_2));
    TEST_ASSERT_EQUAL(is_success, false);

    helper_1 = 0x0;
    is_success = DataModel_ReadDataPoint(data_point_1, (void*)&helper_1, sizeof(helper_1));
    TEST_ASSERT_EQUAL(is_success, true);
    TEST_ASSERT_EQUAL(test_data_1, helper_1);

    memset(helper_2, 0xAA, sizeof(helper_2));
    is_success = DataModel_WriteDataPoint(data_point_2, (void*)helper_2, sizeof(helper_2));
    TEST_ASSERT_EQUAL(is_success, true);
    TEST_ASSERT_EQUAL(memcmp(test_data_2, helper_2, sizeof(helper_2)), 0);

    memset(helper_2, 0x00, sizeof(helper_2));
    is_success = DataModel_ReadDataPoint(data_point_2, (void*)helper_2, sizeof(helper_2));
    TEST_ASSERT_EQUAL(is_success, true);
    TEST_ASSERT_EQUAL(memcmp(test_data_2, helper_2, sizeof(helper_2)), 0);
}


/******************************************************************************
* Internal functions
*******************************************************************************/
static void RunAllTests(void)
{
    RUN_TEST_GROUP(ez_data_model);
}


int DataPointModifyCallback(uint32_t event_code, void* param1, void* param2)
{
    switch ((DATA_POINT_EVENT)event_code)
    {
    case DATA_MODIFY:
        break;
    default:
        break;
    }
    return 0;
}


/* End of file */
