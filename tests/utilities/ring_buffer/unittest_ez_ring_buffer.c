/*****************************************************************************
* Filename:         unittest_ez_ring_buffer.c
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

/** @file   unittest_ez_ring_buffer.c
 *  @author Hai Nguyen
 *  @date   07.03.2024
 *  @brief  Unit test for ring buffer module
 *
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "unity.h"
#include "unity_fixture.h"
#include "ez_ring_buffer.h"

TEST_GROUP(ez_ring_buffer);

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define BUFF_SIZE 10


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint8_t au8TestBuffer[BUFF_SIZE] = { 0 };
static RingBuffer r_buff;


/******************************************************************************
* Function Definitions
*******************************************************************************/
static void RunAllTests(void);

/******************************************************************************
* External functions
*******************************************************************************/
int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}


TEST_SETUP(ez_ring_buffer)
{
}


TEST_TEAR_DOWN(ez_ring_buffer)
{
}


TEST_GROUP_RUNNER(ez_ring_buffer)
{
    RUN_TEST_CASE(ez_ring_buffer, Init);
    RUN_TEST_CASE(ez_ring_buffer, Push);
    RUN_TEST_CASE(ez_ring_buffer, Pop);
    RUN_TEST_CASE(ez_ring_buffer, wrapping_point_push);
    RUN_TEST_CASE(ez_ring_buffer, wrapping_point_pop);
}


TEST(ez_ring_buffer, Init)
{
    bool status;
    status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
    TEST_ASSERT_EQUAL(status, true);
    TEST_ASSERT_EQUAL(r_buff.u16Capacity, BUFF_SIZE);
    status = ezmRingBuffer_IsEmpty(&r_buff);
    TEST_ASSERT_EQUAL(status, true);
}

TEST(ez_ring_buffer, Push)
{
    bool status;
    uint16_t u16Bytes;
    uint8_t push_10[BUFF_SIZE] = { 0,1,2,3,4,5,6,7,8,9 };
    status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
    TEST_ASSERT_EQUAL(status, true);

    u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 10);
    TEST_ASSERT_EQUAL(u16Bytes, BUFF_SIZE);
    status = ezmRingBuffer_IsFull(&r_buff);
    TEST_ASSERT_EQUAL(status, true);

    for (uint8_t i = 0; i < 10; i++)
    {
        TEST_ASSERT_EQUAL(r_buff.pu8Buff[i], i);
    }

    /*Push another round to see if we get over flow error*/ 
    status = ezmRingBuffer_Push(&r_buff, push_10, 10);
    TEST_ASSERT_EQUAL(u16Bytes, BUFF_SIZE);

    /*see which memory we have left, must be 0 since buff is full*/
    uint16_t avai_mem = 10;
    avai_mem = ezmRingBuffer_GetAvailableMemory(&r_buff);
    TEST_ASSERT_EQUAL(avai_mem, 0);

    /*Push 0 byte, should be OKr*/
    u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 0);
    TEST_ASSERT_EQUAL(u16Bytes, 0);
}

TEST(ez_ring_buffer, Pop)
{
    bool status;
    uint16_t u16Bytes;
    uint8_t push_10[BUFF_SIZE] = { 0,1,2,3,4,5,6,7,8,9 };
    uint8_t pop_5[5];
    status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
    TEST_ASSERT_EQUAL(status, true);

    u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, BUFF_SIZE);
    TEST_ASSERT_EQUAL(u16Bytes, BUFF_SIZE);

    /*pop out the first 5*/
    u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_5, 5);
    TEST_ASSERT_EQUAL(u16Bytes, 5);

    for (uint8_t i = 0; i < 5; i++)
    {
        TEST_ASSERT_EQUAL(pop_5[i], i);
    }

    uint16_t avail_mem = 0;
    avail_mem = ezmRingBuffer_GetAvailableMemory(&r_buff);
    TEST_ASSERT_EQUAL(avail_mem, 5);

    /*Get the rest 5 byte*/
    u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_5, 5);
    TEST_ASSERT_EQUAL(u16Bytes, 5);

    for (uint8_t i = 0; i < 5; i++)
    {
        TEST_ASSERT_EQUAL(pop_5[i], i + 5);
    }

    status = ezmRingBuffer_IsEmpty(&r_buff);
    TEST_ASSERT_EQUAL(status, true);

    avail_mem = ezmRingBuffer_GetAvailableMemory(&r_buff);
    TEST_ASSERT_EQUAL(avail_mem, 10);

    /*Get the pop another  5 byte*/
    u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_5, 5);
    TEST_ASSERT_EQUAL(u16Bytes, 0);
}

TEST(ez_ring_buffer, wrapping_point_push)
{
    bool status;
    uint16_t u16Bytes;
    uint8_t push_10[BUFF_SIZE] = { 0,1,2,3,4,5,6,7,8,9 };
    uint8_t pop_5[5];
    status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
    TEST_ASSERT_EQUAL(status, true);

    u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 7);
    TEST_ASSERT_EQUAL(u16Bytes, 7);

    u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_5, 2);
    TEST_ASSERT_EQUAL(u16Bytes, 2);

    u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 5);
    TEST_ASSERT_EQUAL(u16Bytes, 5);

    TEST_ASSERT_EQUAL(au8TestBuffer[0], 3);
    TEST_ASSERT_EQUAL(au8TestBuffer[1], 4);
    TEST_ASSERT_EQUAL(au8TestBuffer[2], 2);
    TEST_ASSERT_EQUAL(au8TestBuffer[3], 3);
    TEST_ASSERT_EQUAL(au8TestBuffer[4], 4);
    TEST_ASSERT_EQUAL(au8TestBuffer[5], 5);
    TEST_ASSERT_EQUAL(au8TestBuffer[6], 6);
    TEST_ASSERT_EQUAL(au8TestBuffer[7], 0);
    TEST_ASSERT_EQUAL(au8TestBuffer[8], 1);
    TEST_ASSERT_EQUAL(au8TestBuffer[9], 2);
}

TEST(ez_ring_buffer, wrapping_point_pop)
{
    bool status;
    uint16_t u16Bytes;
    uint8_t push_10[BUFF_SIZE] = { 0,1,2,3,4,5,6,7,8,9 };
    uint8_t pop_10[10];
    status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
    TEST_ASSERT_EQUAL(status, true);

    u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 7);
    TEST_ASSERT_EQUAL(u16Bytes, 7);

    u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_10, 2);
    TEST_ASSERT_EQUAL(u16Bytes, 2);

    u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 5);
    TEST_ASSERT_EQUAL(u16Bytes, 5);

    TEST_ASSERT_EQUAL(au8TestBuffer[0], 3);
    TEST_ASSERT_EQUAL(au8TestBuffer[1], 4);
    TEST_ASSERT_EQUAL(au8TestBuffer[2], 2);
    TEST_ASSERT_EQUAL(au8TestBuffer[3], 3);
    TEST_ASSERT_EQUAL(au8TestBuffer[4], 4);
    TEST_ASSERT_EQUAL(au8TestBuffer[5], 5);
    TEST_ASSERT_EQUAL(au8TestBuffer[6], 6);
    TEST_ASSERT_EQUAL(au8TestBuffer[7], 0);
    TEST_ASSERT_EQUAL(au8TestBuffer[8], 1);
    TEST_ASSERT_EQUAL(au8TestBuffer[9], 2);

    u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_10, 10);
    TEST_ASSERT_EQUAL(u16Bytes, 10);

    TEST_ASSERT_EQUAL(pop_10[0], 2);
    TEST_ASSERT_EQUAL(pop_10[1], 3);
    TEST_ASSERT_EQUAL(pop_10[2], 4);
    TEST_ASSERT_EQUAL(pop_10[3], 5);
    TEST_ASSERT_EQUAL(pop_10[4], 6);
    TEST_ASSERT_EQUAL(pop_10[5], 0);
    TEST_ASSERT_EQUAL(pop_10[6], 1);
    TEST_ASSERT_EQUAL(pop_10[7], 2);
    TEST_ASSERT_EQUAL(pop_10[8], 3);
    TEST_ASSERT_EQUAL(pop_10[9], 4);
}


/******************************************************************************
* Internal functions
*******************************************************************************/
static void RunAllTests(void)
{
    RUN_TEST_GROUP(ez_ring_buffer);
}


/* End of file */
