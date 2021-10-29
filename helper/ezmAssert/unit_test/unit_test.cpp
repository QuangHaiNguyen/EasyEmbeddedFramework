#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#if (HELPER_ASSERT == 1U)
#include "../../../app/app_config.h"

#define ASSERT_MOD_ID       0x01U

REGISTER_ASSERT(ASSERT_MOD_ID)
#endif /* HELPER_ASSERT */
}

namespace 
{

#if (HELPER_ASSERT == 1U)
    TEST(assert, module) 
    {
        int var1 = 10;
        ASSERT(var1 > 10);
        ASSERT(var1 < 10);
        ASSERT(var1 == 10);

        int * ptr = NULL;
        int var2 = 20;

        ASSERT(ptr == NULL);

        ptr = &var2;
        ASSERT(ptr != NULL);
    }
#endif /* HELPER_ASSERT */
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}