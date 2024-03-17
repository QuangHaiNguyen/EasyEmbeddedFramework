/*****************************************************************************
* Filename:         unittest_ez_uart.c
* Author:           Hai Nguyen
* Original Date:    15.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   unittest_ez_uart.c
 *  @author Hai Nguyen
 *  @date   15.03.2024
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "unity.h"
#include "unity_fixture.h"
#include "ez_uart.h"

TEST_GROUP(ez_uart);

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define DRV_NAME        "MOCK DRIVER"

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct ezUartDriver mock_hw;
static ezUartDrvInstance_t first_inst;
static ezUartDrvInstance_t second_inst;

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void RunAllTests(void);
static EZ_DRV_STATUS ezMockUart_Initialize(uint8_t index);
static EZ_DRV_STATUS ezMockUart_Deinitialize(uint8_t index);
static EZ_DRV_STATUS ezMockUart_AsyncTransmit(uint8_t index, uint8_t *tx_buff, uint16_t buff_size);
static EZ_DRV_STATUS ezMockUart_AsyncReceive(uint8_t index, uint8_t *rx_buff, uint16_t buff_size);
static EZ_DRV_STATUS ezMockUart_SyncTransmit(uint8_t index, uint8_t *tx_buff, uint16_t buff_size, uint32_t timeout_millis);
static EZ_DRV_STATUS ezMockUart_SyncReceive(uint8_t index, uint8_t *rx_buff, uint16_t buff_size, uint32_t timeout_millis);
static EZ_DRV_STATUS ezMockUart_GetConfig(uint8_t index, struct ezUartConfiguration *config);
static EZ_DRV_STATUS ezMockUart_UpdateConfig(uint8_t index);
static void ezMockUart_SetImplementedInterface(struct ezHwUartInterface *interface);


/******************************************************************************
* External functions
*******************************************************************************/
int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}


TEST_SETUP(ez_uart)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;

    mock_hw.common.version[0] = 1;
    mock_hw.common.version[1] = 2;
    mock_hw.common.version[2] = 3;
    mock_hw.common.name = DRV_NAME;

    status = ezUart_SystemRegisterHwDriver(&mock_hw);
    TEST_ASSERT_EQUAL(STATUS_OK, status);
}


TEST_TEAR_DOWN(ez_uart)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;
    status = ezUart_SystemUnregisterHwDriver(&mock_hw);
    TEST_ASSERT_EQUAL(STATUS_OK, status);
}


TEST_GROUP_RUNNER(ez_uart)
{
    RUN_TEST_CASE(ez_uart, ezUart_SystemRegisterHwDriver);
    RUN_TEST_CASE(ez_uart, ezUart_SystemUnregisterHwDriver);
    RUN_TEST_CASE(ez_uart, ezUart_RegisterInstance);
    RUN_TEST_CASE(ez_uart, ezUart_UnregisterInstance);
    RUN_TEST_CASE(ez_uart, ezUart_Initialize);
}


TEST(ez_uart, ezUart_SystemRegisterHwDriver)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;

    TEST_MESSAGE("Test NULL argument");
    status = ezUart_SystemRegisterHwDriver(NULL);
    TEST_ASSERT_EQUAL(STATUS_ERR_ARG, status);
}


TEST(ez_uart, ezUart_SystemUnregisterHwDriver)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;

    TEST_MESSAGE("Test NULL argument");
    status = ezUart_SystemUnregisterHwDriver(NULL);
    TEST_ASSERT_EQUAL(STATUS_ERR_ARG, status);
}


TEST(ez_uart, ezUart_RegisterInstance)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;

    TEST_MESSAGE("Test null pointer");
    status = ezUart_RegisterInstance(NULL, NULL);
    TEST_ASSERT_EQUAL(STATUS_ERR_ARG, status);

    TEST_MESSAGE("Test driver does not exist");
    status = ezUart_RegisterInstance(&first_inst, "Random driver name");
    TEST_ASSERT_EQUAL(STATUS_ERR_DRV_NOT_FOUND, status);

    TEST_MESSAGE("Test happy path");
    status = ezUart_RegisterInstance(&first_inst, DRV_NAME);
    TEST_ASSERT_EQUAL(STATUS_OK, status);
    TEST_ASSERT_NOT_EQUAL(NULL, first_inst.driver);
}


TEST(ez_uart, ezUart_UnregisterInstance)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;

    TEST_MESSAGE("Test null pointer");
    status = ezUart_UnregisterInstance(NULL);
    TEST_ASSERT_EQUAL(STATUS_ERR_ARG, status);

    TEST_MESSAGE("Test happy path");
    status = ezUart_UnregisterInstance(&first_inst);
    TEST_ASSERT_EQUAL(STATUS_OK, status);
    TEST_ASSERT_EQUAL(NULL, first_inst.driver);
}


TEST(ez_uart, ezUart_Initialize)
{
    EZ_DRV_STATUS status = STATUS_ERR_GENERIC;

    TEST_MESSAGE("Test null pointer");
    status = ezUart_Initialize(NULL);
    TEST_ASSERT_EQUAL(STATUS_ERR_ARG, status);

    TEST_MESSAGE("Test unregistered instance");
    status = ezUart_Initialize(&second_inst);
    TEST_ASSERT_EQUAL(STATUS_ERR_DRV_NOT_FOUND, status);

    /* Now we register the instance */
    status = ezUart_RegisterInstance(&first_inst, DRV_NAME);
    status = ezUart_RegisterInstance(&second_inst, DRV_NAME);

    TEST_MESSAGE("Test unimplmented interface ");
    status = ezUart_Initialize(&first_inst);
    TEST_ASSERT_EQUAL(STATUS_ERR_INF_NOT_EXIST, status);

    TEST_MESSAGE("Test interface is locked");
    /* manually set lock */
    mock_hw.common.curr_inst = &first_inst;
    status = ezUart_Initialize(&second_inst);
    TEST_ASSERT_EQUAL(STATUS_BUSY, status);
    /* release the lock */
    mock_hw.common.curr_inst = NULL;

    TEST_MESSAGE("Test happy path");
    ezMockUart_SetImplementedInterface(&mock_hw.interface);
    status = ezUart_Initialize(&first_inst);
    TEST_ASSERT_EQUAL(STATUS_OK, status);
}


/******************************************************************************
* Internal functions
*******************************************************************************/
static void RunAllTests(void)
{
    RUN_TEST_GROUP(ez_uart);
}

static EZ_DRV_STATUS ezMockUart_Initialize(uint8_t index)
{
    return STATUS_OK;
}


static EZ_DRV_STATUS ezMockUart_Deinitialize(uint8_t index)
{
    return STATUS_OK;
}


static EZ_DRV_STATUS ezMockUart_AsyncTransmit(uint8_t index, uint8_t *tx_buff, uint16_t buff_size)
{
    return STATUS_OK;
}


static EZ_DRV_STATUS ezMockUart_AsyncReceive(uint8_t index, uint8_t *rx_buff, uint16_t buff_size)
{
    return STATUS_OK;
}


static EZ_DRV_STATUS ezMockUart_SyncTransmit(uint8_t index, uint8_t *tx_buff, uint16_t buff_size, uint32_t timeout_millis)
{
    return STATUS_OK;
}


static EZ_DRV_STATUS ezMockUart_SyncReceive(uint8_t index, uint8_t *rx_buff, uint16_t buff_size, uint32_t timeout_millis)
{
    return STATUS_OK;
}


static EZ_DRV_STATUS ezMockUart_GetConfig(uint8_t index, struct ezUartConfiguration *config)
{
    return STATUS_OK;
}


static EZ_DRV_STATUS ezMockUart_UpdateConfig(uint8_t index)
{
    return STATUS_OK;
}


static void ezMockUart_SetImplementedInterface(struct ezHwUartInterface *interface)
{
    if(interface != NULL)
    {
        interface->index = 0;
        interface->async_receive = ezMockUart_AsyncReceive;
        interface->async_transmit = ezMockUart_AsyncTransmit;
        interface->deinitialize = ezMockUart_Deinitialize;
        interface->initialize = ezMockUart_Initialize;
        interface->sync_receive = ezMockUart_SyncReceive;
        interface->sync_transmit = ezMockUart_SyncTransmit;
        interface->get_conf = ezMockUart_GetConfig;
        interface->update_conf = ezMockUart_UpdateConfig;
    }
}


/* End of file */
