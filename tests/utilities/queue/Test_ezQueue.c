
/*******************************************************************************
* Filename:         ezQueue.c
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

/** @file   ezQueue.c
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
#include "ezApp/ezSdk_config.h"

#if (EZ_QUEUE == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ezQueue"       /**< module name */
#include "utilities/logging/ez_logging.h"
#include "unity.h"
#include "unity_fixture.h"
#include "utilities/queue/ez_queue.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

TEST_GROUP(ezQueue);


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define BUFF_SIZE     256   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static ezQueue queue;
static uint8_t queue_buff[BUFF_SIZE] = { 0 };
static uint8_t item_1[3] = { 1, 2, 3 };
static uint8_t item_2[6] = { 1, 2, 3 , 4, 5, 6 };
static uint8_t item_3[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
static uint8_t item_4[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

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


TEST_SETUP(ezQueue)
{
    memset(queue_buff, 0, BUFF_SIZE);
    ezQueue_CreateQueue(&queue, queue_buff, BUFF_SIZE);
}


TEST_TEAR_DOWN(ezQueue)
{
}


TEST(ezQueue, CreateQueueFail)
{
    ezSTATUS status = ezSUCCESS;
    status = ezQueue_CreateQueue(NULL, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezQueue_CreateQueue(&queue, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezQueue_CreateQueue(&queue, queue_buff, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);
}


TEST(ezQueue, CreateQueueSuccess)
{
    ezSTATUS status = ezSUCCESS;
    status = ezQueue_CreateQueue(&queue, queue_buff, BUFF_SIZE);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
}


TEST(ezQueue, PopEmptyQueue)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t queue_size = 0U;
    uint8_t *test_data = NULL;
    uint32_t test_data_size = 0U;

    status = ezQueue_GetFront(&queue, &test_data, &test_data_size);
    TEST_ASSERT_EQUAL(ezFAIL, status);
    TEST_ASSERT_EQUAL(0, test_data_size);
}


TEST(ezQueue, PushQueueFail)
{
    ezSTATUS status = ezSUCCESS;
    status = ezQueue_Push(NULL, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezQueue_Push(&queue, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezQueue_Push(&queue, item_1, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);
}


TEST(ezQueue, PushQueueSuccess)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t queue_size = 0U;

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(queue_size, 0);

    status = ezQueue_Push(&queue, item_1, sizeof(item_1));
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezQueue_Push(&queue, item_2, sizeof(item_2));
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezQueue_Push(&queue, item_3, sizeof(item_3));
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezQueue_Push(&queue, item_4, sizeof(item_4));
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(queue_size, 4);
}


TEST(ezQueue, test_GetFrontPop)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t queue_size = 0U;
    uint8_t *test_data = NULL;
    uint32_t test_data_size = 0U;

    status = ezQueue_Push(&queue, item_1, sizeof(item_1));
    status = ezQueue_Push(&queue, item_2, sizeof(item_2));
    status = ezQueue_Push(&queue, item_3, sizeof(item_3));
    status = ezQueue_Push(&queue, item_4, sizeof(item_4));
    queue_size = ezQueue_GetNumOfElement(&queue);

    /* first item */
    status = ezQueue_GetFront(&queue, &test_data, &test_data_size);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
    TEST_ASSERT_EQUAL(sizeof(item_1), test_data_size);
    TEST_ASSERT_EQUAL_MEMORY(item_1, test_data, sizeof(item_1));

    status = ezQueue_PopFront(&queue);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(queue_size, 3);

    /* second item */
    status = ezQueue_GetFront(&queue, &test_data, &test_data_size);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
    TEST_ASSERT_EQUAL(sizeof(item_2), test_data_size);
    TEST_ASSERT_EQUAL_MEMORY(item_2, test_data, sizeof(item_2));

    status = ezQueue_PopFront(&queue);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(queue_size, 2);

    /* third item */
    status = ezQueue_GetFront(&queue, &test_data, &test_data_size);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
    TEST_ASSERT_EQUAL(sizeof(item_3), test_data_size);
    TEST_ASSERT_EQUAL_MEMORY(item_3, test_data, sizeof(item_3));

    status = ezQueue_PopFront(&queue);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(queue_size, 1);

    /* last item */
    status = ezQueue_GetFront(&queue, &test_data, &test_data_size);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
    TEST_ASSERT_EQUAL(sizeof(item_4), test_data_size);
    TEST_ASSERT_EQUAL_MEMORY(item_4, test_data, sizeof(item_4));

    status = ezQueue_PopFront(&queue);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(queue_size, 0);

    /* Sanity check*/
    status = ezQueue_GetFront(&queue, &test_data, &test_data_size);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(queue_size, 0);
}


TEST(ezQueue, GetBackPop)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t queue_size = 0U;
    uint8_t *test_data = NULL;
    uint32_t test_data_size = 0U;

    if (status == ezSUCCESS)
    {
        status = ezQueue_Push(&queue, item_1, sizeof(item_1));
        status = ezQueue_Push(&queue, item_2, sizeof(item_2));
        status = ezQueue_Push(&queue, item_3, sizeof(item_3));
        status = ezQueue_Push(&queue, item_4, sizeof(item_4));
        queue_size = ezQueue_GetNumOfElement(&queue);

        TEST_ASSERT_EQUAL(queue_size, 4);
    }

    if (status == ezSUCCESS)
    {
        /* first item */
        status = ezQueue_GetBack(&queue, &test_data, &test_data_size);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);
        TEST_ASSERT_EQUAL(sizeof(item_4), test_data_size);
        TEST_ASSERT_EQUAL_MEMORY(item_4, test_data, sizeof(item_4));

        status = ezQueue_PopBack(&queue);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);

        queue_size = ezQueue_GetNumOfElement(&queue);
        TEST_ASSERT_EQUAL(queue_size, 3);

        /* second item */
        status = ezQueue_GetBack(&queue, &test_data, &test_data_size);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);
        TEST_ASSERT_EQUAL(sizeof(item_3), test_data_size);
        TEST_ASSERT_EQUAL_MEMORY(item_3, test_data, sizeof(item_3));

        status = ezQueue_PopBack(&queue);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);

        queue_size = ezQueue_GetNumOfElement(&queue);
        TEST_ASSERT_EQUAL(queue_size, 2);

        /* third item */
        status = ezQueue_GetBack(&queue, &test_data, &test_data_size);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);
        TEST_ASSERT_EQUAL(sizeof(item_2), test_data_size);
        TEST_ASSERT_EQUAL_MEMORY(item_2, test_data, sizeof(item_2));

        status = ezQueue_PopBack(&queue);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);

        queue_size = ezQueue_GetNumOfElement(&queue);
        TEST_ASSERT_EQUAL(queue_size, 1);

        /* last item */
        status = ezQueue_GetBack(&queue, &test_data, &test_data_size);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);
        TEST_ASSERT_EQUAL(sizeof(item_1), test_data_size);
        TEST_ASSERT_EQUAL_MEMORY(item_1, test_data, sizeof(item_1));

        status = ezQueue_PopBack(&queue);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);

        queue_size = ezQueue_GetNumOfElement(&queue);
        TEST_ASSERT_EQUAL(queue_size, 0);

        /* Sanity check*/
        status = ezQueue_GetBack(&queue, &test_data, &test_data_size);
        TEST_ASSERT_EQUAL(ezFAIL, status);

        queue_size = ezQueue_GetNumOfElement(&queue);
        TEST_ASSERT_EQUAL(queue_size, 0);
    }
}


TEST(ezQueue, OverflowQueue)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t queue_size = 0U;
    uint8_t overflow_buff[200] = { 0 };

    status = ezQueue_Push(&queue, overflow_buff, sizeof(overflow_buff));
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(1, queue_size);

    status = ezQueue_Push(&queue, overflow_buff, sizeof(overflow_buff));
    TEST_ASSERT_EQUAL(ezFAIL, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(1, queue_size);
}


TEST(ezQueue, ezQueue_ReserveElement)
{
    ezSTATUS status = ezSUCCESS;

    struct TestStruct
    {
        uint32_t a;
        uint8_t b;
        uint32_t c;
    };

    struct TestStruct test_struct = { 0 };
    ezReservedElement elem1 = NULL;
    ezReservedElement elem2 = NULL;
    struct TestStruct *to_be_tested_struct = NULL;
    uint8_t *to_be_tested_stream = NULL;

    test_struct.a = 0xbeefcafe;
    test_struct.b = 0xaa;
    test_struct.c = 0xdeadbeef;

    uint8_t byte_stream[] = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };


    elem1 = ezQueue_ReserveElement(&queue, (void *)&to_be_tested_struct, sizeof(struct TestStruct));
    TEST_ASSERT_NOT_NULL(elem1);
    TEST_ASSERT_EQUAL(0, ezQueue_GetNumOfElement(&queue));

    to_be_tested_struct->a = 0xbeefcafe;
    to_be_tested_struct->b = 0xaa;
    to_be_tested_struct->c = 0xdeadbeef;

    status = ezQueue_PushReservedElement(&queue, elem1);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    elem2 = ezQueue_ReserveElement(&queue, (void *)&to_be_tested_stream, sizeof(byte_stream));
    TEST_ASSERT_NOT_NULL(elem2);
    TEST_ASSERT_EQUAL(1, ezQueue_GetNumOfElement(&queue));

    *(to_be_tested_stream) = 0xaa;
    to_be_tested_stream++;
    *(to_be_tested_stream) = 0xbb;
    to_be_tested_stream++;
    *(to_be_tested_stream) = 0xcc;
    to_be_tested_stream++;
    *(to_be_tested_stream) = 0xdd;
    to_be_tested_stream++;
    *(to_be_tested_stream) = 0xee;
    to_be_tested_stream++;
    *(to_be_tested_stream) = 0xff;

    status = ezQueue_PushReservedElement(&queue, elem2);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
    TEST_ASSERT_EQUAL(2, ezQueue_GetNumOfElement(&queue));

    to_be_tested_struct = NULL;
    to_be_tested_stream = NULL;
    uint32_t data_size = 0;

    status = ezQueue_GetFront(&queue, (void *)&to_be_tested_struct, &data_size);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
    TEST_ASSERT_EQUAL(sizeof(struct TestStruct), data_size);
    TEST_ASSERT_EQUAL_MEMORY(&test_struct, to_be_tested_struct, sizeof(struct TestStruct));

    status = ezQueue_PopFront(&queue);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
    TEST_ASSERT_EQUAL(1, ezQueue_GetNumOfElement(&queue));

    status = ezQueue_GetFront(&queue, (void *)&to_be_tested_stream, &data_size);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
    TEST_ASSERT_EQUAL(sizeof(byte_stream), data_size);
    TEST_ASSERT_EQUAL_MEMORY(byte_stream, to_be_tested_stream, sizeof(byte_stream));

    status = ezQueue_PopFront(&queue);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
    TEST_ASSERT_EQUAL(0, ezQueue_GetNumOfElement(&queue));
}

#endif /* EZ_QUEUE == 1U */

/* End of file */

