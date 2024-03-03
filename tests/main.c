#include "app/ezSdk.h"
#include "app/ezSdk_config.h"
#include "unity.h"
#include "unity_fixture.h"

static void RunAllTests(void);

int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}

static void RunAllTests(void)
{
    //RUN_TEST_GROUP(ezEventNotifier);
    RUN_TEST_GROUP(ezQueue);
    //RUN_TEST_GROUP(ezKernel);
    //RUN_TEST_GROUP(EchoHal);
    //RUN_TEST_GROUP(ez_hal_uart);
}