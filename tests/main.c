#include "ezApp/ezSdk.h"
#include "ezApp/ezSdk_config.h"
#include "unity.h"
#include "unity_fixture.h"

static void RunAllTests(void);

int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}

static void RunAllTests(void)
{
    RUN_TEST_GROUP(ezEventNotifier);
    RUN_TEST_GROUP(ezQueue);
}