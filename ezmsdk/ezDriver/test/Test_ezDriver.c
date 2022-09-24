
/*******************************************************************************
* Filename:         ezDriver.c
* Author:           Hai Nguyen
* Original Date:    24.09.2022
* Last Update:      24.09.2022
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
* Written by Hai Nguyen 24.09.2022
*
*******************************************************************************/

/** @file   ezDriver.c
 *  @author Hai Nguyen
 *  @date   24.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/

#include "app/app_config.h"

#if (CONFIG_DRIVERINF_TEST == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ezDriver"       /**< module name */

#include "utilities/logging/logging.h"
#include "ezDriver/ezDriver.h"
#include "ezKernel/ezKernel.h"
#include "ezDriver/dummy_driver.h"

#include "unity_test_platform/unity.h"
#include "unity_test_platform/unity_fixture.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>



TEST_GROUP(ezDriver);


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define BUFF_SIZE       256

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint8_t read_buff[BUFF_SIZE] = { 0 };
static uint8_t tx_cmplt_count = 0;
static uint8_t rx_cmplt_count = 0;

static uint8_t write_buff1[BUFF_SIZE] = { 0 };
static uint8_t write_buff2[BUFF_SIZE] = { 0 };
static uint8_t write_buff3[BUFF_SIZE] = { 0 };
static uint8_t write_buff4[BUFF_SIZE] = { 0 };


/******************************************************************************
* Function Definitions
*******************************************************************************/
static int DummyCallback(uint8_t cb_code, void *param1, void *param2);


/******************************************************************************
* External functions
*******************************************************************************/
/* None */


/******************************************************************************
* Internal functions
*******************************************************************************/


static int DummyCallback(uint8_t cb_code, void *param1, void *param2)
{
    if (cb_code == CODE_TX_CMPLT)
    {
        tx_cmplt_count++;
    }
    else if (cb_code == CODE_RX_CMPLT)
    {
        if (*(uint32_t *)param1 == BUFF_SIZE)
        {
            rx_cmplt_count++;
        }
    }
    return 0;
}


TEST_SETUP(ezDriver)
{
    memset(read_buff, 0, BUFF_SIZE);
    tx_cmplt_count = 0;
    memset(write_buff1, 0xCA, BUFF_SIZE);
    memset(write_buff2, 0xFE, BUFF_SIZE);
    memset(write_buff3, 0xBE, BUFF_SIZE);
    memset(write_buff4, 0xEF, BUFF_SIZE);
}


TEST_TEAR_DOWN(ezDriver)
{
}


TEST(ezDriver, Test_ezDriver_GetDriver_WrongDriverName)
{
    ezDriveHandle handle = NULL;

    handle = ezDriver_GetDriver("UART");
    TEST_ASSERT_EQUAL(NULL, handle);

    handle = ezDriver_GetDriver("Shalala");
    TEST_ASSERT_EQUAL(NULL, handle);

    handle = ezDriver_GetDriver("I2C");
    TEST_ASSERT_EQUAL(NULL, handle);
}


TEST(ezDriver, Test_ezDriver_GetDriver_CorrectDriverName)
{
    ezDriveHandle handle = NULL;

    handle = ezDriver_GetDriver("dummy_driver");
    TEST_ASSERT_NOT_EQUAL(NULL, handle);
}


TEST(ezDriver, Test_ezDriver_Write)
{
    ezDriveHandle handle = NULL;
    uint8_t write_buff[128] = { 0 };
    struct ezStdInterfaceData data = { 0 };
    ezSTATUS status = ezSUCCESS;

    memset(write_buff, 0xFA, 128);

    handle = ezDriver_GetDriver("dummy_driver");
    TEST_ASSERT_NOT_EQUAL(NULL, handle);

    data.callback = DummyCallback;
    data.data = write_buff;
    data.data_size = 128;

    /* Send data four time */

    status = ezDriver_Write(handle , &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    status = ezDriver_Write(handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    status = ezDriver_Write(handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    status = ezDriver_Write(handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL(4, tx_cmplt_count);
}


TEST(ezDriver, Test_ezDriver_Read)
{
    ezDriveHandle handle = NULL;
    struct ezStdInterfaceData data = { 0 };
    uint8_t read_buff[BUFF_SIZE] = { 0 };
    ezSTATUS status = ezSUCCESS;

    memset(read_buff, 0, BUFF_SIZE);

    handle = ezDriver_GetDriver("dummy_driver");
    TEST_ASSERT_NOT_EQUAL(NULL, handle);

    /* read write */
    data.data_size = BUFF_SIZE;
    data.data = write_buff1;
    data.callback = DummyCallback;

    status = ezDriver_Write((void*)handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    data.data_size = BUFF_SIZE;
    data.data = read_buff;
    data.callback = DummyCallback;

    status = ezDriver_Read((void *)handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL_MEMORY(write_buff1, read_buff, BUFF_SIZE);

    /* read write */
    data.data_size = BUFF_SIZE;
    data.data = write_buff2;
    data.callback = DummyCallback;

    status = ezDriver_Write((void *)handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    data.data_size = BUFF_SIZE;
    data.data = read_buff;
    data.callback = DummyCallback;

    status = ezDriver_Read((void *)handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL_MEMORY(write_buff2, read_buff, BUFF_SIZE);

    /* read write */
    data.data_size = BUFF_SIZE;
    data.data = write_buff3;
    data.callback = DummyCallback;

    status = ezDriver_Write((void *)handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    data.data_size = BUFF_SIZE;
    data.data = read_buff;
    data.callback = DummyCallback;

    status = ezDriver_Read((void *)handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL_MEMORY(write_buff3, read_buff, BUFF_SIZE);
        
    /* read write */
    data.data_size = BUFF_SIZE;
    data.data = write_buff4;
    data.callback = DummyCallback;

    status = ezDriver_Write((void *)handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    data.data_size = BUFF_SIZE;
    data.data = read_buff;
    data.callback = DummyCallback;

    status = ezDriver_Read((void *)handle, &data);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL_MEMORY(write_buff4, read_buff, BUFF_SIZE);

    TEST_ASSERT_EQUAL(4, rx_cmplt_count);
}

#endif /*CONFIG_DRIVERINF_TEST == 1U */

/* End of file */

