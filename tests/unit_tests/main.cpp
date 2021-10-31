
#include <limits.h>
#include "gtest/gtest.h"
#include "unit_test_ipc.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}