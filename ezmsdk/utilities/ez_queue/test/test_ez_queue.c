
/*******************************************************************************
* Filename:         test_ez_queue.c
* Author:           Hai Nguyen
* Original Date:    20.08.2022
* Last Update:      20.08.2022
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
* Written by Hai Nguyen 20.08.2022
*
*******************************************************************************/

/** @file   test_ez_queue.c
 *  @author Hai Nguyen
 *  @date   20.08.2022
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
#define MOD_NAME    "test_ez_queue"       /**< module name */
#include "utilities/logging/logging.h"

#include "unity_test_platform/unity.h"
#include "app/app.h"
#include "utilities/ez_queue/ez_queue.h"

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
ezQueue queue;
uint8_t queue_buff[BUFF_SIZE] = { 0 };
uint8_t item_1[3] = { 1, 2, 3 };
uint8_t item_2[6] = { 1, 2, 3 , 4, 5, 6};
uint8_t item_3[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
uint8_t item_4[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };


/******************************************************************************
* Function Definitions
*******************************************************************************/
void test_CreateQueueFail(void);
void test_CreateQueueSuccess(void);
void test_PopEmptyQueue(void);
void test_PushQueueFail(void);
void test_PushQueueSuccess(void);
void test_GetFrontPop(void);
void test_GetBackPop(void);
void test_OverflowQueue(void);


/******************************************************************************
* External functions
*******************************************************************************/


int main(void)
{
    ezmApp_SdkInit();

    UnityBegin("test/test_ez_queue.c");
 
    /* Note, must be called in order */
    RUN_TEST(test_CreateQueueFail);
    RUN_TEST(test_CreateQueueSuccess);
    RUN_TEST(test_PopEmptyQueue);
    RUN_TEST(test_PushQueueFail);
    RUN_TEST(test_PushQueueSuccess);
    RUN_TEST(test_GetFrontPop);
    RUN_TEST(test_GetBackPop);
    RUN_TEST(test_OverflowQueue);

    return (UnityEnd());
}


/******************************************************************************
* Internal functions
*******************************************************************************/
void test_CreateQueueFail(void)
{
    ezSTATUS status = ezSUCCESS;
    status = ezQueue_CreateQueue(NULL, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezQueue_CreateQueue(&queue, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezQueue_CreateQueue(&queue, queue_buff, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);
}


void test_CreateQueueSuccess(void)
{
    ezSTATUS status = ezSUCCESS;
    status = ezQueue_CreateQueue(&queue, queue_buff, BUFF_SIZE);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
}


void test_PopEmptyQueue(void)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t queue_size = 0U;
    uint8_t* test_data = NULL;
    uint32_t test_data_size = 0U;

    status = ezQueue_GetFront(&queue, &test_data, &test_data_size);
    TEST_ASSERT_EQUAL(ezFAIL, status);
    TEST_ASSERT_EQUAL(0, test_data_size);
}


void test_PushQueueFail(void)
{
    ezSTATUS status = ezSUCCESS;
    status = ezQueue_Push(NULL, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezQueue_Push(&queue, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezQueue_Push(&queue, item_1, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);
}


void test_PushQueueSuccess(void)
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


void test_GetFrontPop(void)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t queue_size = 0U;
    uint8_t *test_data = NULL;
    uint32_t test_data_size = 0U;

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


void test_GetBackPop(void)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t queue_size = 0U;
    uint8_t* test_data = NULL;
    uint32_t test_data_size = 0U;

    /* re init queue again */
    status = ezQueue_CreateQueue(&queue, queue_buff, sizeof(queue_buff));
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

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


void test_OverflowQueue(void)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t queue_size = 0U;

    uint8_t overflow_buff[200] = {0};

    status = ezQueue_CreateQueue(&queue, queue_buff, sizeof(queue_buff));
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezQueue_Push(&queue, overflow_buff, sizeof(overflow_buff));
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(1, queue_size);

    status = ezQueue_Push(&queue, overflow_buff, sizeof(overflow_buff));
    TEST_ASSERT_EQUAL(ezFAIL, status);

    queue_size = ezQueue_GetNumOfElement(&queue);
    TEST_ASSERT_EQUAL(1, queue_size);
}

#endif /* CONFIG_EZ_QUEUE_TEST */

/* End of file*/

