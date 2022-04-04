#include <limits.h>
#include "gtest/gtest.h"
#ifndef UNIT_TEST_ASSERT
#define UNIT_TEST_ASSERT

extern "C" {

#include "../../ezmsdk/app/app_config.h"

#if (HELPER_ASSERT == 1U)
#include "../../ezmsdk/helper/ezmAssert/ezmAssert.h"
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
        ASSERT_MSG(ptr != NULL, "this param must not be NULL");

        ptr = &var2;
        ASSERT(ptr != NULL);
    }
#endif /* HELPER_ASSERT */
}


#endif /* UNIT_TEST_ASSERT */