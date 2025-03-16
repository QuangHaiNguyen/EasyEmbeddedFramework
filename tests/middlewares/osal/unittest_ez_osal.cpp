/*****************************************************************************
* Filename:         unittest_ez_osal.c
* Author:           Hai Nguyen
* Original Date:    15.03.2025
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   unittest_ez_osal.c
 *  @author Hai Nguyen
 *  @date   15.03.2025
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
#include "ez_osal.h"
#include "fff.h"
#include <catch2/catch_test_macros.hpp>


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
DEFINE_FFF_GLOBALS;
FAKE_VALUE_FUNC(ezSTATUS, Init);
FAKE_VALUE_FUNC(ezOsal_TaskHandle_t, TaskCreate, const char*, uint32_t, uint32_t, ezOsal_fpTaskFunction, void*);
FAKE_VALUE_FUNC(ezSTATUS, TaskDelete, ezOsal_TaskHandle_t);
FAKE_VALUE_FUNC(ezSTATUS, TaskStart, ezOsal_TaskHandle_t);
FAKE_VALUE_FUNC(ezSTATUS, TaskStop, ezOsal_TaskHandle_t);
FAKE_VALUE_FUNC(ezSTATUS, TaskSuspend, ezOsal_TaskHandle_t);
FAKE_VALUE_FUNC(ezSTATUS, TaskResume, ezOsal_TaskHandle_t);

FAKE_VALUE_FUNC(ezOsal_SemaphoreHandle_t, SemaphoreCreate, uint32_t);
FAKE_VALUE_FUNC(ezSTATUS, SemaphoreDelete, ezOsal_SemaphoreHandle_t);
FAKE_VALUE_FUNC(ezSTATUS, SemaphoreGive, ezOsal_SemaphoreHandle_t);
FAKE_VALUE_FUNC(ezSTATUS, SemaphoreTake, ezOsal_SemaphoreHandle_t, uint32_t);

FAKE_VALUE_FUNC(ezOsal_TimerHandle_t, TimerCreate, const char*, uint32_t, ezOsal_fpTimerElapseCallback, void*);
FAKE_VALUE_FUNC(ezSTATUS, TimerDelete, ezOsal_TimerHandle_t);
FAKE_VALUE_FUNC(ezSTATUS, TimerStart, ezOsal_TimerHandle_t);
FAKE_VALUE_FUNC(ezSTATUS, TimerStop, ezOsal_TimerHandle_t);

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef struct
{
    const char  *name;
    uint32_t    stack_size;
    uint32_t    prio;
    ezOsal_fpTaskFunction func;
}MockTask;


typedef struct
{
    uint32_t count;
}MockSemaphore;

typedef struct
{
    uint32_t peirod;
}MockTimer;



class OsalTestsFixture {
private:
protected:
    ezOsal_Interfaces_t test_interface;
    MockTask mock_task;
    MockSemaphore mock_sem;
    MockTimer mock_timer;
public:
    OsalTestsFixture();
protected:
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Definitions
*******************************************************************************/


/******************************************************************************
* External functions
*******************************************************************************/

TEST_CASE_METHOD(OsalTestsFixture, "Test no interface is implemented", "[middlewares][osal]")
{
    ezOsal_TaskHandle_t task = NULL;
    ezOsal_SemaphoreHandle_t sem = NULL;

    CHECK(ezOsal_Init() == ezFAIL);

    task = ezOsal_TaskCreate("test_task", 1024, 1, NULL, NULL);
    CHECK(task == NULL);

    CHECK(ezOsal_TaskDelete(task) == ezFAIL);
    CHECK(ezOsal_TaskStart(task) == ezFAIL);
    CHECK(ezOsal_TaskStop(task) == ezFAIL);
    CHECK(ezOsal_TaskSuspend(task) == ezFAIL);
    CHECK(ezOsal_TaskResume(task) == ezFAIL);

    sem = ezOsal_SemaphoreCreate(2);
    CHECK(sem == NULL);

    CHECK(ezOsal_SemaphoreDelete(sem) == ezFAIL);
    CHECK(ezOsal_SemaphoreTake(sem, 100) == ezFAIL);
    CHECK(ezOsal_SemaphoreGive(sem) == ezFAIL);

    ezOsal_TimerHandle_t timer = NULL;
    timer = ezOsal_TimerCreate("timer1", 1000, NULL, NULL);
    CHECK(timer == NULL);
    CHECK(ezOsal_TimerDelete(timer) == ezFAIL);
    CHECK(ezOsal_TimerStart(timer) == ezFAIL);
    CHECK(ezOsal_TimerStop(timer) == ezFAIL);
}


TEST_CASE_METHOD(OsalTestsFixture, "Test task functions", "[middlewares][osal][task]")
{
    ezSTATUS status = ezFAIL;
    ezOsal_TaskHandle_t task = NULL;
    ezOsal_SemaphoreHandle_t sem = NULL;

    SECTION("ezOsal_pfInit")
    {
        Init_fake.return_val = ezFAIL;
        status = Init();
        CHECK(status == ezFAIL);
        CHECK(Init_fake.call_count == 1);

        Init_fake.return_val = ezSUCCESS;
        status = Init();
        CHECK(status == ezSUCCESS);
        CHECK(Init_fake.call_count == 2);
    }

    SECTION("ezOsal_fpTaskCreate")
    {
        TaskCreate_fake.return_val = NULL;
        task = TaskCreate("test_task", 256, 0, NULL, NULL);
        CHECK(task == NULL);
        CHECK(TaskCreate_fake.call_count == 1);

        TaskCreate_fake.return_val = &mock_task;
        task = TaskCreate("test_task", 256, 0, NULL, NULL);
        CHECK(task == &mock_task);
        CHECK(TaskCreate_fake.call_count == 2);

        TaskStart_fake.return_val = ezSUCCESS;
        status = TaskStart(&task);
        CHECK(status == ezSUCCESS);
    }

    SECTION("ezOsal_fpTaskDelete")
    {
        TaskDelete_fake.return_val = ezSUCCESS;
        status = TaskDelete(&task);
        CHECK(status == ezSUCCESS);
        CHECK(TaskDelete_fake.call_count == 1);
    }

    SECTION("ezOsal_fpTaskStart")
    {
        TaskStart_fake.return_val = ezSUCCESS;
        status = TaskStart(&task);
        CHECK(status == ezSUCCESS);
        CHECK(TaskStart_fake.call_count == 1);
    }

    SECTION("ezOsal_fpTaskStop")
    {
        TaskStop_fake.return_val = ezSUCCESS;
        status = TaskStop(&task);
        CHECK(status == ezSUCCESS);
        CHECK(TaskStop_fake.call_count == 1);
    }

    SECTION("ezOsal_fpTaskResume")
    {
        TaskResume_fake.return_val = ezSUCCESS;
        status = TaskResume(&task);
        CHECK(status == ezSUCCESS);
        CHECK(TaskResume_fake.call_count == 1);
    }

    SECTION("TaskSuspend")
    {
        TaskSuspend_fake.return_val = ezSUCCESS;
        status = TaskSuspend(&task);
        CHECK(status == ezSUCCESS);
        CHECK(TaskSuspend_fake.call_count == 1);
    }
}


TEST_CASE_METHOD(OsalTestsFixture, "Test semaphore functions", "[middlewares][osal][semaphore]")
{
    ezSTATUS status = ezFAIL;
    ezOsal_SemaphoreHandle_t sem = NULL;

    SECTION("ezOsal_fpSemaphoreCreate")
    {
        SemaphoreCreate_fake.return_val = NULL;
        sem = SemaphoreCreate(2);
        CHECK(sem == NULL);
        CHECK(SemaphoreCreate_fake.call_count == 1);

        SemaphoreCreate_fake.return_val = &mock_sem;
        sem = SemaphoreCreate(2);
        CHECK(sem == &mock_sem);
        CHECK(SemaphoreCreate_fake.call_count == 2);
    }

    SECTION("ezOsal_fpSemaphoreDelete")
    {
        SemaphoreDelete_fake.return_val = ezSUCCESS;
        status = SemaphoreDelete(&sem);
        CHECK(status == ezSUCCESS);
        CHECK(SemaphoreDelete_fake.call_count == 1);
    }

    SECTION("ezOsal_fpSemaphoreTake")
    {
        SemaphoreTake_fake.return_val = ezSUCCESS;
        status = SemaphoreTake(&sem, 100);
        CHECK(status == ezSUCCESS);
        CHECK(SemaphoreTake_fake.call_count == 1);
    }

    SECTION("ezOsal_fpSemaphoreGive")
    {
        SemaphoreGive_fake.return_val = ezSUCCESS;
        status = SemaphoreGive(&sem);
        CHECK(status == ezSUCCESS);
        CHECK(SemaphoreGive_fake.call_count == 1);
    }
}

TEST_CASE_METHOD(OsalTestsFixture, "Test timer functions", "[middlewares][osal][timer]")
{
    ezSTATUS status = ezFAIL;
    ezOsal_TimerHandle_t timer = NULL;

    SECTION("ezOsal_pfTimerCreate")
    {
        TimerCreate_fake.return_val = NULL;
        timer = TimerCreate("timer1", 1000, NULL, NULL);
        CHECK(timer == NULL);
        CHECK(TimerCreate_fake.call_count == 1);

        TimerCreate_fake.return_val = &mock_timer;
        timer = TimerCreate("timer1", 1000, NULL, NULL);
        CHECK(timer == &mock_timer);
        CHECK(TimerCreate_fake.call_count == 2);
    }

    SECTION("ezOsal_pfTimerDelete")
    {
        TimerDelete_fake.return_val = ezSUCCESS;
        status = TimerDelete(&timer);
        CHECK(status == ezSUCCESS);
        CHECK(TimerDelete_fake.call_count == 1);
    }

    SECTION("ezOsal_pfTimerStart")
    {
        TimerStart_fake.return_val = ezSUCCESS;
        status = TimerStart(&timer);
        CHECK(status == ezSUCCESS);
        CHECK(TimerStart_fake.call_count == 1);
    }

    SECTION("ezOsal_pfTimerStop")
    {
        TimerStop_fake.return_val = ezSUCCESS;
        status = TimerStop(&timer);
        CHECK(status == ezSUCCESS);
        CHECK(TimerStop_fake.call_count == 1);
    }
}

/******************************************************************************
* Internal functions
*******************************************************************************/
OsalTestsFixture::OsalTestsFixture()
{
    RESET_FAKE(Init);
    RESET_FAKE(TaskCreate);
    RESET_FAKE(TaskDelete);
    RESET_FAKE(TaskStart);
    RESET_FAKE(TaskStop);
    RESET_FAKE(TaskSuspend);
    RESET_FAKE(TaskResume);
    RESET_FAKE(SemaphoreCreate);
    RESET_FAKE(SemaphoreDelete);
    RESET_FAKE(SemaphoreGive);
    RESET_FAKE(SemaphoreTake);
    RESET_FAKE(TimerCreate);
    RESET_FAKE(TimerDelete);
    RESET_FAKE(TimerStart);
    RESET_FAKE(TimerStop);

    test_interface.Init = Init,
    test_interface.TaskCreate = TaskCreate;
    test_interface.TaskDelete = TaskDelete;
    test_interface.TaskStart = TaskStart;
    test_interface.TaskStop = TaskStop;
    test_interface.TaskSuspend = TaskSuspend;
    test_interface.TaskResume = TaskResume;

    test_interface.SemaphoreCreate = SemaphoreCreate;
    test_interface.SemaphoreDelete = SemaphoreDelete;
    test_interface.SemaphoreGive = SemaphoreGive;
    test_interface.SemaphoreTake = SemaphoreTake;
    
    test_interface.TimerCreate = TimerCreate;
    test_interface.TimerDelete = TimerDelete;
    test_interface.TimerStart = TimerStart;
    test_interface.TimerStop = TimerStop;
}


/* End of file */
