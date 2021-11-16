#include <limits.h>
#include "gtest/gtest.h"

#ifndef EZMIPC_UNIT_TEST
#define EZMIPC_UNIT_TEST

extern "C" {
#include "../../ezmsdk/ezmIpc/ezmIpc.h"
}

namespace 
{
#if (IPC == 1U)
    TEST(Ipc, main) 
    {
        ezmIpc_Init();
        uint8_t au8Payload1[3] = {0x01, 0x02, 0x03};
        uint8_t au8Payload2[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
        ezmMemoryBlock * pstMsg;
        bool bResult;

        bResult = ezmIpc_RegisterModule(0xFE);
        ASSERT_EQ(bResult, true);

        bResult = ezmIpc_RegisterModule(0xFF);
        ASSERT_EQ(bResult, true);

        pstMsg = ezmIpc_InitMsg(0x03);
        ASSERT_TRUE(pstMsg != nullptr);
        ASSERT_EQ(pstMsg->u16BufferSize, 0x03);
        
        memcpy(pstMsg->pBuffer, au8Payload1, 0x03);
        ASSERT_EQ(memcmp(pstMsg->pBuffer, au8Payload1, 0x03), 0);
        bResult = ezmIpc_SendMsg(0xFE, 0xFF, pstMsg);
        ASSERT_EQ(bResult, true);

        pstMsg = ezmIpc_InitMsg(0x05);
        ASSERT_TRUE(pstMsg != nullptr);
        ASSERT_EQ(pstMsg->u16BufferSize, 0x05);
        
        
        memcpy(pstMsg->pBuffer, au8Payload2, 0x05);
        ASSERT_EQ(memcmp(pstMsg->pBuffer, au8Payload2, 0x05), 0);
        bResult = ezmIpc_SendMsg(0xFE, 0xFF, pstMsg);
        ASSERT_EQ(bResult, true);

        pstMsg = ezmIpc_ReceiveMsg(0xFF);
        ASSERT_TRUE(pstMsg != nullptr);
        ASSERT_EQ(pstMsg->u16BufferSize, 0x03);
        ASSERT_EQ(memcmp(pstMsg->pBuffer, au8Payload1, 0x03), 0);
        bResult = ezmIpc_DeInitMsg(0xFF, pstMsg);

        pstMsg = ezmIpc_ReceiveMsg(0xFF);
        ASSERT_TRUE(pstMsg != nullptr);
        ASSERT_EQ(pstMsg->u16BufferSize, 0x05);
        ASSERT_EQ(memcmp(pstMsg->pBuffer, au8Payload2, 0x05), 0);
        bResult = ezmIpc_DeInitMsg(0xFF, pstMsg);

        pstMsg = ezmIpc_ReceiveMsg(0xFF);
        ASSERT_TRUE(pstMsg == nullptr);
    }
#endif
}
#endif /* EZMIPC_UNIT_TEST */