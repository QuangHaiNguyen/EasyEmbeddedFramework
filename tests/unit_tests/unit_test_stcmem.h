#include <limits.h>
#include "gtest/gtest.h"

#ifndef UNIT_TEST_SMALLOC
#define UNIT_TEST_SMALLOC

extern "C" {
#include "../../ezmsdk/app/app_config.h"
#include "../../ezmsdk/helper/stcmem/stcmem.h"

    uint8_t au8Buffer[512] = {0};
}


namespace
{
#if (SMALLOC == 1U)
    TEST(stcmem, u8_var)
    {
        ezmStcMem_Initialization();
        ezmMemList stMemList;

        ezmStcMem_InitMemList(&stMemList, au8Buffer, 512);
        ezmStcMem_HexdumpBuffer(&stMemList);

        uint8_t* pu8TestU8Var1 = (uint8_t*)ezmStcMem_Malloc(&stMemList, sizeof(uint8_t));
        *pu8TestU8Var1 = 100;
        ASSERT_EQ(*pu8TestU8Var1, au8Buffer[0]);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        uint8_t* pu8TestU8Var2 = (uint8_t*)ezmStcMem_Malloc(&stMemList, sizeof(uint8_t));
        *pu8TestU8Var2 = 123;
        ASSERT_EQ(*pu8TestU8Var2, au8Buffer[1]);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 2U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        ezmStcMem_Free(&stMemList, pu8TestU8Var1);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 2U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);

        ezmStcMem_Free(&stMemList, pu8TestU8Var2);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 0U);
    }

    TEST(stcmem, u16_var)
    {
        ezmStcMem_Initialization();
        ezmMemList stMemList;

        ezmStcMem_InitMemList(&stMemList, au8Buffer, 512);
        ezmStcMem_HexdumpBuffer(&stMemList);

        uint16_t* pu16TestU16Var1 = (uint16_t*)ezmStcMem_Malloc(&stMemList, sizeof(uint16_t));
        *pu16TestU16Var1 = 0xABCD;
        ASSERT_EQ(*pu16TestU16Var1, *(uint16_t*)&au8Buffer[0]);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        uint16_t* pu16TestU16Var2 = (uint16_t*)ezmStcMem_Malloc(&stMemList, sizeof(uint16_t));
        *pu16TestU16Var2 = 0xCDEF;
        ASSERT_EQ(*pu16TestU16Var2, *(uint16_t*)&au8Buffer[2]);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 2U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        ezmStcMem_Free(&stMemList, pu16TestU16Var1);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 2U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);

        ezmStcMem_Free(&stMemList, pu16TestU16Var2);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 0U);
    }

    TEST(stcmem, u32_var)
    {
        ezmStcMem_Initialization();
        ezmMemList stMemList;

        ezmStcMem_InitMemList(&stMemList, au8Buffer, 512);
        ezmStcMem_HexdumpBuffer(&stMemList);

        uint32_t* pu32TestU32Var1 = (uint32_t*)ezmStcMem_Malloc(&stMemList, sizeof(uint32_t));
        *pu32TestU32Var1 = 0xABCDCDEF;
        ASSERT_EQ(*pu32TestU32Var1, *(uint32_t*)&au8Buffer[0]);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        uint32_t* pu32TestU32Var2 = (uint32_t*)ezmStcMem_Malloc(&stMemList, sizeof(uint32_t));
        *pu32TestU32Var2 = 0xCDEFEFAB;
        ASSERT_EQ(*pu32TestU32Var2, *(uint32_t*)&au8Buffer[4]);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 2U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        ezmStcMem_Free(&stMemList, pu32TestU32Var1);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 2U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);

        ezmStcMem_Free(&stMemList, pu32TestU32Var2);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 0U);
    }

    TEST(stcmem, array_1)
    {
        ezmStcMem_Initialization();
        ezmMemList stMemList;
        bool bIsEqual;

        ezmStcMem_InitMemList(&stMemList, au8Buffer, 512);
        ezmStcMem_HexdumpBuffer(&stMemList);
 
        /* alloc 256 bytes */
        uint8_t* au8Array256_1 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 256 * sizeof(uint8_t));
        memset(au8Array256_1, 1, 256 * sizeof(uint8_t));
        bIsEqual = memcmp(au8Array256_1, &au8Buffer[0], 256 * sizeof(uint8_t));

        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        /* alloc another 256 bytes, buffer is fulled */
        uint8_t* au8Array256_2 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 256 * sizeof(uint8_t));
        memset(au8Array256_2, 2, 256 * sizeof(uint8_t));
        bIsEqual = memcmp(au8Array256_2, &au8Buffer[256], 256 * sizeof(uint8_t));

        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 0U);
        ASSERT_EQ(stMemList.alloc_list.u16Size,2U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        /* Free first half 256 bytes */
        ezmStcMem_Free(&stMemList, au8Array256_1);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);

        /* Allocate 128 bytes */
        uint8_t* au8Array128_1 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 128 * sizeof(uint8_t));
        EXPECT_TRUE(au8Array128_1 != nullptr);
        memset(au8Array128_1, 3, 128 * sizeof(uint8_t));
        ezmStcMem_HexdumpBuffer(&stMemList);

        bIsEqual = memcmp(au8Array128_1, &au8Buffer[0], 128 * sizeof(uint8_t));
        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(2U, stMemList.alloc_list.u16Size);

        /* Allocate another 128 bytes */
        uint8_t* au8Array128_2 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 128 * sizeof(uint8_t));
        EXPECT_TRUE(au8Array128_2 != nullptr);
        memset(au8Array128_2, 4, 128 * sizeof(uint8_t));
        ezmStcMem_HexdumpBuffer(&stMemList);

        bIsEqual = memcmp(au8Array128_2, &au8Buffer[128], 128 * sizeof(uint8_t));
        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 0U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 3U);

        /* Free second 128 byte */
        ezmStcMem_Free(&stMemList, au8Array128_2);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 2U);

        /* Allocate another 64 bytes */
        uint8_t* au8Array64_1 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 64 * sizeof(uint8_t));
        EXPECT_TRUE(au8Array64_1 != nullptr);
        memset(au8Array64_1, 5, 64 * sizeof(uint8_t));
        ezmStcMem_HexdumpBuffer(&stMemList);

        bIsEqual = memcmp(au8Array64_1, &au8Buffer[128], 64 * sizeof(uint8_t));
        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 3U);

        /* Allocate another 64 bytes */
        uint8_t* au8Array64_2 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 64 * sizeof(uint8_t));
        EXPECT_TRUE(au8Array64_2 != nullptr);
        memset(au8Array64_2, 6, 64 * sizeof(uint8_t));
        ezmStcMem_HexdumpBuffer(&stMemList);

        bIsEqual = memcmp(au8Array64_2, &au8Buffer[192], 64 * sizeof(uint8_t));
        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 0U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 4U);

        /* Free everything let see if it breaks */
        ezmStcMem_Free(&stMemList, au8Array256_2);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 3U);

        ezmStcMem_Free(&stMemList, au8Array128_1);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 2U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 2U);

        ezmStcMem_Free(&stMemList, au8Array64_1);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 2U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);

        ezmStcMem_Free(&stMemList, au8Array64_2);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 0U);

    }

    TEST(stcmem, array_2)
    {
        ezmStcMem_Initialization();
        ezmMemList stMemList;
        bool bIsEqual;

        ezmStcMem_InitMemList(&stMemList, au8Buffer, 512);
        ezmStcMem_HexdumpBuffer(&stMemList);

        /* alloc 256 bytes */
        uint8_t* au8Array256_1 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 256 * sizeof(uint8_t));
        memset(au8Array256_1, 1, 256 * sizeof(uint8_t));
        bIsEqual = memcmp(au8Array256_1, &au8Buffer[0], 256 * sizeof(uint8_t));

        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        /* alloc another 256 bytes, buffer is fulled */
        uint8_t* au8Array256_2 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 256 * sizeof(uint8_t));
        memset(au8Array256_2, 2, 256 * sizeof(uint8_t));
        bIsEqual = memcmp(au8Array256_2, &au8Buffer[256], 256 * sizeof(uint8_t));

        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 0U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 2U);
        ezmStcMem_HexdumpBuffer(&stMemList);

        /* Free first half 256 bytes */
        ezmStcMem_Free(&stMemList, au8Array256_1);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 1U);

        /* Allocate 128 bytes */
        uint8_t* au8Array128_1 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 128 * sizeof(uint8_t));
        EXPECT_TRUE(au8Array128_1 != nullptr);
        memset(au8Array128_1, 3, 128 * sizeof(uint8_t));
        ezmStcMem_HexdumpBuffer(&stMemList);

        bIsEqual = memcmp(au8Array128_1, &au8Buffer[0], 128 * sizeof(uint8_t));
        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(2U, stMemList.alloc_list.u16Size);

        /* Allocate another 128 bytes */
        uint8_t* au8Array128_2 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 128 * sizeof(uint8_t));
        EXPECT_TRUE(au8Array128_2 != nullptr);
        memset(au8Array128_2, 4, 128 * sizeof(uint8_t));
        ezmStcMem_HexdumpBuffer(&stMemList);

        bIsEqual = memcmp(au8Array128_2, &au8Buffer[128], 128 * sizeof(uint8_t));
        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 0U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 3U);

        /* Free second 128 byte */
        ezmStcMem_Free(&stMemList, au8Array128_2);
        ezmStcMem_HexdumpBuffer(&stMemList);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 2U);

        /* Allocate another 64 bytes */
        uint8_t* au8Array64_1 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 64 * sizeof(uint8_t));
        EXPECT_TRUE(au8Array64_1 != nullptr);
        memset(au8Array64_1, 5, 64 * sizeof(uint8_t));
        ezmStcMem_HexdumpBuffer(&stMemList);

        bIsEqual = memcmp(au8Array64_1, &au8Buffer[128], 64 * sizeof(uint8_t));
        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 3U);

        /* Allocate another 64 bytes */
        uint8_t* au8Array64_2 = (uint8_t*)ezmStcMem_Malloc(&stMemList, 64 * sizeof(uint8_t));
        EXPECT_TRUE(au8Array64_2 != nullptr);
        memset(au8Array64_2, 6, 64 * sizeof(uint8_t));
        ezmStcMem_HexdumpBuffer(&stMemList);

        bIsEqual = memcmp(au8Array64_2, &au8Buffer[192], 64 * sizeof(uint8_t));
        ASSERT_EQ(bIsEqual, 0U);
        ASSERT_EQ(stMemList.free_list.u16Size, 0U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 4U);

        /* Free everything let see if it breaks */
        ezmStcMem_Free(&stMemList, au8Array64_1);
        ezmStcMem_HexdumpBuffer(&stMemList);

        ezmStcMem_Free(&stMemList, au8Array64_2);
        ezmStcMem_HexdumpBuffer(&stMemList);

        ezmStcMem_Free(&stMemList, au8Array128_1);
        ezmStcMem_HexdumpBuffer(&stMemList);

        ezmStcMem_Free(&stMemList, au8Array256_2);
        ezmStcMem_HexdumpBuffer(&stMemList);

        ASSERT_EQ(stMemList.free_list.u16Size, 1U);
        ASSERT_EQ(stMemList.alloc_list.u16Size, 0U);

    }
#endif /* SMALLOC */
#endif /* UNIT_TEST_SMALLOC */
}