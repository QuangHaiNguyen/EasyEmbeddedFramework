/*****************************************************************************
* Filename:         unittest_ez_static_alloc.c
* Author:           Hai Nguyen
* Original Date:    06.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   unittest_ez_static_alloc.c
 *  @author Hai Nguyen
 *  @date   06.03.2024
 *  @brief  Unit test for static allocation component
 *
 *  @details Detail description of the component
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
#include "ez_static_alloc.h"

TEST_GROUP(ez_static_alloc);

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
static uint8_t au8Buffer[512] = {0};


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


TEST_SETUP(ez_static_alloc)
{
}


TEST_TEAR_DOWN(ez_static_alloc)
{
}


TEST_GROUP_RUNNER(ez_static_alloc)
{
    RUN_TEST_CASE(ez_static_alloc, u8_var);
    RUN_TEST_CASE(ez_static_alloc, u16_var);
    RUN_TEST_CASE(ez_static_alloc, u32_var);
    RUN_TEST_CASE(ez_static_alloc, array_1);
    RUN_TEST_CASE(ez_static_alloc, array_2);
}


TEST(ez_static_alloc, u8_var)
{
    ezmMemList stMemList = {0};
    
    ezStaticAlloc_InitMemList(&stMemList, au8Buffer, 512);

    uint8_t* pu8TestU8Var1 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, sizeof(uint8_t));
    *pu8TestU8Var1 = 100;
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(*pu8TestU8Var1, au8Buffer[0]);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);

    uint8_t* pu8TestU8Var2 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, sizeof(uint8_t));
    *pu8TestU8Var2 = 123;
    TEST_ASSERT_EQUAL(*pu8TestU8Var2, au8Buffer[1]);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 2U);
    ezStaticAlloc_HexdumpBuffer(&stMemList); 

    ezStaticAlloc_Free(&stMemList, pu8TestU8Var1);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 2U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);

    ezStaticAlloc_Free(&stMemList, pu8TestU8Var2);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 0U);
    }

TEST(ez_static_alloc, u16_var)
{
    ezmMemList stMemList;

    ezStaticAlloc_InitMemList(&stMemList, au8Buffer, 512);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    uint16_t* pu16TestU16Var1 = (uint16_t*)ezStaticAlloc_Malloc(&stMemList, sizeof(uint16_t));
    *pu16TestU16Var1 = 0xABCD;
    TEST_ASSERT_EQUAL(*pu16TestU16Var1, *(uint16_t*)&au8Buffer[0]);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    uint16_t* pu16TestU16Var2 = (uint16_t*)ezStaticAlloc_Malloc(&stMemList, sizeof(uint16_t));
    *pu16TestU16Var2 = 0xCDEF;
    TEST_ASSERT_EQUAL(*pu16TestU16Var2, *(uint16_t*)&au8Buffer[2]);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 2U);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    ezStaticAlloc_Free(&stMemList, pu16TestU16Var1);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 2U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);

    ezStaticAlloc_Free(&stMemList, pu16TestU16Var2);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 0U);
}

TEST(ez_static_alloc, u32_var)
{
    ezmMemList stMemList;

    ezStaticAlloc_InitMemList(&stMemList, au8Buffer, 512);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    uint32_t* pu32TestU32Var1 = (uint32_t*)ezStaticAlloc_Malloc(&stMemList, sizeof(uint32_t));
    *pu32TestU32Var1 = 0xABCDCDEF;
    TEST_ASSERT_EQUAL(*pu32TestU32Var1, *(uint32_t*)&au8Buffer[0]);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    uint32_t* pu32TestU32Var2 = (uint32_t*)ezStaticAlloc_Malloc(&stMemList, sizeof(uint32_t));
    *pu32TestU32Var2 = 0xCDEFEFAB;
    TEST_ASSERT_EQUAL(*pu32TestU32Var2, *(uint32_t*)&au8Buffer[4]);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 2U);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    ezStaticAlloc_Free(&stMemList, pu32TestU32Var1);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 2U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);

    ezStaticAlloc_Free(&stMemList, pu32TestU32Var2);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 0U);
}

TEST(ez_static_alloc, array_1)
{
    ezmMemList stMemList;
    bool bIsEqual;

    ezStaticAlloc_InitMemList(&stMemList, au8Buffer, 512);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    /* alloc 256 bytes */
    uint8_t* au8Array256_1 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 256 * sizeof(uint8_t));
    memset(au8Array256_1, 1, 256 * sizeof(uint8_t));
    bIsEqual = memcmp(au8Array256_1, &au8Buffer[0], 256 * sizeof(uint8_t));

    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    /* alloc another 256 bytes, buffer is fulled */
    uint8_t* au8Array256_2 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 256 * sizeof(uint8_t));
    memset(au8Array256_2, 2, 256 * sizeof(uint8_t));
    bIsEqual = memcmp(au8Array256_2, &au8Buffer[256], 256 * sizeof(uint8_t));

    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList),2U);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    /* Free first half 256 bytes */
    ezStaticAlloc_Free(&stMemList, au8Array256_1);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);

    /* Allocate 128 bytes */
    uint8_t* au8Array128_1 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 128 * sizeof(uint8_t));
    TEST_ASSERT_NOT_EQUAL(au8Array128_1, NULL);
    memset(au8Array128_1, 3, 128 * sizeof(uint8_t));
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    bIsEqual = memcmp(au8Array128_1, &au8Buffer[0], 128 * sizeof(uint8_t));
    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(2U, ezStaticAlloc_GetNumOfAllocBlock(&stMemList));

    /* Allocate another 128 bytes */
    uint8_t* au8Array128_2 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 128 * sizeof(uint8_t));
    TEST_ASSERT_NOT_EQUAL(au8Array128_2, NULL);
    memset(au8Array128_2, 4, 128 * sizeof(uint8_t));
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    bIsEqual = memcmp(au8Array128_2, &au8Buffer[128], 128 * sizeof(uint8_t));
    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 3U);

    /* Free second 128 byte */
    ezStaticAlloc_Free(&stMemList, au8Array128_2);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 2U);

    /* Allocate another 64 bytes */
    uint8_t* au8Array64_1 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 64 * sizeof(uint8_t));
    TEST_ASSERT_NOT_EQUAL(au8Array64_1, NULL);
    memset(au8Array64_1, 5, 64 * sizeof(uint8_t));
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    bIsEqual = memcmp(au8Array64_1, &au8Buffer[128], 64 * sizeof(uint8_t));
    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 3U);

    /* Allocate another 64 bytes */
    uint8_t* au8Array64_2 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 64 * sizeof(uint8_t));
    TEST_ASSERT_NOT_EQUAL(au8Array64_2, NULL);
    memset(au8Array64_2, 6, 64 * sizeof(uint8_t));
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    bIsEqual = memcmp(au8Array64_2, &au8Buffer[192], 64 * sizeof(uint8_t));
    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 4U);

    /* Free everything let see if it breaks */
    ezStaticAlloc_Free(&stMemList, au8Array256_2);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 3U);

    ezStaticAlloc_Free(&stMemList, au8Array128_1);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 2U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 2U);

    ezStaticAlloc_Free(&stMemList, au8Array64_1);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 2U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);

    ezStaticAlloc_Free(&stMemList, au8Array64_2);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 0U);

}

TEST(ez_static_alloc, array_2)
{
    ezmMemList stMemList;
    bool bIsEqual;

    ezStaticAlloc_InitMemList(&stMemList, au8Buffer, 512);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    /* alloc 256 bytes */
    uint8_t* au8Array256_1 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 256 * sizeof(uint8_t));
    memset(au8Array256_1, 1, 256 * sizeof(uint8_t));
    bIsEqual = memcmp(au8Array256_1, &au8Buffer[0], 256 * sizeof(uint8_t));

    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    /* alloc another 256 bytes, buffer is fulled */
    uint8_t* au8Array256_2 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 256 * sizeof(uint8_t));
    memset(au8Array256_2, 2, 256 * sizeof(uint8_t));
    bIsEqual = memcmp(au8Array256_2, &au8Buffer[256], 256 * sizeof(uint8_t));

    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 2U);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    /* Free first half 256 bytes */
    ezStaticAlloc_Free(&stMemList, au8Array256_1);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 1U);

    /* Allocate 128 bytes */
    uint8_t* au8Array128_1 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 128 * sizeof(uint8_t));
    TEST_ASSERT_NOT_EQUAL(au8Array128_1, NULL);
    memset(au8Array128_1, 3, 128 * sizeof(uint8_t));
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    bIsEqual = memcmp(au8Array128_1, &au8Buffer[0], 128 * sizeof(uint8_t));
    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(2U, ezStaticAlloc_GetNumOfAllocBlock(&stMemList));

    /* Allocate another 128 bytes */
    uint8_t* au8Array128_2 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 128 * sizeof(uint8_t));
    TEST_ASSERT_NOT_EQUAL(au8Array128_2, NULL);
    memset(au8Array128_2, 4, 128 * sizeof(uint8_t));
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    bIsEqual = memcmp(au8Array128_2, &au8Buffer[128], 128 * sizeof(uint8_t));
    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 3U);

    /* Free second 128 byte */
    ezStaticAlloc_Free(&stMemList, au8Array128_2);
    ezStaticAlloc_HexdumpBuffer(&stMemList);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 2U);

    /* Allocate another 64 bytes */
    uint8_t* au8Array64_1 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 64 * sizeof(uint8_t));
    TEST_ASSERT_NOT_EQUAL(au8Array64_1, NULL);
    memset(au8Array64_1, 5, 64 * sizeof(uint8_t));
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    bIsEqual = memcmp(au8Array64_1, &au8Buffer[128], 64 * sizeof(uint8_t));
    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 3U);

    /* Allocate another 64 bytes */
    uint8_t* au8Array64_2 = (uint8_t*)ezStaticAlloc_Malloc(&stMemList, 64 * sizeof(uint8_t));
    TEST_ASSERT_NOT_EQUAL(au8Array64_2, NULL);
    memset(au8Array64_2, 6, 64 * sizeof(uint8_t));
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    bIsEqual = memcmp(au8Array64_2, &au8Buffer[192], 64 * sizeof(uint8_t));
    TEST_ASSERT_EQUAL(bIsEqual, 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 0U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 4U);

    /* Free everything let see if it breaks */
    ezStaticAlloc_Free(&stMemList, au8Array64_1);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    ezStaticAlloc_Free(&stMemList, au8Array64_2);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    ezStaticAlloc_Free(&stMemList, au8Array128_1);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    ezStaticAlloc_Free(&stMemList, au8Array256_2);
    ezStaticAlloc_HexdumpBuffer(&stMemList);

    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfFreeBlock(&stMemList), 1U);
    TEST_ASSERT_EQUAL(ezStaticAlloc_GetNumOfAllocBlock(&stMemList), 0U);

}

/******************************************************************************
* Internal functions
*******************************************************************************/
static void RunAllTests(void)
{
    RUN_TEST_GROUP(ez_static_alloc);
}


/* End of file */
