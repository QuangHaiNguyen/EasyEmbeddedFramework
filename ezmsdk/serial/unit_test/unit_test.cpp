#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "hal_uart.h"
#include <stdio.h>
#include <string.h>
}

namespace 
{
    TEST(Serial, Init) 
    {
        HAL_Status eStatus = HAL_ERR;
        HAL_UART str_Uart;
        
        eStatus = HAL_UART_GetConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);

        eStatus = str_Uart.HAL_UART_Init(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);

        eStatus = HAL_UART_DeConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);
    }

    TEST(Serial, Serial_receive_no_mem) 
    {
        HAL_Status eStatus = HAL_ERR;
        HAL_UART str_Uart;

       eStatus = HAL_UART_GetConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);

        eStatus = str_Uart.HAL_UART_Init(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);

        eStatus = str_Uart.HAL_UART_Read(&str_Uart, 30);
        EXPECT_EQ(eStatus, HAL_NO_MEM);

        eStatus = HAL_UART_DeConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);
    }

    TEST(Serial, Serial_receive_10_byte) 
    {

        HAL_Status eStatus = HAL_ERR;
        HAL_UART str_Uart;
        
        char result[10] = {'q','w','e','r','t','a','s','d','f','g'};


        eStatus = HAL_UART_GetConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);

        eStatus = str_Uart.HAL_UART_Init(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);
        
        printf("Please type qwertasdfg\n");
        eStatus = str_Uart.HAL_UART_Read(&str_Uart, 10);
        EXPECT_EQ(eStatus, HAL_OK);

        int ret =  memcmp(str_Uart.ptr_u8Buffer, (uint8_t *)result, 10);
        EXPECT_EQ(ret, 0);

        eStatus = HAL_UART_DeConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);
    }

    TEST(Serial, Serial_Transmit_no_mem) 
    {
        HAL_Status eStatus = HAL_ERR;
        HAL_UART str_Uart;
        
        eStatus = HAL_UART_GetConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);
        
        eStatus =str_Uart.HAL_UART_Write(&str_Uart,30);
        EXPECT_EQ(eStatus, HAL_NO_MEM);

        eStatus = HAL_UART_DeConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);
    }

    TEST(Serial, Serial_Transmit_10_bytes) 
    {
        HAL_Status eStatus = HAL_ERR;
        HAL_UART str_Uart;
        char result[10] = {'q','w','e','r','t','a','s','d','f','g'};
        
        eStatus = HAL_UART_GetConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);
        
        memcpy(str_Uart.ptr_u8Buffer, result, 10);
        eStatus =str_Uart.HAL_UART_Write(&str_Uart,10);
        EXPECT_EQ(eStatus, HAL_OK);

        eStatus = HAL_UART_DeConfig(&str_Uart);
        EXPECT_EQ(eStatus, HAL_OK);
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}