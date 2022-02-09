#include <limits.h>
#include "gtest/gtest.h"

#ifndef UNIT_TEST_STCMEM
#define UNIT_TEST_STCMEM

extern "C" {
#include "../../ezmsdk/app/app_config.h"
#include "../../ezmsdk/ezmKernel/ezmKernel.h"
#include "../../ezmsdk/helper/linked_list/linked_list.h"
#include "../../ezmsdk/ezmDebug/ezmDebug.h"
#include <time.h>

#define MOD_NAME    "UNIT_TEST"
#define RUNTIME_MS  40000
    uint8_t test_1000_ms_proc(void)
    {
        PRINT_DEBUG(MOD_NAME, "1000 ms has passed");
        return 0;
    }

    uint8_t test_2000_ms_proc(void)
    {
        PRINT_DEBUG(MOD_NAME, "2000 ms has passed");
        return 0;
    }
    uint8_t test_4000_ms_proc(void)
    {
        PRINT_DEBUG(MOD_NAME, "4000 ms has passed");
        return 0;
    }

    uint8_t high_load_proc(void)
    {
        PRINT_DEBUG(MOD_NAME, "high load process");
        
        uint32_t sum = 0;
        for (uint32_t i = 0; i < 0xFFFFFFF; i++)
        {
            sum = sum + i;
        }
        return 0;
    }

    uint32_t ImplementTickFunction(void)
    {
        uint32_t u32TickMicro;
        clock_t tick;
        tick = clock();
        u32TickMicro = tick / (CLOCKS_PER_SEC / 1000);

        return u32TickMicro;
    }
}


namespace
{
#if (KERNEL == 1U)
    TEST(kernel, basic)
    {
        ezmLL_Initialization();
        ezmKernel_Initialization();

        process proc_1000_ms;
        process proc_2000_ms;
        process proc_4000_ms;
        process high_load_process;

        uint32_t start_ms;
        uint32_t current_ms;

        ezmKernel_AddProcess(&proc_1000_ms, PROC_REPEATED, 1000, test_1000_ms_proc);
        ezmKernel_AddProcess(&proc_2000_ms, PROC_REPEATED, 2000, test_2000_ms_proc);
        ezmKernel_AddProcess(&proc_4000_ms, PROC_ONCE, 4000, test_4000_ms_proc);
        ezmKernel_AddProcess(&high_load_process, PROC_REPEATED, 1000, high_load_proc);

        start_ms = ImplementTickFunction();
#if 1
        current_ms = ImplementTickFunction();
        while (start_ms + RUNTIME_MS > ImplementTickFunction())
        {
            if (ImplementTickFunction() - current_ms >= 1)
            {
                ezmKernel_Clock();
                ezmKernel_Run();
                current_ms = ImplementTickFunction();
            }
        }
#endif
    }
#endif /* KERNEL */
#endif /* UNIT_TEST_STCMEM */
}