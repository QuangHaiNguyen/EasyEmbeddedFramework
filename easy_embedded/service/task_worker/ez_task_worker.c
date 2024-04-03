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

/*the rest of include go here*/

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


/*****************************************************************************
* Function Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Public functions
*****************************************************************************/
bool ezTaskWorker_InitializeWorker(struct ezTaskWorker *worker,
                                   uint8_t *queue_buffer,
                                   uint32_t queue_buffer_size)
{
    bool bRet = false;
    ezSTATUS status = ezFAIL;
    EZTRACE("ezTaskWorker_InitializeWorker()");

    if((worker != NULL)
        && (queue_buffer != NULL)
        && (queue_buffer_size > 0))
    {
        status = ezQueue_CreateQueue(&worker->msg_queue, queue_buffer, queue_buffer_size);
        if(status == ezSUCCESS)
        {
            ezLinkedList_InitNode(&worker->node);
            bRet = EZ_LINKEDLIST_ADD_TAIL(&worker_list, &worker->node);
        }
    }
    else
    {
        EZWARNING("Wrong input parameters");
    }

    return bRet;
}


bool ezTaskWorker_EnqueueTask(struct ezTaskWorker *worker,
                              ezTaskWorkerTaskFunc task,
                              ezTaskWorkerCallbackFunc callback,
                              void *context,
                              uint32_t context_size)
{
    bool bRet = false;
    void *buff = NULL;
    ezTaskBlock_t task_block = NULL;
    ezSTATUS status = ezFAIL;

    EZTRACE("ezTaskWorker_EnqueueTask()");

    if((worker != NULL) && (task != NULL) && (callback != NULL))
    {
        /**The idea to store common data and context data is we reserve a buffer
         * with the size = common size + context size. Then, the buffer is convert to
         * ezTaskBlockCommon to store common data. After that it is offseted to the
         * address = common address + sizeof(ezTaskBlockCommon). Then the context
         * is copied to that address.
         */
        task_block = (ezTaskBlock_t)ezQueue_ReserveElement(&worker->msg_queue,
                                                           &buff,
                                                           sizeof(struct ezTaskBlockCommon) + context_size);
        if(task_block != NULL && buff != NULL)
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
                bRet = true;
                EZINFO("Add new task to queue");
            }
            else
            {
                ezQueue_ReleaseReservedElement(&worker->msg_queue,
                                               (ezReservedElement)task_block);
                EZERROR("Cannot push task to queue");
            }
        }
        else
        {
            EZERROR("Do not have enough memory in the queue");
        }
    }

    return bRet;
}

void ezTaskWorker_Run(void)
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

/*****************************************************************************
* Local functions
*****************************************************************************/

#endif /* EZ_TASK_WORKER_ENABLE == 1 */
/* End of file*/
