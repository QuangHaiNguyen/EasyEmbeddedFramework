/*****************************************************************************
* Filename:         ez_task_worker.h
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

/** @file   ez_task_worker.h
 *  @author Hai Nguyen
 *  @date   29.03.2024
 *  @brief  Public API of task worker component
 *
 *  @details
 */

#ifndef _EZ_TASK_WORKER_H
#define _EZ_TASK_WORKER_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Includes
*****************************************************************************/
#include "ez_linked_list.h"
#include "ez_queue.h"

#if (EZ_TASK_WORKER_ENABLE == 1)

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/** @brief Definition of callback function to notify a task is finished or
 *         error occurs
 *  @param[out] event: retrun event, depending on the application
 *  @param[out] ret_data: return data, depending on the task
 *  @return: true if a task is executed successfully, otherwise false
 */
typedef void (*ezTaskWorkerCallbackFunc)(uint8_t event, void *ret_data);


/** @brief Definition of task function.
 *  @param[in] context: context containing data of a task
 *  @param[in] callback: callback to notify the caller about the status of the task
 *  @return: true if a task is executed successfully, otherwise false
 */
typedef bool (*ezTaskWorkerTaskFunc)(void *context, ezTaskWorkerCallbackFunc callback);


/** @brief definition of an ezTaskWorker
 */
struct ezTaskWorker
{
    struct Node node;           /**< Linked list node */
    ezQueue msg_queue;          /**< Queue containing the tasks to be executed */
};


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function: ezTaskWorker_InitializeWorker
*//** 
* @brief Initialize a TaskWorker
*
* @details This function intitializes the task queue, add the worker to list
*          of worker for managing
*
* @param[in]    worker: Worker to be initialized
* @param[in]    queue_buffer: buffer to queue task and data
* @param[in]    queue_buffer_size; size of the buffer
* @return       Return true if success, otherwise false
*
* @pre None
* @post None
*
* \b Example
* @code
*
* uint8_t queue_buff[128];
* struct ezTaskWorker worker;
*
* bool bResult = ezTaskWorker_InitializeWorker(&worker, queue_buff, 128);
* @endcode
*
* @see
*
*****************************************************************************/
bool ezTaskWorker_InitializeWorker(struct ezTaskWorker *worker,
                                   uint8_t *queue_buffer,
                                   uint32_t queue_buffer_size);

/*****************************************************************************
* Function: ezTaskWorker_EnqueueTask
*//** 
* @brief Enqueue a task and its data so that they can be executed later
*
* @details
*
* @param[in]    worker: worker that will execute the task
* @param[in]    task: pointing to the task function
* @param[in]    callback: callback to return result of the task's execution.
* @param[in]    context: context data of the task function. Normally it contains
*                        the task's function parameters
* @param[in]    contex_size: size of the context
* @return       Return true if success, otherwise false
*
* @pre None
* @post None
*
* \b Example
* @code
*
* struct Context context;
* struct ezTaskWorker worker;
*
* bool bResult = ezTaskWorker_EnqueueTask(&worker, worker_task, callback, &context, sizeof(struct Context));
* @endcode
*
* @see
*
*****************************************************************************/
bool ezTaskWorker_EnqueueTask(struct ezTaskWorker *worker,
                              ezTaskWorkerTaskFunc task,
                              ezTaskWorkerCallbackFunc callback,
                              void *context,
                              uint32_t contex_size);


/*****************************************************************************
* Function: ezTaskWorker_Run
*//** 
* @brief If no RTOS is used, this function provides the processing time to
*        the worker
*
* @details This function must be called periodically to provide processing time
*
* @param    None
* @return   None
*
* @pre None
* @post None
*
* \b Example
* @code
*
* //super loop
* while(1)
* {
*     ezTaskWorker_Run();
* }
*
* @endcode
*
* @see
*
*****************************************************************************/
void ezTaskWorker_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* EZ_TASK_WORKER_ENABLE == 1 */
#endif /* _EZ_TASK_WORKER_H */


/* End of file */
