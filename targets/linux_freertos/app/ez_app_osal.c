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
#include "ez_osal_freertos.h"

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
static const ezOsal_Interfaces_t *rtos_interface = NULL;


/*****************************************************************************
* Function Definitions
*****************************************************************************/
static void Task1Function(void *argument);
static void Task2Function(void *argument);
static void TimerElapseCallback(void *argument);

#if (EZ_OSAL_USE_STATIC == 1)
static ezOsal_Stack_t stack1[STACK_SIZE];
static ezOsal_Stack_t stack2[STACK_SIZE];
static ezOsal_TaskResource_t task1_resource;
static ezOsal_TaskResource_t task2_resource;
static ezOsal_SemaphoreResource_t semaphore_resource;

EZ_OSAL_DEFINE_TASK_HANDLE(task1, STACK_SIZE, 1, Task1Function, NULL, &task1_resource);
EZ_OSAL_DEFINE_TASK_HANDLE(task2, STACK_SIZE, 1, Task2Function, NULL, &task2_resource);
EZ_OSAL_DEFINE_SEMAPHORE_HANDLE(semaphore_handle, 1, &semaphore_resource);
EZ_OSAL_DEFINE_TIMER_HANDLE(timer, 100, TimerElapseCallback, NULL, NULL);
#else
EZ_OSAL_DEFINE_TASK_HANDLE(task1, STACK_SIZE, 1, Task1Function, NULL, NULL);
EZ_OSAL_DEFINE_TASK_HANDLE(task2, STACK_SIZE, 1, Task2Function, NULL, NULL);
EZ_OSAL_DEFINE_SEMAPHORE_HANDLE(semaphore_handle, 1, NULL);
EZ_OSAL_DEFINE_TIMER_HANDLE(timer, 100, TimerElapseCallback, NULL, NULL);
#endif

/*****************************************************************************
* Public functions
*****************************************************************************/
void ezApp_OsalInit(void)
{
#if (EZ_OSAL_USE_STATIC == 1)
    task1_resource.stack = stack1;
    task2_resource.stack = stack2;
#endif

    rtos_interface = ezOsal_FreeRTOSGetInterface();
    (void) ezOsal_SetInterface(rtos_interface);

    (void) ezOsal_SemaphoreCreate(&semaphore_handle);
    (void) ezOsal_TimerCreate(&timer);

    ezOsal_TimerStart(&timer);
    (void) ezOsal_TaskCreate(&task1);
    (void) ezOsal_TaskCreate(&task2);

    ezOsal_TaskSuspend(&task2);
    ezOsal_TaskStartScheduler();
}


/*****************************************************************************
* Local functions
*****************************************************************************/
static void Task1Function(void *argument)
{
    uint8_t count = 0;
    ezSTATUS status = ezFAIL;

    EZINFO("Enter Task 1");
    while(1)
    {
        count++;
        EZINFO("Task 1 - count = %d", count);
        if(count == 5)
        {
            status = ezOsal_TaskResume(&task2);
            if(status == ezSUCCESS)
            {
                EZINFO("Resuming task 2");
            }
        }
        ezOsal_TaskDelay(10);
    }
}

static void Task2Function(void *argument)
{
    uint8_t count = 0;
    ezSTATUS status = ezFAIL;

    EZINFO("Enter Task 2");
    while(1)
    {
        count++;
        EZINFO("Task 2 - count = %d", count);
        if(count == 5)
        {
            status = ezOsal_TaskDelete(&task1);
            if(status == ezSUCCESS)
            {
                EZINFO("delete task 1");
            }
        }
        ezOsal_TaskDelay(20);
    }
}

static void TimerElapseCallback(void *argument)
{
    static int count = 0;
    ezSTATUS status = ezFAIL;

    EZINFO("Timer elapsed, count = %d", count);
    if(count == 5)
    {
        status = ezOsal_TaskDelete(&task2);
        if(status == ezSUCCESS)
        {
            EZINFO("delete task 2");
        }
    }
    count++;
}
/* End of file*/
