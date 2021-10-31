#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "uart.h"
}

namespace 
{
    TEST(Sum, Result) 
    {
        EXPECT_EQ(2, sum(1,2));
        EXPECT_EQ(1, sum(0,1));
    }

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}