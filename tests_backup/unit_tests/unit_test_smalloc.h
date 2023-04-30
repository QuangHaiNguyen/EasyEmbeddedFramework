#include <limits.h>
#include "gtest/gtest.h"

#ifndef UNIT_TEST_SMALLOC
#define UNIT_TEST_SMALLOC

extern "C" {
#include "../../ezmsdk/app/app_config.h"
#include "../../ezmsdk/helper/smalloc/smalloc.h"
}


namespace
{
#if (SMALLOC == 1U)
    TEST(smalloc, main)
    {
        struct
        {
            uint8_t a;
            uint16_t b;
            uint32_t c;
            uint8_t array[5];
        }Test_Struct;

        ezmSmalloc_Initialize();

        char* p8buffer1 = (char*)ezmSmalloc_Malloc(8);
        for (size_t i = 0; i < 8; i++)
        {
            p8buffer1[i] = 0xaa;
        }

        uint32_t* p32buffer2 = (uint32_t*)ezmSmalloc_Malloc(sizeof(uint32_t));
        *p32buffer2 = 0xBEEFCAFE;

        struct Test_Struct* pstTestStruct;
        pstTestStruct = (struct Test_Struct*)ezmSmalloc_Malloc(sizeof(Test_Struct));

        emzSmalloc_Free(p32buffer2);
        emzSmalloc_Free(p8buffer1);
        emzSmalloc_Free(pstTestStruct);
    }

    TEST(smalloc, mem_ll)
    {
        ezmSmalloc_Initialize();

        ezmMemList stTestList;
        ezmSmalloc_InitMemList(&stTestList, 0x22);

        ASSERT_EQ(stTestList.u16Size, 0);
        ASSERT_EQ(stTestList.u8ModuleId, 0x22);
        ASSERT_TRUE(stTestList.pstHead == nullptr);
        ASSERT_TRUE(stTestList.pstTail == nullptr);

        ezmMemoryBlock* pstMemBlock1 = ezmSmalloc_GetMemBlockInList(&stTestList, 16);

        for (uint8_t i = 0; i < pstMemBlock1->u16BufferSize; i++)
        {
            pstMemBlock1->pBuffer[i] = 0xaa;
        }
        ASSERT_EQ(stTestList.u16Size, 1);

        ezmMemoryBlock* pstMemBlock2 = ezmSmalloc_GetMemBlockInList(&stTestList, 32);
        ASSERT_TRUE(pstMemBlock2 != nullptr);

        for (uint8_t i = 0; i < pstMemBlock2->u16BufferSize; i++)
        {
            pstMemBlock2->pBuffer[i] = 0xaa + 1;
        }
        ASSERT_EQ(stTestList.u16Size, 2);

        /*Using other method*/
        ezmMemoryBlock* pstMemBlock3 = ezmSmalloc_GetFreeBlock(48);
        ezmSmalloc_ApendBlockToList(pstMemBlock3, &stTestList);

        for (uint8_t i = 0; i < pstMemBlock3->u16BufferSize; i++)
        {
            pstMemBlock3->pBuffer[i] = 0xaa + 2;
        }
        ASSERT_EQ(stTestList.u16Size, 3);

        ezmMemoryBlock* pstBlock;
        pstBlock = (ezmMemoryBlock*)stTestList.pstHead;
        uint8_t count = 0;

        for (pstBlock = (ezmMemoryBlock*)stTestList.pstHead; pstBlock != NULL; pstBlock = pstBlock->pstNextNode)
        {
            for (uint8_t i = 0; i < pstBlock->u16BufferSize; i++)
            {
                ASSERT_EQ(pstBlock->pBuffer[i], 0xaa + count);
            }
            count++;
        }
        ASSERT_EQ(stTestList.u16Size, count);

        ezmSmalloc_ReturnMemBlock(&stTestList, pstMemBlock1);
        ASSERT_EQ(stTestList.u16Size, 2);

        ezmSmalloc_ReturnMemBlock(&stTestList, pstMemBlock2);
        ASSERT_EQ(stTestList.u16Size, 1);

        ezmSmalloc_ReturnMemBlock(&stTestList, pstMemBlock3);
        ASSERT_EQ(stTestList.u16Size, 0);
    }
#endif /* SMALLOC */
#endif /* UNIT_TEST_SMALLOC */
}