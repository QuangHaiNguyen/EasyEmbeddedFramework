
#include <limits.h>
#include "gtest/gtest.h"
#include "unit_test_ipc.h"
#include "unit_test_binParser.h"
#include "unit_test_assert.h"
#include "unit_test_hexdump.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}