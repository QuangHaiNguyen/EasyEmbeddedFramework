#include <limits.h>
#include "gtest/gtest.h"
#include "stdio.h"
#ifndef UNIT_TEST_PRINTF
#define UNIT_TEST_PRINTF

extern "C" {
#include "../../ezmsdk/app/app_config.h"
#include "../../ezmsdk/ezmDebug/ezmDebug.h"
    char buffer[128] = { 0 };
}


namespace
{
#if (EZM_PRINTF == 1U)
    TEST(printf, basic)
    {
        int compare = 0;
        ezm_snprintf(buffer, 128, "test %d", 1234);
        printf("%s\n", buffer);

        ezm_snprintf(buffer, 128, "test %d", -4567);
        printf("%s\n", buffer);

        ezm_snprintf(buffer, 128, "test %x", 100);
        printf("%s\n", buffer);

        ezm_snprintf(buffer, 128, "test %x", -100);
        printf("%s\n", buffer);

        ezm_snprintf(buffer, 128, "test %c", 'u');
        printf("%s\n", buffer);

        ezm_snprintf(buffer, 128, "%s", "hello world!!!");
        printf("%s\n", buffer);

        ezm_snprintf(buffer, 128, "test float number. Value of pi: %f", 3.00);
        printf("%s\n", buffer);

        ezm_snprintf(buffer, 128, "test %s %d %x\n", "hello world", 12345, 100);
        printf("%s\n", buffer);

        ezm_printf("test %s %d %x\n", "hello world", 12345, 100);

        unsigned int len = ezm_StringLen("hello world");
        ezm_printf("[len = %d]\n", len);

        len = ezm_StringLen("");
        ezm_printf("[len = %d]\n", len);

        len = ezm_StringCopy("hello world", buffer, 128);
        compare = strcmp(buffer, "hello world");
        ASSERT_EQ(compare, 0);
        ASSERT_EQ(len, 11);

    }
#endif /* KERNEL */
#endif /* UNIT_TEST_PRINTF */
}