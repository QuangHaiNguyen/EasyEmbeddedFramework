#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "../ezmAssert.h"

#define ASSERT_MOD_ID       0x01U

REGISTER_ASSERT(ASSERT_MOD_ID)
}

namespace 
{
    TEST(assert, module) 
    {
        int var1 = 10;
        ASSERT(var1 > 10);
        ASSERT(var1 < 10);
        ASSERT(var1 == 10);

        int * ptr;
        int var2 = 20;

        ASSERT(ptr != NULL);

        ptr = &var2;
        ASSERT(ptr != NULL);
    }

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}