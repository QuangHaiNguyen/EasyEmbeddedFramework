#include <limits.h>
#include "gtest/gtest.h"

#ifndef EZMIPC_UNIT_TEST
#define EZMIPC_UNIT_TEST

extern "C"
{
#include "../../ezmsdk/ezmIpc/ezmIpc.h"
#include "../../ezmsdk/helper/stcmem/stcmem.h"

    uint8_t ipc_buffer[32U] = { 0 };
    uint8_t callback_count = 0;
    uint32_t MessageRcvCallback(void)
    {
        callback_count++;
        return 0;
    }
}

namespace 
{
#if (IPC == 1U)
    TEST(Ipc, main) 
    {
        ezmStcMem_Initialization();
        ezmIpc_InitModule();

        bool is_success;
        uint8_t compare;
        ezmIpc test_ipc;

        test_ipc = ezmIpc_GetInstance(ipc_buffer, 32, &MessageRcvCallback);
        ASSERT_LT(test_ipc, IPC_INVALID);

        uint8_t* buffer = (uint8_t*)ezmIpc_InitMessage(test_ipc, 16);
        EXPECT_NE(buffer, nullptr);

        memset(buffer, 1, 16);
        compare = memcmp(buffer, &ipc_buffer[0], 16);
        ASSERT_EQ(0, compare);

        is_success = ezmIpc_SendMessage(test_ipc, buffer);
        ASSERT_EQ(true, is_success);
        ASSERT_EQ(1, callback_count);

        buffer = (uint8_t*)ezmIpc_InitMessage(test_ipc, 8);
        EXPECT_NE(buffer, nullptr);

        memset(buffer, 2, 8);
        compare = memcmp(buffer, &ipc_buffer[16], 8);
        ASSERT_EQ(0, compare);

        is_success = ezmIpc_SendMessage(test_ipc, buffer);
        ASSERT_EQ(true, is_success);
        ASSERT_EQ(2, callback_count);

        buffer = (uint8_t*)ezmIpc_InitMessage(test_ipc, 8);
        EXPECT_NE(buffer, nullptr);

        memset(buffer, 3, 8);
        compare = memcmp(buffer, &ipc_buffer[24], 8);
        ASSERT_EQ(0, compare);

        is_success = ezmIpc_SendMessage(test_ipc, buffer);
        ASSERT_EQ(true, is_success);
        ASSERT_EQ(3, callback_count);

        uint16_t message_size;
        buffer = (uint8_t*)ezmIpc_ReceiveMessage(test_ipc, &message_size);
        compare = memcmp(buffer, &ipc_buffer[0], message_size);
        ASSERT_EQ(0, compare);
        EXPECT_NE(buffer, nullptr);
        ASSERT_EQ(16, message_size);

        is_success = ezmIpc_ReleaseMessage(test_ipc, buffer);
        ASSERT_EQ(true, is_success);

        buffer = (uint8_t*)ezmIpc_ReceiveMessage(test_ipc, &message_size);
        compare = memcmp(buffer, &ipc_buffer[16], message_size);
        ASSERT_EQ(0, compare);
        EXPECT_NE(buffer, nullptr);
        ASSERT_EQ(8, message_size);

        is_success = ezmIpc_ReleaseMessage(test_ipc, buffer);
        ASSERT_EQ(true, is_success);

        buffer = (uint8_t*)ezmIpc_ReceiveMessage(test_ipc, &message_size);
        compare = memcmp(buffer, &ipc_buffer[24], message_size);
        ASSERT_EQ(0, compare);
        EXPECT_NE(buffer, nullptr);
        ASSERT_EQ(8, message_size);

        is_success = ezmIpc_ReleaseMessage(test_ipc, buffer);
        ASSERT_EQ(true, is_success);
    }
#endif
}
#endif /* EZMIPC_UNIT_TEST */