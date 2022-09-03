
/*******************************************************************************
* Filename:         test_ezRpc.c
* Author:           Hai Nguyen
* Original Date:    01.09.2022
* Last Update:      01.09.2022
*
* -----------------------------------------------------------------------------
* Company:          Embedded Easy
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Embedded Easy
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 01.09.2022
*
*******************************************************************************/

/** @file   test_ezRpc.c
 *  @author Hai Nguyen
 *  @date   01.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (CONFIG_EZ_RPC_TEST == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "test_ezRpc"       /**< module name */
#include "utilities/logging/logging.h"

#include "app/app.h"
#include "ezRpc/ezRpc.h"
#include "unity_test_platform/unity.h"

/*the rest of include go here*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define TEST_BUFF_SIZE      256U
#define CRC_SIZE            1U

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
uint8_t test_rpc_buff[TEST_BUFF_SIZE] = { 0 };
struct ezRpcService offered_services[] = {
    {0xFF, NULL},
};

struct ezRpc test_rpc;


/******************************************************************************
* Function Definitions
*******************************************************************************/
void Test_CrcCal(uint8_t *input,
                    uint32_t input_size,
                    uint8_t *crc_output,
                    uint32_t crc_output_size);
bool Test_CrcVerify(struct ezRpcMsg *msg);

void Test_ezRpc_Initialization_InvalidParams(void);
void Test_ezRpc_Initialization_ValidParams(void);
void Test_ezRpc_SetCrcFunctions_InvalidParams(void);
void Test_ezRpc_SetCrcFunctions_ValidParams(void);


/******************************************************************************
* External functions
*******************************************************************************/

int main(void)
{
    ezmApp_SdkInit();

    UnityBegin("test/test_ezRpc.c");

    /* Note, must be called in order */
    RUN_TEST(Test_ezRpc_Initialization_InvalidParams);
    RUN_TEST(Test_ezRpc_Initialization_ValidParams);
    RUN_TEST(Test_ezRpc_SetCrcFunctions_InvalidParams);
    RUN_TEST(Test_ezRpc_SetCrcFunctions_ValidParams);

    return (UnityEnd());
}

/******************************************************************************
* Internal functions
*******************************************************************************/
void Test_CrcCal(uint8_t *input,
                    uint32_t input_size,
                    uint8_t *crc_output,
                    uint32_t crc_output_size)
{
    uint32_t sum = 0;
    for (uint32_t i = 0; i < input_size; i++)
    {
        sum += *input;
        input++;
    }

    *crc_output = (uint8_t)sum;
}


bool Test_CrcVerify(struct ezRpcMsg *msg)
{
    return true;
}


void Test_ezRpc_Initialization_InvalidParams(void)
{
    ezSTATUS status;

    status = ezRpc_Initialization(&test_rpc,
                                    NULL,
                                    0,
                                    NULL,
                                    0);

    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezRpc_Initialization(&test_rpc,
                                    test_rpc_buff,
                                    0,
                                    NULL,
                                    0);

    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezRpc_Initialization(&test_rpc,
                                    test_rpc_buff,
                                    TEST_BUFF_SIZE,
                                    NULL,
                                    0);

    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezRpc_Initialization(&test_rpc,
                                    test_rpc_buff,
                                    TEST_BUFF_SIZE,
                                    offered_services,
                                    0);

    TEST_ASSERT_EQUAL(ezFAIL, status);
}


void Test_ezRpc_Initialization_ValidParams(void)
{
    ezSTATUS status;

    status = ezRpc_Initialization(&test_rpc,
                                    test_rpc_buff,
                                    TEST_BUFF_SIZE,
                                    offered_services,
                                    sizeof(offered_services)/sizeof(struct ezRpcService));

    TEST_ASSERT_EQUAL(ezSUCCESS, status);
}


void Test_ezRpc_SetCrcFunctions_InvalidParams(void)
{
    ezSTATUS status;

    status = ezRpc_SetCrcFunctions(&test_rpc,
                                    0,
                                    NULL,
                                    NULL);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezRpc_SetCrcFunctions(&test_rpc,
                                    CRC_SIZE,
                                    NULL,
                                    NULL);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezRpc_SetCrcFunctions(&test_rpc,
                                    0,
                                    Test_CrcVerify,
                                    NULL);
    TEST_ASSERT_EQUAL(ezFAIL, status);
}


void Test_ezRpc_SetCrcFunctions_ValidParams(void)
{
    ezSTATUS status;

    status = ezRpc_SetCrcFunctions(&test_rpc,
                                    CRC_SIZE,
                                    Test_CrcVerify,
                                    Test_CrcCal);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);
}
#endif /* CONFIG_EZ_RPC_TEST == 1U */
/* End of file*/

