#include <limits.h>
#include "gtest/gtest.h"

#ifndef UNIT_TEST_RINGBUFFER
#define UNIT_TEST_RINGBUFFER


extern "C" {
#include "../../ezmsdk/app/app_config.h"
#include "../../ezmsdk/helper/ring_buffer/ring_buffer.h"
#define BUFF_SIZE 10
    uint8_t au8TestBuffer[BUFF_SIZE] = { 0 };
    RingBuffer r_buff;
}

namespace
{
#if (CONFIG_RING_BUFFER == 1U)

    TEST(Ring_Buffer, Init)
    {
        bool status;
        status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
        ASSERT_EQ(status, true);
        ASSERT_EQ(r_buff.u16Capacity, BUFF_SIZE);
        status = ezmRingBuffer_IsEmpty(&r_buff);
        ASSERT_EQ(status, true);
    }

    TEST(Ring_Buffer, Push)
    {
        bool status;
        uint16_t u16Bytes;
        uint8_t push_10[BUFF_SIZE] = { 0,1,2,3,4,5,6,7,8,9 };
        status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
        ASSERT_EQ(status, true);

        u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 10);
        ASSERT_EQ(u16Bytes, BUFF_SIZE);
        status = ezmRingBuffer_IsFull(&r_buff);
        ASSERT_EQ(status, true);

        for (uint8_t i = 0; i < 10; i++)
        {
            ASSERT_EQ(r_buff.pu8Buff[i], i);
        }

        /*Push another round to see if we get over flow error*/ 
        status = ezmRingBuffer_Push(&r_buff, push_10, 10);
        ASSERT_EQ(u16Bytes, BUFF_SIZE);

        /*see which memory we have left, must be 0 since buff is full*/
        uint16_t avai_mem = 10;
        avai_mem = ezmRingBuffer_GetAvailableMemory(&r_buff);
        ASSERT_EQ(avai_mem, 0);

        /*Push 0 byte, should be OKr*/
        u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 0);
        ASSERT_EQ(u16Bytes, 0);
    }

    TEST(Ring_Buffer, Pop)
    {
        bool status;
        uint16_t u16Bytes;
        uint8_t push_10[BUFF_SIZE] = { 0,1,2,3,4,5,6,7,8,9 };
        uint8_t pop_5[5];
        status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
        ASSERT_EQ(status, true);

        u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, BUFF_SIZE);
        ASSERT_EQ(u16Bytes, BUFF_SIZE);

        /*pop out the first 5*/
        u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_5, 5);
        ASSERT_EQ(u16Bytes, 5);

        for (uint8_t i = 0; i < 5; i++)
        {
            ASSERT_EQ(pop_5[i], i);
        }

        uint16_t avail_mem = 0;
        avail_mem = ezmRingBuffer_GetAvailableMemory(&r_buff);
        ASSERT_EQ(avail_mem, 5);

        /*Get the rest 5 byte*/
        u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_5, 5);
        ASSERT_EQ(u16Bytes, 5);

        for (uint8_t i = 0; i < 5; i++)
        {
            ASSERT_EQ(pop_5[i], i + 5);
        }

        status = ezmRingBuffer_IsEmpty(&r_buff);
        ASSERT_EQ(status, true);

        avail_mem = ezmRingBuffer_GetAvailableMemory(&r_buff);
        ASSERT_EQ(avail_mem, 10);

        /*Get the pop another  5 byte*/
        u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_5, 5);
        ASSERT_EQ(u16Bytes, 0);
    }

    TEST(Ring_Buffer, wrapping_point_push)
    {
        bool status;
        uint16_t u16Bytes;
        uint8_t push_10[BUFF_SIZE] = { 0,1,2,3,4,5,6,7,8,9 };
        uint8_t pop_5[5];
        status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
        ASSERT_EQ(status, true);

        u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 7);
        ASSERT_EQ(u16Bytes, 7);

        u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_5, 2);
        ASSERT_EQ(u16Bytes, 2);

        u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 5);
        ASSERT_EQ(u16Bytes, 5);

        ASSERT_EQ(au8TestBuffer[0], 3);
        ASSERT_EQ(au8TestBuffer[1], 4);
        ASSERT_EQ(au8TestBuffer[2], 2);
        ASSERT_EQ(au8TestBuffer[3], 3);
        ASSERT_EQ(au8TestBuffer[4], 4);
        ASSERT_EQ(au8TestBuffer[5], 5);
        ASSERT_EQ(au8TestBuffer[6], 6);
        ASSERT_EQ(au8TestBuffer[7], 0);
        ASSERT_EQ(au8TestBuffer[8], 1);
        ASSERT_EQ(au8TestBuffer[9], 2);
    }

    TEST(Ring_Buffer, wrapping_point_pop)
    {
        bool status;
        uint16_t u16Bytes;
        uint8_t push_10[BUFF_SIZE] = { 0,1,2,3,4,5,6,7,8,9 };
        uint8_t pop_10[10];
        status = ezmRingBuffer_Init(&r_buff, au8TestBuffer, BUFF_SIZE);
        ASSERT_EQ(status, true);

        u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 7);
        ASSERT_EQ(u16Bytes, 7);

        u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_10, 2);
        ASSERT_EQ(u16Bytes, 2);

        u16Bytes = ezmRingBuffer_Push(&r_buff, push_10, 5);
        ASSERT_EQ(u16Bytes, 5);

        ASSERT_EQ(au8TestBuffer[0], 3);
        ASSERT_EQ(au8TestBuffer[1], 4);
        ASSERT_EQ(au8TestBuffer[2], 2);
        ASSERT_EQ(au8TestBuffer[3], 3);
        ASSERT_EQ(au8TestBuffer[4], 4);
        ASSERT_EQ(au8TestBuffer[5], 5);
        ASSERT_EQ(au8TestBuffer[6], 6);
        ASSERT_EQ(au8TestBuffer[7], 0);
        ASSERT_EQ(au8TestBuffer[8], 1);
        ASSERT_EQ(au8TestBuffer[9], 2);

        u16Bytes = ezmRingBuffer_Pop(&r_buff, pop_10, 10);
        ASSERT_EQ(u16Bytes, 10);

        ASSERT_EQ(pop_10[0], 2);
        ASSERT_EQ(pop_10[1], 3);
        ASSERT_EQ(pop_10[2], 4);
        ASSERT_EQ(pop_10[3], 5);
        ASSERT_EQ(pop_10[4], 6);
        ASSERT_EQ(pop_10[5], 0);
        ASSERT_EQ(pop_10[6], 1);
        ASSERT_EQ(pop_10[7], 2);
        ASSERT_EQ(pop_10[8], 3);
        ASSERT_EQ(pop_10[9], 4);
    }
#endif /* CONFIG_RING_BUFFER */
#endif /* UNIT_TEST_RINGBUFFER */
}
