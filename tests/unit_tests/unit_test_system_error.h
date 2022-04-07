#include <limits.h>
#include "gtest/gtest.h"
#include "stdio.h"
#ifndef UNIT_TEST_SYSTEM_ERROR
#define UNIT_TEST_SYSTEM_ERROR

extern "C" {
#include "../../ezmsdk/app/app_config.h"
#include "../../ezmsdk/helper/system_error/system_error.h"
}


namespace
{
#if (SYSTEM_ERROR == 1U)
    TEST(system_error, basic)
    {
        bool result;
        result = SystemError_SetError(0x00);
        ASSERT_EQ(result, true);

        result = SystemError_SetError(0x01);
        ASSERT_EQ(result, true);

        result = SystemError_SetError(0x02);
        ASSERT_EQ(result, true);

        result = SystemError_SetError(0x03);
        ASSERT_EQ(result, true);

    }
#endif /* KERNEL */
#endif /* UNIT_TEST_SYSTEM_ERROR */
}