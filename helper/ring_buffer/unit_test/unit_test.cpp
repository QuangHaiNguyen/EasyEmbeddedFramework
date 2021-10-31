#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "../../../app/app_config.h"
}

namespace 
{
#if (RING_BUFFER == 1U)
    #define BUFF_SIZE 10
    RingBuffer r_buff;
    RingBuff_Status status;

#if (STATIC_MEM == 0)
    TEST(Ring_Buffer, Init) 
    {
        status = ezmRingBuffer_Init(&r_buff, BUFF_SIZE);
        ASSERT_EQ(status, BUFF_OK);
        ASSERT_EQ(r_buff.u16Capacity, BUFF_SIZE);
        status = ezmRingBuffer_IsEmpty(&r_buff);
        ASSERT_EQ(status, BUFF_EMPTY);
    }

    TEST(Ring_Buffer, Push) 
    {
        uint8_t push_10[10] = {0,1,2,3,4,5,6,7,8,9};
        status = ezmRingBuffer_Init(&r_buff, BUFF_SIZE);
        ASSERT_EQ(status, BUFF_OK);
        
        status = ezmRingBuffer_Push(&r_buff, push_10, 10);
        ASSERT_EQ(status, BUFF_OK);
        status = ezmRingBuffer_IsFull(&r_buff);
        ASSERT_EQ(status, BUFF_FULL);

        for(uint8_t i = 0; i < 10; i++)
        {
            ASSERT_EQ(r_buff.pu8Buff[i], i);
        }

        /*Push another round to see if we get over flow error*/
        status = ezmRingBuffer_Push(&r_buff, push_10, 10);
        ASSERT_EQ(status, BUFF_NO_MEMORY);

        /*see which memory we have left, must be 0 since buff is full*/
        uint16_t avai_mem = 10;
        status = ezmRingBuffer_GetAvailableMemory(&r_buff, &avai_mem);
        ASSERT_EQ(avai_mem, 0);

        /*Push 0 byte, should be OKr*/
        status = ezmRingBuffer_Push(&r_buff, push_10, 0);
        ASSERT_EQ(status, BUFF_OK);
    }

    TEST(Ring_Buffer, Pop) 
    {
        uint8_t push_10[10] = {0,1,2,3,4,5,6,7,8,9};
        uint8_t pop_5[5];
        status = ezmRingBuffer_Init(&r_buff, BUFF_SIZE);
        ASSERT_EQ(status, BUFF_OK);
        
        status = ezmRingBuffer_Push(&r_buff, push_10, 10);
        ASSERT_EQ(status, BUFF_OK);

        /*pop out the first 5*/
        status = ezmRingBuffer_Pop(&r_buff, pop_5, 5);
        ASSERT_EQ(status, BUFF_OK);

        for(uint8_t i = 0; i < 5; i++)
        {
            ASSERT_EQ(pop_5[i], i);
        }

        uint16_t avail_mem = 0;
        status = ezmRingBuffer_GetAvailableMemory(&r_buff, &avail_mem);
        ASSERT_EQ(avail_mem, 5);

        /*Get the rest 5 byte*/
        status = ezmRingBuffer_Pop(&r_buff, pop_5, 5);
        ASSERT_EQ(status, BUFF_OK);

        for(uint8_t i = 0; i < 5; i++)
        {
            ASSERT_EQ(pop_5[i], i + 5);
        }

        status = ezmRingBuffer_IsEmpty(&r_buff);
        ASSERT_EQ(status, BUFF_EMPTY);

        status = ezmRingBuffer_GetAvailableMemory(&r_buff, &avail_mem);
        ASSERT_EQ(avail_mem, 10);

        /*Get the pop another  5 byte*/
        status = ezmRingBuffer_Pop(&r_buff, pop_5, 5);
        ASSERT_EQ(status, BUFF_NO_MEMORY);
    }
#else
    TEST(Ring_Buffer, STATIC_MEMORY) 
    {
        uint8_t push_512[STATIC_MEM_SIZE];
        uint8_t pop_128[128];
        
        for(uint16_t i = 0; i < 512; i++)
        {
            push_512[i] = (uint8_t)(i & 0xFF);
        }

        
        status = ezmRingBuffer_Init(&r_buff, BUFF_SIZE);
        ASSERT_EQ(status, BUFF_OK);
        
        ASSERT_EQ(r_buff.capacity, 512U);

        status = ezmRingBuffer_Push(&r_buff, push_512, 512);
        ASSERT_EQ(status, BUFF_OK);

        /*pop out the first 5*/
        status = ezmRingBuffer_Pop(&r_buff, pop_128, 128);
        ASSERT_EQ(status, BUFF_OK);

        for(uint8_t i = 0; i < 128; i++)
        {
            ASSERT_EQ(pop_128[i], i);
        }

        uint16_t avail_mem = 0;
        status = ezmRingBuffer_GetAvailableMemory(&r_buff, &avail_mem);
        ASSERT_EQ(avail_mem, 128);

        /*Get the next 128 byte*/
        status = ezmRingBuffer_Pop(&r_buff, pop_128, 128);
        ASSERT_EQ(status, BUFF_OK);

        for(uint8_t i = 0; i < 128; i++)
        {
            ASSERT_EQ(pop_128[i], i + 128);
        }

        status = ezmRingBuffer_GetAvailableMemory(&r_buff, &avail_mem);
        ASSERT_EQ(avail_mem, 128 + 128);

    }
#endif /* STATIC_MEM */
#endif /* RING_BUFFER */
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}