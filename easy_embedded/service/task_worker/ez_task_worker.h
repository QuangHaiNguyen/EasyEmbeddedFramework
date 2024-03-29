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
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 */

#ifndef _EZ_TASK_WORKER_H
#define _EZ_TASK_WORKER_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Includes
*****************************************************************************/
/* INCLUDE HEADER HERE */

#if (EZ_TASK_WORKER_ENABLE == 1)

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define A_MACRO     1   /**< a macro*/

/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/** @brief Definition of function executing the tasks
 *  @return: true if a task is executed successfully, otherwise false
 */
typedef bool (*TaskWorker_Execute)(void);


/** @brief Definition of callback function to notify a task is finished or
 *         error occurs
 *  @param[out] event: retrun event, depending on the application
 *  @param[out] ret_data: return data, depending on the task
 *  @return: true if a task is executed successfully, otherwise false
 */
typedef void (*TaskWorker_Callback)(uint8_t event, void *ret_data);


/** @brief Definition of task function.
 *  @param[out] context: context containing data of a task
 *  @return: true if a task is executed successfully, otherwise false
 */
typedef bool (*TaskWorker_Task)(void *context);


/** @brief definition of an ezTaskWorker
 */
struct ezTaskWorker
{
    struct Node node;           /**< Linked list node */
    ezQueue msg_queue;          /**< Queue containing the tasks to be executed */
    TaskWorker_Execute execute; /**< Execute function */
};


/** @brief definition of an Header of the message
 */
struct ezTaskWorker_Header
{
    TaskWorker_Task task;
    TaskWorker_Callback callback;
};


typedef ezReservedElement ezTaskBlock;


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
* @param[in]    worker
* @param[in]    exec_func
* @param[in]    queue_buffer
* @param[in]    queue_buffer_size
* @return       Return true if success, otherwise false
*
* @pre None
* @post None
*
* \b Example
* @code
* 
* bool ExecuteTask(void); // Demonstration only
*
* uint8_t queue_buff[128];
* struct ezTaskWorker worker;
*
* bool bResult = ezTaskWorker_InitializeWorker(&worker, ExecuteTask, queue_buff, 128);
* @endcode
*
* @see
*
*****************************************************************************/
bool ezTaskWorker_InitializeWorker(struct ezTaskWorker *worker,
                                   TaskWorker_Execute exec_func,
                                   uint8_t *queue_buffer,
                                   uint32_t queue_buffer_size);

ezTaskBlock ezTaskWorker_GetTaskBlock(struct ezTaskWorker *worker,
                                      void **data,
                                      uint32_t data_size);

bool ezTaskWorker_ReleaseTaskBlock(struct ezTaskWorker *worker,
                                   ezTaskBlock task_block);

bool ezTaskWorker_EnqueueTaskBlock(struct ezTaskWorker *worker,
                                   ezTaskBlock task_block);

void ezTaskWorker_Run(void);

#ifdef __cplusplus
}
#endif

#endif /* EZ_TASK_WORKER_ENABLE == 1 */
#endif /* _EZ_TASK_WORKER_H */


/* End of file */
