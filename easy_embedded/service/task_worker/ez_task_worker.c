/*****************************************************************************
* Filename:         ez_task_worker.c
* Author:           Hai Nguyen
* Original Date:    29.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_task_worker.c
 *  @author Hai Nguyen
 *  @date   29.03.2024
 *  @brief  Implementation of the task worker component
 *
 *  @details
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_TASK_WORKER_ENABLE == 1)

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ez_task_worker"       /**< module name */
#include "ez_logging.h"
#include "ez_utilities_common.h"
#include "ez_task_worker.h"


/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None*/


/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/** @brief definition of a task block
 */
typedef ezReservedElement ezTaskBlock_t;


/** @brief definition of common data that every task block must have.
 */
struct ezTaskBlockCommon
{
    ezTaskWorkerTaskFunc task;          /* Task that will be executed by the worker */
    ezTaskWorkerCallbackFunc callback;  /* Callback function to notify the result of the task execution */
};


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
static struct Node worker_list = EZ_LINKEDLIST_INIT_NODE(worker_list);
static struct ezTaskWorkerThreadInterfaces *rtos_interfaces = NULL;

/*****************************************************************************
* Function Definitions
*****************************************************************************/

#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
/*****************************************************************************
* Function: ezTaskWorker_CheckRtosInterfaceSanity
*//** 
* @brief This function check if all of the function pointers are set accordngly
*
* @details
*
* @param[in]    rtos_interfaces: pointer to the rtos interface
* @return       true if function pointers are set, else false
*
* @pre None
* @post None
*
* \b Example
* @code
* @endcode
*
* @see
*
*****************************************************************************/
static bool ezTaskWorker_CheckRtosInterfaceSanity(struct ezTaskWorkerThreadInterfaces *rtos_interfaces);
#endif /* (EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1) */

/*****************************************************************************
* Public functions
*****************************************************************************/
bool ezTaskWorker_CreateWorker(struct ezTaskWorker *worker,
                               uint8_t *queue_buffer,
                               uint32_t queue_buffer_size,
                               void *thread_func)
{
    bool bRet = false;
    ezSTATUS status = ezFAIL;
    EZTRACE("ezTaskWorker_CreateWorker()");

    if(worker != NULL)
    {
        status = ezQueue_CreateQueue(&worker->msg_queue, queue_buffer, queue_buffer_size);
        if(status == ezSUCCESS)
        {
#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
            if(rtos_interfaces != NULL)
            {
                bRet = rtos_interfaces->create_event(worker);
                bRet &= rtos_interfaces->create_semaphore(worker);
                bRet &= rtos_interfaces->create_thread(worker, thread_func);
            }
#else
            ezLinkedList_InitNode(&worker->node);
            bRet = EZ_LINKEDLIST_ADD_TAIL(&worker_list, &worker->node);
#endif /* (EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1) */
        }
    }
    else
    {
        EZWARNING("Wrong input parameters");
    }

    return bRet;
}

#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
bool ezTaskWorker_SetRtosInterface(struct ezTaskWorkerThreadInterfaces *interfaces)
{
    bool ret = false;
    EZTRACE("ezTaskWorker_SetRtosInterface()");
    if(interfaces != NULL)
    {
        rtos_interfaces = interfaces;
        ret = ezTaskWorker_CheckRtosInterfaceSanity(interfaces);
    }
    return ret;
}
#endif /* (EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1) */

bool ezTaskWorker_EnqueueTask(struct ezTaskWorker *worker,
                              ezTaskWorkerTaskFunc task,
                              ezTaskWorkerCallbackFunc callback,
                              void *context,
                              uint32_t context_size,
                              uint32_t ticks_to_wait)
{
    bool ret = false;
    void *buff = NULL;
    ezTaskBlock_t task_block = NULL;
    ezSTATUS status = ezFAIL;

    EZTRACE("ezTaskWorker_EnqueueTask()");

    if((worker != NULL) && (task != NULL) && (callback != NULL))
    {
#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
        EZTRACE("Getting semaphore from worker = %s", worker->worker_name);
        ret = rtos_interfaces->take_semaphore(worker, ticks_to_wait);
#else
        ret = true;
#endif /* (EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1) */
        if(ret == true)
        {
            EZTRACE("Get semaphore from worker = %s OK", worker->worker_name);
            /**The idea to store common data and context data is we reserve a buffer
             * with the size = common size + context size. Then, the buffer is convert to
             * ezTaskBlockCommon to store common data. After that it is offseted to the
             * address = common address + sizeof(ezTaskBlockCommon). Then the context
             * is copied to that address.
             */
            task_block = (ezTaskBlock_t)ezQueue_ReserveElement(&worker->msg_queue,
                                                               &buff,
                                                               sizeof(struct ezTaskBlockCommon) + context_size);
            if((task_block != NULL) && (buff != NULL))
            {
                /* Set common data */
                ((struct ezTaskBlockCommon*)buff)->callback = callback;
                ((struct ezTaskBlockCommon*)buff)->task = task;

                /* Offset the pointer */
                buff += sizeof(struct ezTaskBlockCommon);

                /* Copy context data */
                memcpy(buff, context, context_size);
                status = ezQueue_PushReservedElement(&worker->msg_queue,
                                                    (ezReservedElement)task_block);
                if(status == ezSUCCESS)
                {
                    ret = true;
#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
                    rtos_interfaces->set_events(worker, EZ_EVENT_TASK_AVAIL);
#endif /* (EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1) */
                    EZINFO("Add new task to %s",worker->worker_name);
                }
                else
                {
                    ret = false;
                    ezQueue_ReleaseReservedElement(&worker->msg_queue,
                                                (ezReservedElement)task_block);
                    EZERROR("Cannot add task to %s",worker->worker_name);
                }
            }
#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
            ret &= rtos_interfaces->give_semaphore(worker);
#endif /* (EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1) */
        }
    }
    
    if(ret == false)
    {
        EZERROR("Enqueue task error");
    }

    return ret;
}

#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
void ezTaskWorker_ExecuteTask(struct ezTaskWorker *worker, uint32_t ticks_to_wait)
{
    bool ret = false;
    ezSTATUS status = ezFAIL;
    void *context = NULL;
    struct ezTaskBlockCommon *common = NULL;
    uint32_t data_size = 0;

    if(worker != NULL)
    {
        EZTRACE("ezTaskWorker_ExecuteTask(woker = %s)", worker->worker_name);
        ret = rtos_interfaces->get_events(worker, EZ_EVENT_TASK_AVAIL, ticks_to_wait);

        if(ret == false)
        {
            EZERROR("Get event failed");
        }
        else
        {
            EZINFO("Receive EZ_EVENT_TASK_AVAIL");
            ret = rtos_interfaces->take_semaphore(worker, ticks_to_wait);
            EZTRACE("Getting semaphore from worker = %s", worker->worker_name);
        }

        if(ret == false)
        {
            EZERROR("Take semaphore failed");
        }
        else
        {
            EZTRACE("Get semaphore from worker = %s OK", worker->worker_name);
            status = ezQueue_GetFront(&worker->msg_queue, (void**)&common, &data_size);

            if((status == ezSUCCESS) && (common->task != NULL))
            {
                context = common;
                context += sizeof(struct ezTaskBlockCommon);
                common->task(context, common->callback);
            }
            else
            {
                ret = false;
                EZERROR("Get data from queue failed");
            }

            status = ezQueue_PopFront(&worker->msg_queue);
            rtos_interfaces->give_semaphore(worker);
        }
    }
}
#else
void ezTaskWorker_ExecuteTaskNoRTOS(void)
{
    struct Node *it = NULL;
    struct ezTaskBlockCommon *common = NULL;
    void *context = NULL;
    struct ezTaskWorker *worker = NULL;
    ezSTATUS status = ezFAIL;
    uint32_t data_size = 0;

    EZTRACE("ezTaskWorker_Run()");

    EZ_LINKEDLIST_FOR_EACH(it, &worker_list)
    {
        worker = EZ_LINKEDLIST_GET_PARENT_OF(it, node, struct ezTaskWorker);
        if(ezQueue_GetNumOfElement(&worker->msg_queue) > 0)
        {
            status = ezQueue_GetFront(&worker->msg_queue, (void**)&common, &data_size);
            if(status == ezSUCCESS && common->task != NULL)
            {
                context = common;
                context += sizeof(struct ezTaskBlockCommon);
                common->task(context, common->callback);
            }
            status = ezQueue_PopFront(&worker->msg_queue);
        }
    }
}
#endif /* (EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1) */

/*****************************************************************************
* Local functions
*****************************************************************************/

#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
static bool ezTaskWorker_CheckRtosInterfaceSanity(struct ezTaskWorkerThreadInterfaces *rtos_interfaces)
{
    bool ret = false;
    EZTRACE("ezTaskWorker_CheckRtosInterfaceSanity()");

    if(rtos_interfaces != NULL)
    {
        ret = (rtos_interfaces->create_thread != NULL);
        ret &= (rtos_interfaces->create_semaphore != NULL);
        ret &= (rtos_interfaces->give_semaphore != NULL);
        ret &= (rtos_interfaces->take_semaphore != NULL);
        ret &= (rtos_interfaces->create_event != NULL);
        ret &= (rtos_interfaces->set_events != NULL);
        ret &= (rtos_interfaces->get_events != NULL);
    }

    return ret;
}
#endif /* (EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1) */

#endif /* EZ_TASK_WORKER_ENABLE == 1 */
/* End of file*/
