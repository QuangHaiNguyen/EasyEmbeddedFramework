
#include "app/app.h"
#include "app/app_config.h"
#include "ezmKernel/ezmKernel.h"
#include "app/app_embedded_emulator.h"

#include <stdint.h>


#if (CONFIG_WIN == 1U)
#include <time.h>
#endif

#if(CONFIG_WIN == 1U)
static uint32_t ezmApp_ReturnTimestampMillisvoid(void);
#endif

#if (CONFIG_UNITY_UNIT_TEST == 0U)
void main(void)
{
    ezmApp_SdkInit();

#if(CONFIG_WIN == 1U)
    uint64_t execute_time_stamp = ezmApp_ReturnTimestampMillisvoid();
    do
    {
#if (CONFIG_EMBEDDED_EMULATOR == 1U)
        ezMbedEmulator_Run();
#endif /* CONFIG_EMBEDDED_EMULATOR */

#if (CONFIG_CLI == 1U)
        // ezmCli_Run();
#endif
        if (ezmApp_ReturnTimestampMillisvoid() - execute_time_stamp > 1)
        {
            ezmKernel_UpdateClock();
            ezmKernel_Run();
            execute_time_stamp = ezmApp_ReturnTimestampMillisvoid();
        }
    } while (execute_time_stamp);
#endif /* CONFIG_WIN */
}
#endif

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
