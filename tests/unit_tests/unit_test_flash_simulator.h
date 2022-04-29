#include <limits.h>
#include "gtest/gtest.h"
#include "stdio.h"
#ifndef UNIT_TEST_FLASH_SIMULATOR
#define UNIT_TEST_FLASH_SIMULATOR

extern "C" {
#include "app/app_config.h"
#include "hw_simulator/flash/flash_simulator.h"
}


namespace
{
#if (FLASH_SIM == 1U)
    TEST(flash_simulator, tbd)
    {
    }
#endif /* KERNEL */
#endif /* UNIT_TEST_FLASH_SIMULATOR */
}