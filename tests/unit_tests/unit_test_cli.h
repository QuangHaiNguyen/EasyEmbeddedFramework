#include <limits.h>
#include "gtest/gtest.h"
#ifndef UNIT_TEST_ASSERT
#define UNIT_TEST_ASSERT

extern "C" {

#include "../../ezmsdk/app/app_config.h"
#include "string.h"

#if (CLI == 1U)
#include "../../ezmsdk/cli/cli.h"

uint32_t TestCommand(const char * pu8Command, void * pArgumentList, void * pValueList)
{
    (void)(pu8Command);
    (void)(pArgumentList);
    (void)(pValueList);
    return 0;
}

#endif /* CLI */
}

namespace 
{

#if (HELPER_ASSERT == 1U)
    TEST(cli, add_command) 
    {
        uint8_t u8Result;
        u8Result = ezmCli_RegisterCommand(NULL, NULL, NULL);
        ASSERT_EQ(u8Result, 0xff);

        u8Result = ezmCli_RegisterCommand("Test_Command", NULL, NULL);
        ASSERT_EQ(u8Result, 0xff);

        u8Result = ezmCli_RegisterCommand(NULL, "this is a test command", &TestCommand);
        ASSERT_EQ(u8Result, 0xff);

        u8Result = ezmCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
        ASSERT_NE(u8Result, 0xff);

        u8Result = ezmCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
        ASSERT_EQ(u8Result, 0xff);
    }

    TEST(cli, add_argument) 
    {
        uint8_t u8Result;
        bool    bResult;

        ezmCli_Init();
        u8Result = ezmCli_RegisterCommand("Test_Command1", "this is a test command",  &TestCommand);
        ASSERT_NE(u8Result, 0xff);

        bResult = ezmCli_AddArgument(u8Result, NULL, "-a1", "argument 1");
        ASSERT_EQ(bResult, false);

        bResult = ezmCli_AddArgument(u8Result, "--arg1", NULL, "argument 1");
        ASSERT_EQ(bResult, false);

        bResult = ezmCli_AddArgument(u8Result, "--arg1", "-a1", NULL);
        ASSERT_EQ(bResult, false);

        bResult = ezmCli_AddArgument(30, "--arg1", "-a1", "argument 1");
        ASSERT_EQ(bResult, false);

        bResult = ezmCli_AddArgument(u8Result, "--arg1", "-a1", "argument 1");
        ASSERT_EQ(bResult, true);

        bResult = ezmCli_AddArgument(u8Result, "--arg1", "-a2", "argument 2");
        ASSERT_EQ(bResult, false);

        bResult = ezmCli_AddArgument(u8Result, "--arg2", "-a1", "argument 2");
        ASSERT_EQ(bResult, false);

        bResult = ezmCli_AddArgument(u8Result, "--arg2", "-a2", "argument 2");
        ASSERT_EQ(bResult, true);

        bResult = ezmCli_AddArgument(u8Result, "--arg3", "-a3", "argument 3");
        ASSERT_EQ(bResult, true);

        bResult = ezmCli_AddArgument(u8Result, "--arg4", "-a4", "argument 4");
        ASSERT_EQ(bResult, true);

        bResult = ezmCli_AddArgument(u8Result, "--arg5", "-a5", "argument 5");
        ASSERT_EQ(bResult, false);

    }

    TEST(cli, receive_command) 
    {
        uint8_t u8Result;
        bool    bResult;

        ezmCli_Init();
        u8Result = ezmCli_RegisterCommand("Test_Command1", "this is a test command",  &TestCommand);
        ASSERT_NE(u8Result, 0xff);

        bResult = ezmCli_AddArgument(u8Result, "--arg1", "-a1", "argument 1");
        ASSERT_EQ(bResult, true);

        memcpy(au8CommandBuffer, "Test_Command --arg1 1234\n", sizeof("Test_Command --arg1 1234\n"));
        ezmCli_CommandReceivedCallback(0, au8CommandBuffer, NULL);

        memset(au8CommandBuffer, 0, CLI_BUFF_SIZE);
        memcpy(au8CommandBuffer, "Test_Command1 --arg1 1234\n", sizeof("Test_Command1 --arg1 1234\n"));
        ezmCli_CommandReceivedCallback(0, au8CommandBuffer, NULL);
    }
#endif /* HELPER_ASSERT */
}


#endif /* UNIT_TEST_ASSERT */