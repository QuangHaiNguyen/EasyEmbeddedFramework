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
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_TASK_WORKER_ENABLE == 1)

#include <stdint.h>
#include <stdbool.h>

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ez_task_worker"       /**< module name */
#include "ez_logging.h"
#include "ez_utilities_common.h"
#include "ez_task_worker.h"

/*the rest of include go here*/

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define A_MACRO     1   /**< a macro*/

/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None */

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
            bRet = ezLinkedList_AppendNode(&worker_list, &worker->node);
        }
    }
    else
    {
        EZWARNING("Wrong input parameters");
    }

    return bRet;
}


ezTaskBlock ezTaskWorker_GetTaskBlock(struct ezTaskWorker *worker,
                                      void **data,
                                      uint32_t data_size)
{
    ezTaskBlock element = NULL;
    if(worker != NULL && data != NULL && data_size > 0)
    {
        element = (ezTaskBlock)ezQueue_ReserveElement(&worker->msg_queue, data, data_size);
    }
    return element;
}


bool ezTaskWorker_ReleaseTaskBlock(struct ezTaskWorker *worker,
                                   ezTaskBlock task_block)
{
    bool bRet = false;
    ezSTATUS status = ezFAIL;

    if(worker != NULL && task_block != NULL)
    {
        status = ezQueue_ReleaseReservedElement(&worker->msg_queue,
                                                (ezReservedElement)task_block);
        if(status == ezSUCCESS)
        {
            bRet = true;
        }
    }

    return bRet;
}


bool ezTaskWorker_EnqueueTaskBlock(struct ezTaskWorker *worker, ezTaskBlock task_block)
{
    bool bRet = false;
    ezSTATUS status = ezFAIL;

    if(worker != NULL && task_block != NULL)
    {
        status = ezQueue_PushReservedElement(&worker->msg_queue,
                                             (ezReservedElement)task_block);
        if(status == ezSUCCESS)
        {
            bRet = true;
        }
    }

    return bRet;
}


void ezTaskWorker_Run(void)
{
    struct Node *it = NULL;
    struct ezTaskWorker_Header *header = NULL;
    struct ezTaskWorker *worker = NULL;
    ezSTATUS status = ezFAIL;
    uint32_t data_size = 0;

    EZ_LINKEDLIST_FOR_EACH(it, &worker_list)
    {
        worker = EZ_LINKEDLIST_GET_PARENT_OF(it, node, struct ezTaskWorker);
        if(ezQueue_GetNumOfElement(&worker->msg_queue) > 0)
        {
            status = ezQueue_GetFront(&worker->msg_queue, (void**)&header, &data_size);
            if(status == ezSUCCESS && header->task != NULL)
            {
                header->task(header);
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
