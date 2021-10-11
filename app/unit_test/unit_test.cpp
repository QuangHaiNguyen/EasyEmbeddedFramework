#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "../app.h"
}

namespace 
{
    TEST(Sum, Result) 
    {
        ezmApp_SdkInit();
    }

}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}