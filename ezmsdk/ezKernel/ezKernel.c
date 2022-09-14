
/*******************************************************************************
* Filename:         ezKernel.c
* Author:           Hai Nguyen
* Original Date:    13.09.2022
* Last Update:      13.09.2022
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
* Written by Hai Nguyen 13.09.2022
*
*******************************************************************************/

/** @file   ezKernel.c
 *  @author Hai Nguyen
 *  @date   13.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezKernel.h"

#if(CONFIG_KERNEL == 1U)

#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "ezKernel"      /**< module name */
#include "utilities/logging/logging.h"

#include "utilities/linked_list/linked_list.h"
#include "utilities/stcmem/stcmem.h"
#include <string.h>

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

#ifndef KERNEL_BUFF_SIZE
#define KERNEL_BUFF_SIZE        512     /**< Size for the kernel buffer */
#endif /*KERNEL_BUFF_SIZE*/

#ifndef NUM_OF_KERNEL_TASK
#define NUM_OF_KERNEL_TASK      10      /**< Size for the kernel buffer */
#endif /*KERNEL_BUFF_SIZE*/

#define DEFAULT_SLEEP_MILLIS    1000

/******************************************************************************
* Module Typedefs
*******************************************************************************/


/** @brief Definition of a kernel object
 *
 */
struct ezKernel
{
    uint32_t current_tick;      /**< */
    bool is_busy;               /**< Busy flag, act as binary mutex */
    ezmMemList memory_list;     /**< */
    struct Node executed_tasks; /**< List of tasks must be executed */
};


/** @brief Definition of a kernel task
 *
 */
struct ezKernelTask
{
    bool is_busy;                   /**< */
    struct Node node;               /**< */
    int32_t delay_millis;          /**< execution cycle*/
    int32_t  exec_cnt_down;         /**< count down until execution */
    void *task_data;                /**< */
    uint32_t task_data_size;        /**< */
    ezKernelTaskFunction function;  /**< */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct ezKernel kernel = { 0 };
static uint8_t kernel_buff[KERNEL_BUFF_SIZE] = { 0 };
static struct ezKernelTask task_pool[NUM_OF_KERNEL_TASK];


/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezKernel_ResetKernelTask(struct ezKernelTask *task);
static struct ezKernelTask *ezKernel_GetFreeTask(void);


/******************************************************************************
* External functions
*******************************************************************************/


void ezKernel_Initialization(void)
{
    TRACE("ezKernel_Initialization()");

    kernel.is_busy = false;
    kernel.current_tick = 0;

    ezmLL_InitNode(&kernel.executed_tasks);

    (void) ezmStcMem_InitMemList(&kernel.memory_list, kernel_buff, KERNEL_BUFF_SIZE);

    for (uint32_t i = 0; i < NUM_OF_KERNEL_TASK; i++)
    {
        ezKernel_ResetKernelTask(&task_pool[i]);
    }
}


void ezKernel_UpdateTickMillis(void)
{
    struct Node *iterator = NULL;
    struct ezKernelTask *exec_task = NULL;

    /* Increase kernel tick */
    kernel.current_tick++;

    EZMLL_FOR_EACH(iterator, &kernel.executed_tasks)
    {
        exec_task = EZMLL_GET_PARENT_OF(iterator, node, struct ezKernelTask);

        if (exec_task->exec_cnt_down > INT32_MIN)
        {
            exec_task->exec_cnt_down--;
        }
    }
}


ezSTATUS ezKernel_AddTask(ezKernelTaskFunction function,
    int32_t delay_millis,
    void *task_data,
    uint32_t task_data_size)
{
    ezSTATUS status = ezFAIL;
    struct ezKernelTask *task = NULL;

    if (kernel.is_busy == false)
    {
        kernel.is_busy = true;
        status = ezSUCCESS;

        DEBUG("kernel is not busy, start adding task...");
    }

    if (status == ezSUCCESS && function != NULL)
    {
        task = ezKernel_GetFreeTask();

        if (task)
        {
            task->delay_millis = delay_millis;
            task->exec_cnt_down = delay_millis;
            task->function = function;
 
            if (task->task_data != NULL && task->task_data_size > 0)
            {
                task->task_data_size = task_data_size;
                task->task_data = ezmStcMem_Malloc(&kernel.memory_list,
                    task_data_size);

                if (task->task_data != NULL)
                {
                    memcpy(task->task_data, task_data, task->task_data_size);
                    status = ezSUCCESS;
                }
            }
            else
            {
                status = ezSUCCESS;
            }

            EZMLL_ADD_TAIL(&kernel.executed_tasks, &task->node);
        }
    }

    kernel.is_busy = false;

    if (status == ezSUCCESS)
    {
        DEBUG("Add task success");
    }
    else
    {
        DEBUG("Add task fail");
    }

    return status;
}


void ezKernel_Run(void)
{
    struct Node *iterator = NULL;
    int32_t next_wakeup = 0;
    struct ezKernelTask *task = NULL;
    KERNEL_TASK_STATUS task_status = TASK_STATUS_OK;

    EZMLL_FOR_EACH(iterator, &kernel.executed_tasks)
    {
        task = EZMLL_GET_PARENT_OF(iterator, node, struct ezKernelTask);

        if (task->exec_cnt_down <= 0)
        {
            /* execute the task when it is timeout */
            if (task->function)
            {
                task_status = task->function(task->task_data,
                    task->task_data_size);
            }
            else
            {
                task_status = TASK_STATUS_ERROR;
            }

            /* handle the return task status */
            if (task_status == TASK_STATUS_OK || task_status == TASK_STATUS_ERROR)
            {
                EZMLL_UNLINK_NODE(iterator);
                ezmStcMem_Free(&kernel.memory_list, task->task_data);
                ezKernel_ResetKernelTask(task);
            }
            else if (task_status == TASK_STATUS_EXEC_AGAIN)
            {
                /* refresh count down value*/
                task->exec_cnt_down = task->delay_millis;
            }
            else
            {
                /* do nothing */
            }
        }
        else
        {
            /* get the next wake up time if applied */
            if (next_wakeup == 0)
            {
                next_wakeup = task->exec_cnt_down;
            }
            else if (task->exec_cnt_down < next_wakeup)
            {
                next_wakeup = task->exec_cnt_down;
            }
            else
            {
                /* do nothing */
            }
        }
    }
}


/******************************************************************************
* Internal functions
*******************************************************************************/


/******************************************************************************
* Function : ezKernel_ResetKernelTask
*//**
* @Description:
*
* @param    a:
* @return   None
*
*******************************************************************************/
static void ezKernel_ResetKernelTask(struct ezKernelTask *task)
{
    TRACE("ezKernel_ResetKernelTask()");

    if (task)
    {
        task->exec_cnt_down = 0;
        task->function = NULL;
        task->is_busy = false;
        task->delay_millis = 0;
        task->task_data = NULL;
        task->task_data_size = 0;
        ezmLL_InitNode(&task->node);

        DEBUG("add task success");
    }
    else
    {
        DEBUG("add task fail");
    }
}


static struct ezKernelTask *ezKernel_GetFreeTask(void)
{
    struct ezKernelTask *free_task = NULL;
    
    for (uint32_t i = 0; i < NUM_OF_KERNEL_TASK; i++)
    {
        if (task_pool[i].is_busy == false)
        {
            free_task = &task_pool[i];
            task_pool[i].is_busy = true;
            break;
        }
    }

    return free_task;
}


#endif /* CONFIG_KERNEL == 1U */
/* End of file*/

