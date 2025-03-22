/*****************************************************************************
* Filename:         ez_app_osal.c
* Author:           Hai Nguyen
* Original Date:    16.03.2025
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_app_osal.c
 *  @author Hai Nguyen
 *  @date   16.03.2025
 *  @brief  Application to test OSAL component
 *
 *  @details None
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "ez_app_osal.h"


#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ez_app_osal"       /**< module name */
#include "ez_logging.h"
#include "ez_osal.h"
#include "ez_osal_threadx.h"

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define STACK_SIZE    256

/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
static ezOsal_TaskConfig_t task1;
static ezOsal_TaskConfig_t task2;
static ezOsal_TaskHandle_t task1_handle;
static ezOsal_TaskHandle_t task2_handle;
static ezOsal_TaskResource_t task1_resource;
static ezOsal_TaskResource_t task2_resource;


static const ezOsal_Interfaces_t *rtos_interface = NULL;

static ezOsal_SemaphoreHandle_t semaphore_handle;
static ezOsal_SemaphoreConfig_t semaphore;
static ezOsal_SemaphoreResource_t semaphore_resource;

static ezOsal_TimerHandle_t timer_handle;
static ezOsal_TimerResource_t timer_resource;

static ezOsal_TimerConfig_t timer;

/*****************************************************************************
* Function Definitions
*****************************************************************************/
static void Task1Function(void* argument);
static void Task2Function(void* argument);
static void TimerElapseCallback(void *argument);

/*****************************************************************************
* Public functions
*****************************************************************************/
void ezApp_OsalInit(void)
{
    rtos_interface = ezOsal_ThreadXGetInterface();
    (void) ezOsal_SetInterface(rtos_interface);
    ezOsal_TaskStartScheduler();
}

void tx_application_define(void *first_unused_memory)
{
    task1.task_name = "task1";
    task1.stack_size = STACK_SIZE;
    task1.priority = 1;
    task1.task_function = Task1Function;
    task1.argument = NULL;
    task1.static_resource = &task1_resource;


    task2.task_name = "task2";
    task2.stack_size = STACK_SIZE;
    task2.priority = 2;
    task2.task_function = Task2Function;
    task2.argument = NULL;
    task2.static_resource = &task2_resource;

    timer.argument = NULL;
    timer.period_ticks = 500;
    timer.timer_callback = TimerElapseCallback;
    timer.timer_name = "timer1";
    timer.static_resource = &timer_resource;

    semaphore.max_count = 1;
    semaphore.static_resource = &semaphore_resource;

    ezOsal_Init(first_unused_memory);

    semaphore_handle = ezOsal_SemaphoreCreate(&semaphore);
    
    timer_handle = ezOsal_TimerCreate(&timer);

    ezOsal_TimerStart(timer_handle);
    task1_handle = ezOsal_TaskCreate(&task1);
    task2_handle = ezOsal_TaskCreate(&task2);
}

/*****************************************************************************
* Local functions
*****************************************************************************/
static void Task1Function(void* argument)
{
    uint8_t count = 0;
    ezSTATUS status = ezFAIL;
    while(1)
    {
        status = ezOsal_SemaphoreTake(semaphore_handle, 50);
        if(status == ezSUCCESS)
        {
            EZINFO("Task 1 is running, count = %d", count++);
            ezOsal_SemaphoreGive(semaphore_handle);
        }
        ezOsal_TaskDelay(100);
    }
}

static void Task2Function(void* argument)
{
    uint8_t count = 0;
    ezSTATUS status = ezFAIL;

    while(1)
    {
        status = ezOsal_SemaphoreTake(semaphore_handle, 100);
        if(status == ezSUCCESS)
        {
            EZINFO("Task 2 is running, count = %d", count++);
            ezOsal_TaskDelay(100); /* Simulate some heavy load */
        }
        ezOsal_SemaphoreGive(semaphore_handle);
        ezOsal_TaskDelay(300);
    }
}

static void TimerElapseCallback(void *argument)
{
    EZINFO("Timer elapsed");
}
/* End of file*/
