
/*******************************************************************************
* Filename:         EchoHal.c
* Author:           Hai Nguyen
* Original Date:    18.06.2023
* Last Update:      18.06.2023
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
* Written by Hai Nguyen 18.06.2023
*
*******************************************************************************/

/** @file   EchoHal.c
 *  @author Hai Nguyen
 *  @date   18.06.2023
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
#include "string.h"

#include "service/driver/ez_hal_driver_def.h"
#include "service/driver/hal/ez_hal_echo.h"

#include "unity.h"
#include "unity_fixture.h"


TEST_GROUP(EchoHal);


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
static ezDriverHandle_t handle1;
static ezDriverHandle_t handle2;
static ezDriverHandle_t handle3;

static ezDriverStatus_t status = EZ_DRIVER_OK;
static uint8_t buff_1[32] = { 0xca };
static uint8_t buff_2[64] = { 0xfe };
static uint8_t read_buff[128] = { 0 };

static uint32_t handle1_callback_count = 0;
static uint32_t handle2_callback_count = 0;
static uint32_t handle3_callback_count = 0;


/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/
/* None */


/******************************************************************************
* Internal functions
*******************************************************************************/
static void Handle1Callback(uint8_t cb_code, void *param1, void *param2);
static void Handle2Callback(uint8_t cb_code, void *param1, void *param2);
static void Handle3Callback(uint8_t cb_code, void *param1, void *param2);

static void Handle1Callback(uint8_t cb_code, void *param1, void *param2)
{
    handle1_callback_count++;
}

static void Handle2Callback(uint8_t cb_code, void *param1, void *param2)
{
    handle2_callback_count++;
}

static void Handle3Callback(uint8_t cb_code, void *param1, void *param2)
{
    handle3_callback_count++;
}


TEST_SETUP(EchoHal)
{
    memset(&handle1, 0, sizeof(handle1));
    memset(&handle2, 0, sizeof(handle2));
    memset(&handle3, 0, sizeof(handle3));
    memset(&buff_1, 0xca, sizeof(buff_1));
    memset(&buff_2, 0xfe, sizeof(buff_2));
    memset(&read_buff, 0, sizeof(read_buff));

    handle1_callback_count = 0;
    handle2_callback_count = 0;
    handle3_callback_count = 0;

    ezHalEcho_Initialize();
}


TEST_TEAR_DOWN(EchoHal)
{
}


TEST(EchoHal, ezHalEcho_GetDriver_Correct)
{
    status = ezHalEcho_GetDriver(ECHO_INTERFACE_1_NAME, &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    status = ezHalEcho_GetDriver(ECHO_INTERFACE_2_NAME, &handle2, Handle2Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    TEST_ASSERT_NOT_EQUAL(handle1.driver, NULL);
    TEST_ASSERT_NOT_EQUAL(handle2.driver, NULL);

    TEST_ASSERT_NOT_EQUAL(handle1.callback, NULL);
    TEST_ASSERT_NOT_EQUAL(handle2.callback, NULL);

    if (handle1.driver != NULL)
    {
        TEST_ASSERT_EQUAL_STRING(ECHO_INTERFACE_1_NAME, handle1.driver->name);
    }

    if (handle2.driver != NULL)
    {
        TEST_ASSERT_EQUAL_STRING(ECHO_INTERFACE_2_NAME, handle2.driver->name);
    }
}


TEST(EchoHal, ezHalEcho_GetDriver_Incorrect)
{
    status = ezHalEcho_GetDriver("Random name", &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_ERR_NO_DRIVER, status);
    TEST_ASSERT_EQUAL(NULL, handle1.driver);
    TEST_ASSERT_EQUAL(NULL, handle1.callback);

    status = ezHalEcho_GetDriver("Shalala", &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_ERR_NO_DRIVER, status);
    TEST_ASSERT_EQUAL(NULL, handle1.driver);
    TEST_ASSERT_EQUAL(NULL, handle1.callback);

    status = ezHalEcho_GetDriver("", &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_ERR_NO_DRIVER, status);
    TEST_ASSERT_EQUAL(NULL, handle1.driver);
    TEST_ASSERT_EQUAL(NULL, handle1.callback);

    status = ezHalEcho_GetDriver(NULL, &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_ERR_NO_DRIVER, status);
    TEST_ASSERT_EQUAL(NULL, handle1.driver);
    TEST_ASSERT_EQUAL(NULL, handle1.callback);
}


TEST(EchoHal, ezHalEcho_ReleaseDriver)
{
    status = ezHalEcho_GetDriver(ECHO_INTERFACE_1_NAME, &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    status = ezHalEcho_ReleaseDriver(&handle1);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);
    TEST_ASSERT_EQUAL(NULL, handle1.driver);
    TEST_ASSERT_EQUAL(NULL, handle1.callback);
    TEST_ASSERT_EQUAL(&handle1.node, handle1.node.next);
    TEST_ASSERT_EQUAL(&handle1.node, handle1.node.prev);
}


TEST(EchoHal, ezHalEcho_Write_Incorrect)
{
    uint32_t byte_written = 0xdeadcafe;

    byte_written = ezHalEcho_Write(&handle1, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    status = ezHalEcho_GetDriver(ECHO_INTERFACE_1_NAME, &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_written = ezHalEcho_Write(&handle1, NULL, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_written = ezHalEcho_Write(&handle1, NULL, 0);
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_written = ezHalEcho_Write(&handle1, buff_1, 0);
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_written = ezHalEcho_Write(NULL, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_written = ezHalEcho_Write(NULL, NULL, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_written = ezHalEcho_Write(NULL, NULL, sizeof(0));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    /* interface 2 */
    byte_written = 0xdeadcafe;

    byte_written = ezHalEcho_Write(&handle2, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    status = ezHalEcho_GetDriver(ECHO_INTERFACE_2_NAME, &handle2, Handle2Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_written = ezHalEcho_Write(&handle2, NULL, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_written = ezHalEcho_Write(&handle2, NULL, 0);
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_written = ezHalEcho_Write(&handle2, buff_1, 0);
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_written = ezHalEcho_Write(NULL, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_written = ezHalEcho_Write(NULL, NULL, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_written = ezHalEcho_Write(NULL, NULL, sizeof(0));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);
}


TEST(EchoHal, ezHalEcho_Write_Correct)
{
    uint32_t byte_written = 0xdeadcafe;

    status = ezHalEcho_GetDriver(ECHO_INTERFACE_1_NAME, &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    status = ezHalEcho_GetDriver(ECHO_INTERFACE_1_NAME, &handle3, Handle3Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    byte_written = ezHalEcho_Write(&handle1, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(sizeof(buff_1), byte_written);
    TEST_ASSERT_EQUAL(1, handle1_callback_count);

    byte_written = ezHalEcho_Write(&handle1, buff_2, sizeof(buff_2));
    TEST_ASSERT_EQUAL(sizeof(buff_2), byte_written);
    TEST_ASSERT_EQUAL(2, handle1_callback_count);

    /* Write when buff is full */
    byte_written = ezHalEcho_Write(&handle1, buff_2, sizeof(buff_2));
    TEST_ASSERT_EQUAL(32, byte_written);
    TEST_ASSERT_EQUAL(3, handle1_callback_count);

    byte_written = ezHalEcho_Write(&handle1, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_written);
    TEST_ASSERT_EQUAL(4, handle1_callback_count);
}


TEST(EchoHal, ezHalEcho_Read_Incorrect)
{
    uint32_t byte_read = 0xdeadcafe;

    byte_read = ezHalEcho_Write(&handle1, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    status = ezHalEcho_GetDriver(ECHO_INTERFACE_1_NAME, &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_read = ezHalEcho_Read(&handle1, NULL, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_read = ezHalEcho_Read(&handle1, NULL, 0);
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_read = ezHalEcho_Read(&handle1, buff_1, 0);
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_read = ezHalEcho_Read(NULL, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_read = ezHalEcho_Read(NULL, NULL, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    byte_read = ezHalEcho_Read(NULL, NULL, sizeof(0));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle1_callback_count);

    /* interface 2 */
    byte_read = 0xdeadcafe;

    byte_read = ezHalEcho_Read(&handle2, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    status = ezHalEcho_GetDriver(ECHO_INTERFACE_2_NAME, &handle2, Handle2Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_read = ezHalEcho_Read(&handle2, NULL, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_read = ezHalEcho_Read(&handle2, NULL, 0);
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_read = ezHalEcho_Read(&handle2, buff_1, 0);
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_read = ezHalEcho_Read(NULL, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_read = ezHalEcho_Read(NULL, NULL, sizeof(buff_1));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);

    byte_read = ezHalEcho_Read(NULL, NULL, sizeof(0));
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(0, handle2_callback_count);
}

TEST(EchoHal, ezHalEcho_Read_Correct)
{
    uint32_t byte_read = 0xdeadcafe;
    uint32_t byte_written = 0xdeadcafe;

    status = ezHalEcho_GetDriver(ECHO_INTERFACE_1_NAME, &handle1, Handle1Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    status = ezHalEcho_GetDriver(ECHO_INTERFACE_1_NAME, &handle3, Handle3Callback);
    TEST_ASSERT_EQUAL(EZ_DRIVER_OK, status);

    byte_written = ezHalEcho_Write(&handle1, buff_1, sizeof(buff_1));
    TEST_ASSERT_EQUAL(sizeof(buff_1), byte_written);
    TEST_ASSERT_EQUAL(1, handle1_callback_count);

    byte_written = ezHalEcho_Write(&handle1, buff_2, sizeof(buff_2));
    TEST_ASSERT_EQUAL(sizeof(buff_2), byte_written);
    TEST_ASSERT_EQUAL(2, handle1_callback_count);

    byte_read = ezHalEcho_Read(&handle1, read_buff, sizeof(buff_1));
    TEST_ASSERT_EQUAL(sizeof(buff_1), byte_read);
    TEST_ASSERT_EQUAL(3, handle1_callback_count);
    TEST_ASSERT_EQUAL_MEMORY(buff_1, read_buff, sizeof(buff_1));

    byte_read = ezHalEcho_Read(&handle1, read_buff, sizeof(buff_2));
    TEST_ASSERT_EQUAL(sizeof(buff_2), byte_read);
    TEST_ASSERT_EQUAL(4, handle1_callback_count);
    TEST_ASSERT_EQUAL_MEMORY(buff_2, read_buff, sizeof(buff_2));

    byte_read = ezHalEcho_Read(&handle1, read_buff, 32);
    TEST_ASSERT_EQUAL(0, byte_read);
    TEST_ASSERT_EQUAL(5, handle1_callback_count);
}
/* End of file */

