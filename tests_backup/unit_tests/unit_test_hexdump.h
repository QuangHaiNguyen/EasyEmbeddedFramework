#include <limits.h>
#include "gtest/gtest.h"

#ifndef UNIT_TEST_HEXDUMP
#define UNIT_TEST_HEXDUMP

extern "C" {
#include "../../ezmsdk/app/app_config.h"
#include "../../ezmsdk/helper/hexdump/hexdump.h"
}

namespace 
{
#if (CONFIG_HELPER_HEXDUMP == 1U)
    TEST(Hexdump, Test_Binary) 
    {
        uint8_t au8Data[100];
        for(uint8_t i = 0; i < 100; i= i +4)
        {
            au8Data[i] = 0xCA;
            au8Data[i + 1] = 0xFE;
            au8Data[i + 2] = 0xBE;
            au8Data[i + 3] = 0xEF;
        }
        ezHexdump((void *)au8Data, 100);

        struct TestStruture
        {
            uint16_t u16c = 0xCAFE;
            uint16_t u16a = 0xBEEF;
            uint32_t u32a = 0xFFFFFFFF;
            uint8_t u8b = 0xAA;
        };

        TestStruture stTest;
        ezHexdump((void *)&stTest, sizeof(TestStruture));
    }
    
    TEST(Hexdump, Test_Ascii)
    {
        uint8_t au8CapitalLetter[24];

        for(uint8_t i = 0; i < 24; i++)
        {
            au8CapitalLetter[i] = i + 0x41;
        }
        ezHexdump((void *)au8CapitalLetter, 24);

        uint8_t au8SmallLetter[24];

        for(uint8_t i = 0; i < 24; i++)
        {
            au8SmallLetter[i] = i + 0x61;
        }
        ezHexdump((void *)au8SmallLetter, 24);
    } 
#endif /* CONFIG_HELPER_HEXDUMP */
}

#endif /* UNIT_TEST_HEXDUMPT */