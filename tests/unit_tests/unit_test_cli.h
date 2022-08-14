#include <limits.h>
#include "gtest/gtest.h"
#ifndef UNIT_TEST_ASSERT
#define UNIT_TEST_ASSERT

extern "C" {

#include "../../ezmsdk/app/app_config.h"
#include "string.h"

#if (CONFIG_CLI == 1U)
#include "../../ezmsdk/cli/cli.h"

char  au8CommandBuffer[512];

uint32_t    u32TestParam1;
uint32_t    u32TestParam2;
uint32_t    u32TestSum;
char        *pu8TestString;

CLI_NOTIFY_CODE TestCommand(const char * pu8Command, void * pValueList)
{
    CLI_NOTIFY_CODE eReturn = CLI_NC_OK;
    (void)(pu8Command);
    (void)(pValueList);
    printf("call back executed\n");
    return eReturn;
}

CLI_NOTIFY_CODE Sum(const char* pu8Command, void * pValueList)
{
    CLI_NOTIFY_CODE eReturn = CLI_NC_OK;
    uint32_t u32Param1 = 0;
    uint32_t u32Param2 = 0;
    uint32_t * pu32Params = (uint32_t*)pValueList;

    if (*pu32Params)
    {
        u32Param1 = atoi((char*)(*pu32Params));
        u32TestParam1 = u32Param1;
        pu32Params++;
    }
    else
    {
        eReturn = CLI_NC_BAD_ARG;
    }


    if (*pu32Params != NULL && eReturn == CLI_NC_OK)
    {
        u32Param2 = atoi((char*)(*pu32Params));
        u32TestParam2 = u32Param2;
    }
    else
    {
        eReturn = CLI_NC_BAD_ARG;
    }

    u32TestSum = u32Param1 + u32Param2;

    return eReturn;
}

CLI_NOTIFY_CODE CopyString(const char* pu8Command, void* pValueList)
{
    CLI_NOTIFY_CODE eReturn = CLI_NC_OK;
    uint32_t* pu32Params = (uint32_t*)pValueList;

    if (*pu32Params)
    {
        pu8TestString = ((char*)(*pu32Params));
    }
    else
    { 
        eReturn = CLI_NC_BAD_ARG;
    }

    return eReturn;
}

CLI_NOTIFY_CODE DoNothing(const char* pu8Command, void* pValueList)
{
    CLI_NOTIFY_CODE eReturn = CLI_NC_OK;
    return eReturn;
}

#endif /* CLI */
}

namespace 
{

#if (CONFIG_CLI == 1U)
    TEST(cli, add_command) 
    {
        uint8_t u8Result;
        u8Result = ezmCli_RegisterCommand(NULL, NULL, NULL);
        ASSERT_EQ(u8Result, CLI_HANDLE_INVALID);

        u8Result = ezmCli_RegisterCommand("Test_Command", NULL, NULL);
        ASSERT_EQ(u8Result, CLI_HANDLE_INVALID);

        u8Result = ezmCli_RegisterCommand(NULL, "this is a test command", &TestCommand);
        ASSERT_EQ(u8Result, CLI_HANDLE_INVALID);

        u8Result = ezmCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
        ASSERT_NE(u8Result, CLI_HANDLE_INVALID);

        u8Result = ezmCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
        ASSERT_EQ(u8Result, CLI_HANDLE_INVALID);
    }

    TEST(cli, add_argument) 
    {
        uint8_t u8Result;
        bool    bResult;

        ezmCli_Init(NULL);
        u8Result = ezmCli_RegisterCommand("Test_Command1", "this is a test command",  &TestCommand);
        ASSERT_NE(u8Result, CLI_HANDLE_INVALID);

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

    TEST(cli, receive_command_one_arg) 
    {
        uint8_t u8Result;
        bool    bResult;

        ezmCli_Init(NULL);
        u8Result = ezmCli_RegisterCommand("Test_Command1", "this is a test command",  &TestCommand);
        ASSERT_NE(u8Result, CLI_HANDLE_INVALID);

        bResult = ezmCli_AddArgument(u8Result, "--arg1", "-a1", "argument 1");
        ASSERT_EQ(bResult, true);

        memcpy(au8CommandBuffer, "Test_Command --arg1 1234\n", sizeof("Test_Command --arg1 1234\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, false);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "Test_Command1 --arg1 1234\n", sizeof("Test_Command1 --arg1 1234\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, true);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "Test_Command1 -a1 1234\n", sizeof("Test_Command1 -a1 1234\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, true);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "Test_Command1 wrong_arg 1234\n", sizeof("Test_Command1 wrong_arg 1234\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, false);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "Test_Command1 -arg1 1234\n", sizeof("Test_Command1 -arg1 1234\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, false);
        
        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "Test_Command1 --a1 1234\n", sizeof("Test_Command1 --a1 1234\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, false);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "Test_Command1 --arg1\n", sizeof("Test_Command1 --arg1\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, false);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "Test_Command1\n", sizeof("Test_Command1\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, false);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "\n", 1);
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, false);
        
        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "       Test_Command1   -a1    1234\n", sizeof("       Test_Command1   -a1    1234\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, true);
    }

    TEST(cli, test_uint32_data)
    {
        uint8_t u8Result;
        bool    bResult;

        ezmCli_Init(NULL);
        u8Result = ezmCli_RegisterCommand("sum", "sum of 2 unsigned interger", &Sum);
        ASSERT_NE(u8Result, CLI_HANDLE_INVALID);

        bResult = ezmCli_AddArgument(u8Result, "--int1", "-i1", "integer 1");
        ASSERT_EQ(bResult, true);

        bResult = ezmCli_AddArgument(u8Result, "--int2", "-i2", "integer 2");
        ASSERT_EQ(bResult, true);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "sum -i1 10 -i2 20\n", sizeof("sum -i1 10 -i2 20\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, true);
        ASSERT_EQ(u32TestSum, 30);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "sum -i1 0 -i2 20\n", sizeof("sum -i1 0 -i2 20\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, true);
        ASSERT_EQ(u32TestSum, 20);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "sum -i1 0 -i2 0\n", sizeof("sum -i1 0 -i2 0\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, true);
        ASSERT_EQ(u32TestSum, 0);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "sum -i1 10\n", sizeof("sum -i1 10\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, false);
    }

    TEST(cli, test_string)
    {
        uint8_t u8Result;
        bool    bResult;
        bool    bStringCompareResult;
        
        ezmCli_Init(NULL);
        u8Result = ezmCli_RegisterCommand("string", "copy a string", &CopyString);
        ASSERT_NE(u8Result, CLI_HANDLE_INVALID);

        bResult = ezmCli_AddArgument(u8Result, "--string", "-str", "string value");
        ASSERT_EQ(bResult, true);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "string -str hello_world\n", sizeof("string -str hello_world\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, true);
        bStringCompareResult = strcmp(pu8TestString, "hello_world\0");
        ASSERT_EQ(bStringCompareResult, 0);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "string -str hello world\n", sizeof("string -str hello world\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, true);
        bStringCompareResult = strcmp(pu8TestString, "hello world\0");
        ASSERT_EQ(bStringCompareResult, 0);

        pu8TestString = NULL;
        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "string -str     \n", sizeof("string -str     \n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, false);
    }

    TEST(cli, test_cmd_no_arg)
    {
        uint8_t u8Result;
        bool    bResult;

        ezmCli_Init(NULL);
        u8Result = ezmCli_RegisterCommand("no_arg", "command has no argument", &DoNothing);
        ASSERT_NE(u8Result, CLI_HANDLE_INVALID);

        memset(au8CommandBuffer, 0, sizeof(au8CommandBuffer));
        memcpy(au8CommandBuffer, "no_arg\n", sizeof("no_arg\n"));
        bResult = ezmCli_CommandReceivedCallback(0, au8CommandBuffer, sizeof(au8CommandBuffer));
        ASSERT_EQ(bResult, true);
    }

#endif /* CLI */
}


#endif /* UNIT_TEST_ASSERT */