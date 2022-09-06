
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

#include <string.h>

/*the rest of include go here*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define TEST_BUFF_SIZE      2048
#define CRC_SIZE            4U

/******************************************************************************
* Module Typedefs
*******************************************************************************/
struct TestProfile
{
    uint32_t test_count;
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
uint8_t test_rpc_buff[TEST_BUFF_SIZE] = { 0 };

uint8_t data_stream_1[] = {
    /* first response */
    0x80, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0xbe, 0xef, 0xca, 0xfe, 0x00, 0x00, 0x03, 0x75,
    /* second response */
    0x80, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0xbe, 0xef, 0xca, 0xfe, 0x00, 0x00, 0x03, 0x75,
    /* third response */
    0x80, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0xbe, 0xef, 0xca, 0xfe, 0x00, 0x00, 0x03, 0x75,
};


struct ezRpc test_rpc;
struct TestProfile profile;

/******************************************************************************
* Function Definitions
*******************************************************************************/
void Test_CrcCal(uint8_t *input,
                    uint32_t input_size,
                    uint8_t *crc_output,
                    uint32_t crc_output_size);

bool Test_CrcVerify(uint8_t *input,
                    uint32_t input_size,
                    uint8_t *crc,
                    uint32_t crc_size);

uint32_t TestTransmit(uint8_t *data, uint32_t size);
uint32_t TestReceive(uint8_t *data, uint32_t size);

void TestReceiveService1(void *payload, uint32_t payload_size_byte);

void Test_ezRpc_Initialization_InvalidParams(void);
void Test_ezRpc_Initialization_ValidParams(void);
void Test_ezRpc_SetCrcFunctions_InvalidParams(void);
void Test_ezRpc_SetCrcFunctions_ValidParams(void);
void Test_ezRPC_CreateRpcMessage_InvalidParam(void);
void Test_ezRPC_CreateRpcMessage_WithNoPayload(void);
void Test_ezRPC_CreateRpcMessage_OverflowRecords(void);
void Test_ezRPC_Run_NumOfValidMessageReceived(void);

struct ezRpcService services[] = {
    {0x01, TestReceiveService1}
};


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
    RUN_TEST(Test_ezRPC_CreateRpcMessage_InvalidParam);
    RUN_TEST(Test_ezRPC_CreateRpcMessage_WithNoPayload);
    RUN_TEST(Test_ezRPC_CreateRpcMessage_OverflowRecords);
    RUN_TEST(Test_ezRPC_Run_NumOfValidMessageReceived);

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


    crc_output[0] = sum >> 24;
    crc_output[1] = (sum >> 16) & 0xFF;
    crc_output[2] = (sum >> 8) & 0xFF;;
    crc_output[3] = sum & 0xFF;
}


bool Test_CrcVerify(uint8_t *input,
                    uint32_t input_size,
                    uint8_t *crc,
                    uint32_t crc_size)
{
    uint8_t crc_cal_out[CRC_SIZE] = {0};

    Test_CrcCal(input, input_size, crc_cal_out, CRC_SIZE);

    return (crc_size == CRC_SIZE && memcmp(crc_cal_out, crc, CRC_SIZE) == 0);
}


uint32_t TestTransmit(uint8_t *data, uint32_t size)
{
    uint32_t transmitted_byte = size;

    DEBUG("transmit: ");
    HEXDUMP(data, size);

    return transmitted_byte;
}

uint32_t TestReceive(uint8_t *data, uint32_t size)
{
    static uint32_t curr_index = 0;
    uint32_t ret_size = 0;

    for (uint32_t i = 0; i < size && curr_index < sizeof(data_stream_1); i++)
    {
        *(data + i) = data_stream_1[curr_index];
        curr_index++;
        ret_size = size;
    }

    return ret_size;
}

void TestReceiveService1(void *payload, uint32_t payload_size_byte)
{
    DEBUG("Receive echo response:");
    HEXDUMP(payload, payload_size_byte);
}


void Test_ezRpc_Initialization_InvalidParams(void)
{
    ezSTATUS status;
    bool is_rpc_ready = false;

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
                                    services,
                                    0);

    TEST_ASSERT_EQUAL(ezFAIL, status);

    is_rpc_ready = ezRpc_IsRpcInstanceReady(&test_rpc);

    TEST_ASSERT_EQUAL(false, is_rpc_ready);
}


void Test_ezRpc_Initialization_ValidParams(void)
{
    ezSTATUS status;
    bool is_rpc_ready = false;

    status = ezRpc_Initialization(&test_rpc,
                                    test_rpc_buff,
                                    TEST_BUFF_SIZE,
                                    services,
                                    sizeof(services)/sizeof(struct ezRpcService));

    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    ezRpc_SetTxRxFunctions(&test_rpc, TestTransmit, TestReceive);

    is_rpc_ready = ezRpc_IsRpcInstanceReady(&test_rpc);

    TEST_ASSERT_EQUAL(true, is_rpc_ready);
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


void Test_ezRPC_CreateRpcMessage_InvalidParam(void)
{
    ezSTATUS status;

    status = ezRPC_CreateRpcMessage(NULL, RPC_MSG_REQ, 0xaa, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezRPC_CreateRpcMessage(NULL, RPC_MSG_RESP, 0xaa, NULL, 4);
    TEST_ASSERT_EQUAL(ezFAIL, status);
}


void Test_ezRPC_CreateRpcMessage_WithNoPayload(void)
{
    ezSTATUS status;
    uint32_t num_of_pendign_msg = 0;

    status = ezRPC_CreateRpcMessage(&test_rpc, RPC_MSG_REQ, 0xaa, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezRPC_CreateRpcMessage(&test_rpc, RPC_MSG_RESP, 0xab, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    num_of_pendign_msg = ezRPC_NumOfTxPendingMsg(&test_rpc);
    TEST_ASSERT_EQUAL(2, num_of_pendign_msg);

    /* reset RPC */
    status = ezRpc_Initialization(&test_rpc,
                                    test_rpc_buff,
                                    TEST_BUFF_SIZE,
                                    services,
                                    sizeof(services) / sizeof(struct ezRpcService));

    TEST_ASSERT_EQUAL(ezSUCCESS, status);
}


void Test_ezRPC_CreateRpcMessage_OverflowRecords(void)
{
    ezSTATUS status;
    uint32_t num_of_pendign_msg = 0;

    for (uint32_t i = 0; i < CONFIG_NUM_OF_REQUEST; i++)
    {
        status = ezRPC_CreateRpcMessage(&test_rpc, RPC_MSG_REQ, 0xaa, NULL, 0);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);
    }

    num_of_pendign_msg = ezRPC_NumOfTxPendingMsg(&test_rpc);
    TEST_ASSERT_EQUAL(CONFIG_NUM_OF_REQUEST, num_of_pendign_msg);

    status = ezRPC_CreateRpcMessage(&test_rpc, RPC_MSG_REQ, 0xaa, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    num_of_pendign_msg = ezRPC_NumOfTxPendingMsg(&test_rpc);
    TEST_ASSERT_EQUAL(CONFIG_NUM_OF_REQUEST, num_of_pendign_msg);


    /* reset RPC */
    status = ezRpc_Initialization(&test_rpc,
                                    test_rpc_buff,
                                    TEST_BUFF_SIZE,
                                    services,
                                    sizeof(services) / sizeof(struct ezRpcService));

    TEST_ASSERT_EQUAL(ezSUCCESS, status);
}


void Test_ezRPC_Run_NumOfValidMessageReceived(void)
{
    ezSTATUS status;
    bool is_rpc_ready = true;

    profile.test_count = 0;

    status = ezRpc_Initialization(&test_rpc,
                                    test_rpc_buff,
                                    TEST_BUFF_SIZE,
                                    services,
                                    sizeof(services) / sizeof(struct ezRpcService));

    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    ezRpc_SetTxRxFunctions(&test_rpc, TestTransmit, TestReceive);

    is_rpc_ready = ezRpc_IsRpcInstanceReady(&test_rpc);

    TEST_ASSERT_EQUAL(true, is_rpc_ready);

    status = ezRpc_SetCrcFunctions(&test_rpc,
                                    CRC_SIZE,
                                    Test_CrcVerify,
                                    Test_CrcCal);

    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezRPC_CreateRpcMessage(&test_rpc, RPC_MSG_REQ, 0xaa, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezRPC_CreateRpcMessage(&test_rpc, RPC_MSG_REQ, 0xaa, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezRPC_CreateRpcMessage(&test_rpc, RPC_MSG_REQ, 0xaa, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    TEST_ASSERT_EQUAL(3, ezRPC_NumOfTxPendingMsg(&test_rpc));

    for (uint32_t i = 0; i < 180; i++)
    {
        ezRPC_Run(&test_rpc);
    }

    TEST_ASSERT_EQUAL(0, ezRPC_NumOfTxPendingMsg(&test_rpc));
}


#endif /* CONFIG_EZ_RPC_TEST == 1U */
/* End of file*/

