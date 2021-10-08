#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "debug.h"
}

namespace 
{
    TEST(Sum, Result) 
    {
        PRINT1("info");
        PRINT2("info %d", 1);
        PRINT3("info %d %d", 1, 2);
        PRINT_ERR1("error");
    }

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}