
#include "ezApp/ezSdk.h"
#include "ezApp/ezSdk_config.h"
#include "ezApp/ezKernel/ezKernel.h"
#include "ezApp/cli/cli.h"
#include "unity_test_platform/unity_fixture.h"
#include <stdint.h>


#if (CONFIG_WIN == 1U)
#include <time.h>
#endif

#if(CONFIG_WIN == 1U)
static uint32_t ezmApp_ReturnTimestampMillisvoid(void);
#endif

#if (CONFIG_UNITY_UNIT_TEST == 1U)
static void RunAllTests(void);
#endif

int main(int argc, const char *argv[])
{
    ezSdk_Initialization();

#if (CONFIG_UNITY_UNIT_TEST == 1U)
    (void) UnityMain(argc, argv, RunAllTests);
#endif /* CONFIG_UNITY_UNIT_TEST == 1U */

#if(CONFIG_WIN == 1U)
    uint64_t execute_time_stamp = ezmApp_ReturnTimestampMillisvoid();
    do
    {
#if (CONFIG_EMBEDDED_EMULATOR == 1U)
        ezMbedEmulator_Run();
#endif /* CONFIG_EMBEDDED_EMULATOR */

#if (CONFIG_CLI == 1U)
         ezmCli_Run();
#endif
        if (ezmApp_ReturnTimestampMillisvoid() - execute_time_stamp > 1)
        {
            ezKernel_UpdateTickMillis();
            ezKernel_Run();
            execute_time_stamp = ezmApp_ReturnTimestampMillisvoid();
        }
} while (execute_time_stamp);
#endif /* CONFIG_WIN */

}


#if(CONFIG_WIN == 1U)
static uint32_t ezmApp_ReturnTimestampMillisvoid(void)
{
    uint32_t tick_milli;
    clock_t tick;
    tick = clock();
    tick_milli = tick / (CLOCKS_PER_SEC / 1000);

    return tick_milli;
}
#endif


#if (CONFIG_UNITY_UNIT_TEST == 1U)
static void RunAllTests(void)
{
#if (CONFIG_EZ_RPC_TEST == 1U)
    RUN_TEST_GROUP(ezRpc);
#endif /* CONFIG_EZ_RPC_TEST == 1U */

#if (CONFIG_EZ_QUEUE_TEST == 1U)
    RUN_TEST_GROUP(ezQueue);
#endif /* CONFIG_EZ_QUEUE_TEST == 1U */

#if(CONFIG_KERNEL_TEST == 1U)
    RUN_TEST_GROUP(ezKernel);
#endif /* CONFIG_KERNEL_TEST == 1U */

#if (CONFIG_DRIVERINF_TEST == 1U)
    RUN_TEST_GROUP(ezDriver);
#endif /* CONFIG_DRIVERINF_TEST == 1U */

#if (CONFIG_EZ_EVENT_NOTIFIER_TEST == 1U)
    RUN_TEST_GROUP(ezEventNotifier);
#endif /* CONFIG_EZ_EVENT_NOTIFIER_TEST == 1U */
}
#endif
