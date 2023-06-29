
/*******************************************************************************
* Filename:         ez_hal_uart.c
* Author:           Hai Nguyen
* Original Date:    25.06.2023
* Last Update:      25.06.2023
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
* Written by Hai Nguyen 25.06.2023
*
*******************************************************************************/

/** @file   ez_hal_uart.c
 *  @author Hai Nguyen
 *  @date   25.06.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "unity.h"
#include "unity_fixture.h"
#include "service/driver/hal_uart/ez_hal_uart.h"
#include "service/driver/hal_uart/ez_target_uart_def.h"

TEST_GROUP(ez_hal_uart);


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static ezDriverHandle_t driver1;
static ezDriverHandle_t driver2;

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezHalUart_TestCallback1(uint8_t cb_code,
                                    void *param1,
                                    void *param2)
{
}


static void ezHalUart_TestCallback2(uint8_t cb_code,
                                    void *param1,
                                    void *param2)
{
}


TEST_SETUP(ez_hal_uart)
{
    ezHalUart_Initialize();
}


TEST_TEAR_DOWN(ez_hal_uart)
{
    ezHalUart_Deinitialize();
}


TEST(ez_hal_uart, ezHalUart_GetDriver_Correct)
{
    ezDriverStatus_t status = EZ_DRIVER_ERR_NO_DRIVER;
    status = ezHalUart_GetDriver(UART1_NAME, &driver1, ezHalUart_TestCallback1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);
}


TEST(ez_hal_uart, ezHalUart_GetDriver_Incorrect)
{
    ezDriverStatus_t status = EZ_DRIVER_OK;
    status = ezHalUart_GetDriver("", &driver1, ezHalUart_TestCallback1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_ERR_NO_DRIVER, status);

    status = EZ_DRIVER_OK;
    status = ezHalUart_GetDriver("Test Wrong Name", &driver1, ezHalUart_TestCallback1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_ERR_NO_DRIVER, status);
}


TEST(ez_hal_uart, ezHalUart_Write_Without_Get_Driver)
{
    uint32_t num_byte_written = 0;
    uint8_t test_written_buff[128] = { 0xca };

    num_byte_written = ezHalUart_WriteBlocking(&driver1, test_written_buff, 128);
    TEST_ASSERT_EQUAL(0, num_byte_written);
}


TEST(ez_hal_uart, ezHalUart_ReleaseDriver)
{
    ezDriverStatus_t status = EZ_DRIVER_ERR_GENERIC;
    uint32_t num_byte_written = 0;
    uint8_t test_written_buff[128] = { 0xca };

    status = ezHalUart_GetDriver(UART1_NAME, &driver1, ezHalUart_TestCallback1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    status = EZ_DRIVER_ERR_GENERIC;
    status = ezHalUart_ReleaseDriver(&driver1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    num_byte_written = ezHalUart_WriteBlocking(&driver1, test_written_buff, 128);
    TEST_ASSERT_EQUAL(0, num_byte_written);
}


TEST(ez_hal_uart, UsingUnsupportedAPI)
{
    ezDriverStatus_t status = EZ_DRIVER_ERR_GENERIC;
    uint32_t num_byte_written = 0;
    uint32_t num_byte_read = 0;

    uint8_t test_written_buff[128] = { 0xca };
    uint8_t test_read_buff[128] = { 0xff };

    memset(test_written_buff, 0xca, 128);
    memset(test_read_buff, 0xff, 128);

    status = ezHalUart_GetDriver(UART1_NAME, &driver1, ezHalUart_TestCallback1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);


    num_byte_written = ezHalUart_Write(&driver1, test_written_buff, 128);
    TEST_ASSERT_EQUAL(0, num_byte_written);

    num_byte_read = ezHalUart_Read(&driver1, test_read_buff, 128);
    TEST_ASSERT_EQUAL(0, num_byte_read);
}

TEST(ez_hal_uart, ezHalUart_Write)
{
    ezDriverStatus_t status = EZ_DRIVER_ERR_GENERIC;
    uint32_t num_byte_written = 0;
    uint32_t num_byte_read = 0;

    uint8_t test_written_buff[128] = { 0xca };
    uint8_t test_read_buff[128] = { 0xff };

    memset(test_written_buff, 0xca, 128);
    memset(test_read_buff, 0xff, 128);

    status = ezHalUart_GetDriver(UART0_NAME, &driver1, ezHalUart_TestCallback1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    status = ezHalUart_GetDriver(UART1_NAME, &driver2, ezHalUart_TestCallback2);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    num_byte_written = ezHalUart_WriteBlocking(&driver1, test_written_buff, 128);
    TEST_ASSERT_EQUAL(128, num_byte_written);

    num_byte_read = ezHalUart_ReadBlocking(&driver2, test_read_buff, 128);
    TEST_ASSERT_EQUAL(128, num_byte_read);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_written_buff, test_read_buff, 128);
}


TEST(ez_hal_uart, ezHalUart_WriteReadSequencial)
{
    ezDriverStatus_t status = EZ_DRIVER_ERR_GENERIC;
    uint32_t num_byte_written = 0;
    uint32_t num_byte_read = 0;

    uint8_t test_written_buff1[128] = { 0xca };
    uint8_t test_written_buff2[128] = { 0xca };
    uint8_t test_written_buff3[128] = { 0xca };
    uint8_t test_read_buff[128] = { 0xff };

    memset(test_written_buff1, 0xca, 128);
    memset(test_written_buff2, 0xbe, 128);
    memset(test_written_buff3, 0xde, 128);

    status = ezHalUart_GetDriver(UART0_NAME, &driver1, ezHalUart_TestCallback1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    status = ezHalUart_GetDriver(UART1_NAME, &driver2, ezHalUart_TestCallback2);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    /* first sequence */
    num_byte_written = ezHalUart_WriteBlocking(&driver1, test_written_buff1, 128);
    TEST_ASSERT_EQUAL(128, num_byte_written);

    memset(test_read_buff, 0xff, 128);
    num_byte_read = ezHalUart_ReadBlocking(&driver2, test_read_buff, 128);
    TEST_ASSERT_EQUAL(128, num_byte_read);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_written_buff1, test_read_buff, 128);

    /* second sequence */
    num_byte_written = ezHalUart_WriteBlocking(&driver1, test_written_buff2, 128);
    TEST_ASSERT_EQUAL(128, num_byte_written);

    memset(test_read_buff, 0xff, 128);
    num_byte_read = ezHalUart_ReadBlocking(&driver2, test_read_buff, 128);
    TEST_ASSERT_EQUAL(128, num_byte_read);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_written_buff2, test_read_buff, 128);

    /* third sequence */
    num_byte_written = ezHalUart_WriteBlocking(&driver1, test_written_buff3, 128);
    TEST_ASSERT_EQUAL(128, num_byte_written);

    memset(test_read_buff, 0xff, 128);
    num_byte_read = ezHalUart_ReadBlocking(&driver2, test_read_buff, 128);
    TEST_ASSERT_EQUAL(128, num_byte_read);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_written_buff3, test_read_buff, 128);
}


TEST(ez_hal_uart, ezHalUart_WriteReadBlock)
{
    ezDriverStatus_t status = EZ_DRIVER_ERR_GENERIC;
    uint32_t num_byte_written = 0;
    uint32_t num_byte_read = 0;

    uint8_t test_written_buff1[128] = { 0xca };
    uint8_t test_written_buff2[128] = { 0xca };
    uint8_t test_written_buff3[128] = { 0xca };
    uint8_t test_read_buff[128 * 3] = { 0xff };

    memset(test_written_buff1, 0xca, 128);
    memset(test_written_buff2, 0xbe, 128);
    memset(test_written_buff3, 0xde, 128);

    status = ezHalUart_GetDriver(UART0_NAME, &driver1, ezHalUart_TestCallback1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    status = ezHalUart_GetDriver(UART1_NAME, &driver2, ezHalUart_TestCallback2);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    /* first sequence */
    num_byte_written = ezHalUart_WriteBlocking(&driver1, test_written_buff1, 128);
    TEST_ASSERT_EQUAL(128, num_byte_written);

    num_byte_written = ezHalUart_WriteBlocking(&driver1, test_written_buff2, 128);
    TEST_ASSERT_EQUAL(128, num_byte_written);

    num_byte_written = ezHalUart_WriteBlocking(&driver1, test_written_buff3, 128);
    TEST_ASSERT_EQUAL(128, num_byte_written);


    memset(test_read_buff, 0xff, 128 * 3);
    num_byte_read = ezHalUart_ReadBlocking(&driver2, test_read_buff, 128 * 3);
    TEST_ASSERT_EQUAL(128 * 3, num_byte_read);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_written_buff1, test_read_buff, 128);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_written_buff2, &test_read_buff[128], 128);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(test_written_buff3, &test_read_buff[256], 128);
}

/* End of file */

