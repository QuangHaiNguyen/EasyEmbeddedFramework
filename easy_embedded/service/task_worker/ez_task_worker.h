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

#if (EZ_THREADX_PORT_ENABLE == 1)
#include "tx_api.h"
#elif (EZ_FREERTOS_PORT_ENABLE == 1)
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"
#endif

#if (EZ_TASK_WORKER_ENABLE == 1)

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define EZ_THREAD_WAIT_NO       0x00        /* Thread does not wait for event, semaphore */
#define EZ_THREAD_WAIT_FOREVER  0xFFFFFFFF  /* Thread waits for event, semaphore forever */
#define EZ_EVENT_TASK_AVAIL     0x01        /* Task avaialble event */

#if (EZ_THREADX_PORT_ENABLE == 1)
#define INIT_THREAD_FUNCTIONS(worker_name) \
    static void worker_name##_thread(ULONG thread_input);\
    static void worker_name##_thread_body(void);\

#define GET_THREAD_FUNC(worker_name) \
    worker_name##_thread\

#define THREAD_FUNC(worker_name) \
    static void worker_name##_thread(ULONG thread_input)\
    {\
        while(1)\
        {\
            worker_name##_thread_body();\
            if(worker_name.sleep_ticks > 0)\
            { tx_thread_sleep(worker_name.sleep_ticks); }\
        }\
    }\
    static void worker_name##_thread_body(void)

#define INIT_WORKER(name, worker_sleep_ticks, worker_priority, worker_stack_size) \
    struct ezTaskWorker name =\
    {\
        .worker_name = #name,\
        .sleep_ticks = worker_sleep_ticks,\
        .priority = worker_priority,\
        .stack_size = worker_stack_size,\
    }

#elif (EZ_FREERTOS_PORT_ENABLE == 1)
    #define INIT_THREAD_FUNCTIONS(worker_name) \
    static void worker_name##_thread(void * parameters);\
    static void worker_name##_thread_body(void);\

#define GET_THREAD_FUNC(worker_name) \
    worker_name##_thread\

#define THREAD_FUNC(worker_name) \
    static void worker_name##_thread(void * parameters)\
    {\
        while(1)\
        {\
            worker_name##_thread_body();\
            if(worker_name.sleep_ticks > 0)\
            { vTaskDelay(worker_name.sleep_ticks); }\
        }\
    }\
    static void worker_name##_thread_body(void)

#define INIT_WORKER(name, worker_sleep_ticks, worker_priority, worker_stack_size) \
    struct ezTaskWorker name =\
    {\
        .worker_name = #name,\
        .sleep_ticks = worker_sleep_ticks,\
        .priority = worker_priority,\
        .stack_size = worker_stack_size,\
        .semaphore_h = NULL,\
        .events_h = NULL,\
    }
#else
    #define INIT_THREAD_FUNCTIONS(worker_name)
    #define GET_THREAD_FUNC(worker_name)
    #define THREAD_FUNC(worker_name)
    #define INIT_WORKER(name, worker_sleep_ticks, worker_priority, worker_stack_size)
#endif



/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/** @brief definition of an ezTaskWorker
 */
struct ezTaskWorker
{
    
    ezQueue msg_queue;              /**< Queue containing the tasks to be executed */
    char* worker_name;              /**< Name of the worker */
#if (EZ_THREADX_PORT_ENABLE == 1)
    uint8_t priority;               /**< Priority of the worker thread, the value must match the number of the activated RTOS */
    uint32_t stack_size;            /**< Stask size of the worker thread, in bytes */
    uint32_t sleep_ticks;           /**< Number of tick the thread must sleep before being activated again */
    TX_THREAD thread;               /**< ThreadX thread */
    TX_SEMAPHORE sem;               /**< ThreadX semaphore */
    TX_EVENT_FLAGS_GROUP events;    /**< ThreadX event */
#elif (EZ_FREERTOS_PORT_ENABLE == 1)
    uint8_t priority;               /**< Priority of the worker thread, the value must match the number of the activated RTOS */
    uint32_t stack_size;            /**< Stask size of the worker thread, in bytes */
    uint32_t sleep_ticks;           /**< Number of tick the thread must sleep before being activated again */
    StaticTask_t thread;
    StaticSemaphore_t sem;
    SemaphoreHandle_t semaphore_h;
    StaticEventGroup_t events;
    EventGroupHandle_t events_h;
#else
    struct Node node;               /**< Linked list node */
#endif /* EZ_THREADX_PORT_ENABLE == 1 */
};


#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))

/** @brief Return status of the RTOS interfaces
 */
typedef enum
{
    RTOS_STATUS_OK,         /**< OK */
    RTOS_STATUS_OK_TIMEOUT, /**< Operation OK, but there is timeout in RTOS operation, e.g. wait or semaphore or event */
    RTOS_STATUS_ERR,        /**< Generic error */
    RTOS_STATUS_ERR_ARG,    /**< Invalid input arguments */
    RTOS_STATUS_ERR_NO_INF, /**< No RTOS porting interface */
    RTOS_STATUS_END_LIST,   /**< End of list */
}EZ_RTOS_STATUS;


/** @brief definition of an ezTaskWorkerCreateThread.
 *  @param[in]  worker: pointer to the worker 
 *  @param[in]  thread_func; pointer to the RTOS thread function
 *  @return     RTOS_STATUS_OK if success, else one of the error code RTOS_STATUS_ERR_XXX
 */
typedef EZ_RTOS_STATUS (*ezTaskWorkerCreateThread)(struct ezTaskWorker *worker,
                                                   void *thread_func);


/** @brief definition of an ezTaskWorkerCreateSemaphore
 *  @param[in]  worker: pointer to the worker 
 *  @return     RTOS_STATUS_OK if success, else one of the error code RTOS_STATUS_ERR_XXX
 */
typedef EZ_RTOS_STATUS (*ezTaskWorkerCreateSemaphore)(struct ezTaskWorker *worker);


/** @brief definition of an ezTaskWorkerGiveSemaphore
 *  @param[in]  worker: pointer to the worker 
 *  @return     RTOS_STATUS_OK if success, else one of the error code RTOS_STATUS_ERR_XXX
 */
typedef EZ_RTOS_STATUS (*ezTaskWorkerGiveSemaphore)(struct ezTaskWorker *worker);


/** @brief definition of an ezTaskWorkerTakeSemaphore
 *  @param[in]  worker: pointer to the worker
 *  @param[in]  tick_to_wait: number of tick to wait for the semaphore.
 *                            EZ_THREAD_WAIT_NO: do not wait for semaphore
 *                            EZ_THREAD_WAIT_FOREVER: wait forever
 *  @return     RTOS_STATUS_OK if success, else one of the error code RTOS_STATUS_ERR_XXX
 */
typedef EZ_RTOS_STATUS (*ezTaskWorkerTakeSemaphore)(struct ezTaskWorker *worker, uint32_t tick_to_wait);


/** @brief definition of an ezTaskWorkerCreateEvent
 *  @param[in]  worker: pointer to the worker
 *  @return     RTOS_STATUS_OK if success, else one of the error code RTOS_STATUS_ERR_XXX
 */
typedef EZ_RTOS_STATUS (*ezTaskWorkerCreateEvent)(struct ezTaskWorker *worker);


/** @brief definition of an ezTaskWorkerSetEvent
 *  @param[in]  worker: pointer to the worker
 *  @param[in]  events: event to set. Supported event: EZ_EVENT_TASK_AVAIL
 *  @return     RTOS_STATUS_OK if success, else one of the error code RTOS_STATUS_ERR_XXX
 */
typedef EZ_RTOS_STATUS (*ezTaskWorkerSetEvent)(struct ezTaskWorker *worker, uint32_t events);


/** @brief definition of an ezTaskWorkerGetEvent
 *  @param[in]  worker: pointer to the worker
 *  @param[in]  events: event to get. Supported event: EZ_EVENT_TASK_AVAIL
 *  @param[in]  tick_to_wait: number of tick to wait for the event.
 *                            EZ_THREAD_WAIT_NO: do not wait for event
 *                            EZ_THREAD_WAIT_FOREVER: wait forever
 *  @return     RTOS_STATUS_OK if success, else one of the error code RTOS_STATUS_ERR_XXX
 */
typedef EZ_RTOS_STATUS (*ezTaskWorkerGetEvent)(struct ezTaskWorker *worker, uint32_t events, uint32_t tick_to_wait);


/** @brief Definition of the interfaces that RTOS porting
 *         must follow to let task worker running with RTOS
 */
struct ezTaskWorkerThreadInterfaces
{
    ezTaskWorkerCreateThread    create_thread;      /**< Create thread function pointer */
    ezTaskWorkerCreateSemaphore create_semaphore;   /**< Create semaphore function pointer */
    ezTaskWorkerGiveSemaphore   give_semaphore;     /**< Give semaphore function pointer */
    ezTaskWorkerTakeSemaphore   take_semaphore;     /**< Take semaphore function pointer */
    ezTaskWorkerCreateEvent     create_event;       /**< Create event function pointer */
    ezTaskWorkerSetEvent        set_events;         /**< Set event function pointer */
    ezTaskWorkerGetEvent        get_events;         /**< Get event function pointer */
};
#endif /* ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1)) */


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


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function: ezTaskWorker_CreateWorker
*//** 
* @brief Create a Task Worker
*
* @details This function create the task queue, add the worker to list
*          of worker for managing. If an RTOS is activated, it will create a
*          thread, a semephore, and an event group of the worker.
*
* @param[in]    worker: Worker to be initialized
* @param[in]    queue_buffer: buffer to queue task and data
* @param[in]    queue_buffer_size; size of the buffer
* @param[in]    thread_func: pointer to the RTOS function. Set to NULL if no RTOS is used.
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
bool ezTaskWorker_CreateWorker(struct ezTaskWorker *worker,
                               uint8_t *queue_buffer,
                               uint32_t queue_buffer_size,
                               void *thread_func);


#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
/*****************************************************************************
* Function: ezTaskWorker_SetRtosInterface
*//** 
* @brief Provide the porting RTOS interface so that the task workers can run
*        with RTOS
*
* @details Check er_threadx_port.h and .c
*
* @param[in]    interfaces: pointer to the interfaces
* @return       Return true if success, otherwise false
*
* @pre RTOS must be initialized. See ezThreadXPort_Init
* @post None
*
* \b Example
* @code
*
* struct ezTaskWorkerThreadInterfaces *threadx_interfaces = NULL;
* threadx_interfaces = ezThreadXPort_GetInterface();
* (void)ezTaskWorker_SetRtosInterface(threadx_interfaces);
* @endcode
*
* @see ezThreadXPort_Init
*
*****************************************************************************/
bool ezTaskWorker_SetRtosInterface(struct ezTaskWorkerThreadInterfaces *interfaces);
#endif

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
                              uint32_t context_size,
                              uint32_t ticks_to_wait);

#if ((EZ_THREADX_PORT_ENABLE == 1) || (EZ_FREERTOS_PORT_ENABLE == 1))
/*****************************************************************************
* Function: ezTaskWorker_ExecuteTask
*//** 
* @brief This function is call within the THREAD_FUNC to let the worker execute
*        available task
*
* @details This function is used when RTOS is activated
*
* @param[in]    worker: pointer to the worker which execute the task
* @param[in]    ticks_to_wait: number of tick to wait for task available
*               event (EZ_EVENT_TASK_AVAIL) and to wait for semaphore available.
*               Use EZ_THREAD_WAIT_NO to not to wait or EZ_THREAD_WAIT_FOREVER to
*               wait unti the event and semaphore available
* @return       None
*
* @pre worker is created by ezTaskWorker_CreateWorker and RTOS interface is set
* @post None
*
* \b Example
* @code
*
* THREAD_FUNC(worker1)
* {
*     ezTaskWorker_ExecuteTask(&worker1, EZ_THREAD_WAIT_FOREVER);
* }
*
* @endcode
*
* @see ezTaskWorker_CreateWorker, ezTaskWorker_SetRtosInterface
*
*****************************************************************************/
void ezTaskWorker_ExecuteTask(struct ezTaskWorker *worker, uint32_t ticks_to_wait);
#else
/*****************************************************************************
* Function: ezTaskWorker_ExecuteTaskNoRTOS
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
*     ezTaskWorker_ExecuteTaskNoRTOS();
* }
*
* @endcode
*
* @see
*
*****************************************************************************/
void ezTaskWorker_ExecuteTaskNoRTOS(void);

#endif

#ifdef __cplusplus
}
#endif

#endif /* EZ_TASK_WORKER_ENABLE == 1 */
#endif /* _EZ_TASK_WORKER_H */


/* End of file */
