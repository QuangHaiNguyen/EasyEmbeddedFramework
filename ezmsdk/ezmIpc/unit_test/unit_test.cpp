#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "../ezmIpc.h"
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

        bResult = ezmIpc_RegisterModule(0x01);
        ASSERT_EQ(bResult, true);

        bResult = ezmIpc_RegisterModule(0x02);
        ASSERT_EQ(bResult, true);

        pstMsg = ezmIpc_InitMsg(0x03);
        ASSERT_TRUE(pstMsg != nullptr);
        ASSERT_EQ(pstMsg->u16BufferSize, 0x03);
        
        memcpy(pstMsg->pBuffer, au8Payload1, 0x03);
        ASSERT_EQ(memcmp(pstMsg->pBuffer, au8Payload1, 0x03), 0);
        bResult = ezmIpc_SendMsg(0x01, 0x02, pstMsg);
        ASSERT_EQ(bResult, true);

        pstMsg = ezmIpc_InitMsg(0x05);
        ASSERT_TRUE(pstMsg != nullptr);
        ASSERT_EQ(pstMsg->u16BufferSize, 0x05);
        
        
        memcpy(pstMsg->pBuffer, au8Payload2, 0x05);
        ASSERT_EQ(memcmp(pstMsg->pBuffer, au8Payload2, 0x05), 0);
        bResult = ezmIpc_SendMsg(0x01, 0x02, pstMsg);
        ASSERT_EQ(bResult, true);

        pstMsg = ezmIpc_ReceiveMsg(0x02);
        ASSERT_TRUE(pstMsg != nullptr);
        ASSERT_EQ(pstMsg->u16BufferSize, 0x03);
        ASSERT_EQ(memcmp(pstMsg->pBuffer, au8Payload1, 0x03), 0);
        bResult = ezmIpc_DeInitMsg(0x02, pstMsg);

        pstMsg = ezmIpc_ReceiveMsg(0x02);
        ASSERT_TRUE(pstMsg != nullptr);
        ASSERT_EQ(pstMsg->u16BufferSize, 0x05);
        ASSERT_EQ(memcmp(pstMsg->pBuffer, au8Payload2, 0x05), 0);
        bResult = ezmIpc_DeInitMsg(0x02, pstMsg);

        pstMsg = ezmIpc_ReceiveMsg(0x02);
        ASSERT_TRUE(pstMsg == nullptr);
    }
#endif
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}