
/*******************************************************************************
* Filename:         ezKernel.c
* Author:           Hai Nguyen
* Original Date:    15.09.2022
* Last Update:      15.09.2022
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
* Written by Hai Nguyen 15.09.2022
*
*******************************************************************************/

/** @file   ezKernel.c
 *  @author Hai Nguyen
 *  @date   15.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "unity.h"
#include "unity_fixture.h"
#include "ezApp/ezSdk_config.h"
#include "service/kernel/ez_kernel.h"
#include <stdint.h>
#include <stdbool.h>

#if(EZ_KERNEL == 1U)

TEST_GROUP(ezKernel);


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
static bool task1_executed = false;
static bool task2_executed = false;
static bool task3_executed = false;
static uint32_t task4_loop_till_100 = 0U;
static uint32_t task5_loop_till_100 = 0U;
static bool task7_executed = false;
static bool task8_executed = false;

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/
static KERNEL_TASK_STATUS Test_Task1(void *data, uint32_t data_size);
static KERNEL_TASK_STATUS Test_Task2(void *data, uint32_t data_size);
static KERNEL_TASK_STATUS Test_Task3(void *data, uint32_t data_size);
static KERNEL_TASK_STATUS Test_Task4(void *data, uint32_t data_size);
static KERNEL_TASK_STATUS Test_Task5(void *data, uint32_t data_size);
static KERNEL_TASK_STATUS Test_Task6(void *data, uint32_t data_size);
static KERNEL_TASK_STATUS Test_Task7(void *data, uint32_t data_size);
static KERNEL_TASK_STATUS Test_Task8(void *data, uint32_t data_size);


/******************************************************************************
* Internal functions
*******************************************************************************/
static KERNEL_TASK_STATUS Test_Task1(void *data, uint32_t data_size)
{
    task1_executed = true;
    return TASK_STATUS_OK;
}


static KERNEL_TASK_STATUS Test_Task2(void *data, uint32_t data_size)
{
    task2_executed = true;
    return TASK_STATUS_OK;
}


static KERNEL_TASK_STATUS Test_Task3(void *data, uint32_t data_size)
{
    task3_executed = true;
    return TASK_STATUS_OK;
}

TEST_SETUP(ezKernel)
{
    task1_executed = false;
    task2_executed = false;
    task3_executed = false;
    task4_loop_till_100 = 0;
    task5_loop_till_100 = 0;
    task7_executed = false;
    task8_executed = false;

    ezKernel_Initialization();
}

static KERNEL_TASK_STATUS Test_Task4(void *data, uint32_t data_size)
{
    task4_loop_till_100++;

    return TASK_STATUS_EXEC_AGAIN;
}


static KERNEL_TASK_STATUS Test_Task5(void *data, uint32_t data_size)
{
    task5_loop_till_100++;
    if (task5_loop_till_100 == 100)
    {
        return TASK_STATUS_OK;
    }
    else
    {
        return TASK_STATUS_EXEC_AGAIN;
    }
}


static KERNEL_TASK_STATUS Test_Task6(void *data, uint32_t data_size)
{
    return TASK_STATUS_ERROR;
}


static KERNEL_TASK_STATUS Test_Task7(void *data, uint32_t data_size)
{
    task7_executed = true;
    (void)ezKernel_AddTask(Test_Task8, 0, NULL, 0);
    return TASK_STATUS_OK;
}

static KERNEL_TASK_STATUS Test_Task8(void *data, uint32_t data_size)
{
    task8_executed = true;
    return TASK_STATUS_OK;
}


TEST_TEAR_DOWN(ezKernel)
{
}


TEST(ezKernel, Test_ezKernel_GetTickMillis_TickIsAdvance)
{
    for (uint32_t i = 0; i < 3000; i++)
    {
        ezKernel_UpdateTickMillis();
    }

    uint32_t current_tick = ezKernel_GetTickMillis();
    TEST_ASSERT_EQUAL(3000, current_tick);
}


TEST(ezKernel, Test_ezKernel_Run_OneShortTask)
{
    ezSTATUS status = ezSUCCESS;

    status = ezKernel_AddTask(Test_Task1, 0, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezKernel_AddTask(Test_Task2, 10, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    status = ezKernel_AddTask(Test_Task3, 20, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 20; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL(true, task1_executed);
    TEST_ASSERT_EQUAL(true, task2_executed);
    TEST_ASSERT_EQUAL(true, task3_executed);
}


TEST(ezKernel, Test_ezKernel_AddTask_MoreTaskThanSupported)
{
    ezSTATUS status = ezSUCCESS;

    for (uint32_t i = 0; i < CONFIG_NUM_OF_KERNEL_TASK; i++)
    {
        status = ezKernel_AddTask(Test_Task1, 0, NULL, 0);
        TEST_ASSERT_EQUAL(ezSUCCESS, status);
    }

    status = ezKernel_AddTask(Test_Task1, 0, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezKernel_AddTask(Test_Task1, 0, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);

    status = ezKernel_AddTask(Test_Task1, 0, NULL, 0);
    TEST_ASSERT_EQUAL(ezFAIL, status);
}


TEST(ezKernel, Test_ezKernel_Run_LoopTask)
{
    ezSTATUS status = ezSUCCESS;

    status = ezKernel_AddTask(Test_Task4, 0, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 100; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL(100, task4_loop_till_100);
}


TEST(ezKernel, Test_ezKernel_Run_RunUntilComplete)
{
    ezSTATUS status = ezSUCCESS;

    status = ezKernel_AddTask(Test_Task5, 0, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 100; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL(100, task5_loop_till_100);
    TEST_ASSERT_EQUAL(0, ezKernel_GetNumOfTasks());
}


TEST(ezKernel, Test_ezKernel_Run_RunFailTask)
{
    ezSTATUS status = ezSUCCESS;

    status = ezKernel_AddTask(Test_Task6, 0, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL(0, ezKernel_GetNumOfTasks());
}


TEST(ezKernel, Test_ezKernel_Run_AddAnotherTaskInsideTaskFunction)
{
    ezSTATUS status = ezSUCCESS;

    status = ezKernel_AddTask(Test_Task7, 0, NULL, 0);
    TEST_ASSERT_EQUAL(ezSUCCESS, status);

    for (uint32_t i = 0; i < 10; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL(true, task7_executed);
    TEST_ASSERT_EQUAL(true, task8_executed);
}


TEST(ezKernel, Test_ezKernel_GetTickMillis_TickValueCorrect)
{
    for (uint32_t i = 0; i < 100; i++)
    {
        ezKernel_UpdateTickMillis();
        ezKernel_Run();
    }

    TEST_ASSERT_EQUAL(100, ezKernel_GetTickMillis());
}


#endif /* EZ_KERNEL == 1U */

/* End of file */
