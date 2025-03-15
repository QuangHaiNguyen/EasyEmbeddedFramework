/*****************************************************************************
* Filename:         ez_freertos_port.c
* Author:           Hai Nguyen
* Original Date:    06.04.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_freertos_port.c
 *  @author Hai Nguyen
 *  @date   06.04.2024
 *  @brief  Porting of the task worker rtos interface
 *
 *  @details
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_FREERTOS_PORT == 1)
#include <stdbool.h>
#include <stdint.h>

#define DEBUG_LVL   LVL_INFO               /**< logging level */
#define MOD_NAME    "ez_freertos_port"       /**< module name */

#include "ez_logging.h"
#include "ez_freertos_port.h"


/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
static struct ezTaskWorkerThreadInterfaces interfaces;
static bool interface_initialized = false;
static StackType_t TaskStack[FREERTOS_STACK_SIZE];
static StackType_t used_stack_size = 0;

/*****************************************************************************
* Function Definitions
*****************************************************************************/
static EZ_RTOS_STATUS ezFreeRTOSPort_CreateThread(struct ezTaskWorker *worker,
                                                  void *thread_func);
static EZ_RTOS_STATUS ezFreeRTOSPort_CreateSemaphore(struct ezTaskWorker *worker);
static EZ_RTOS_STATUS ezFreeRTOSPort_GiveSemaphore(struct ezTaskWorker *worker);
static EZ_RTOS_STATUS ezFreeRTOSPort_TakeSemaphore(struct ezTaskWorker *worker,
                                                   uint32_t tick_to_wait);
static EZ_RTOS_STATUS ezFreeRTOSPort_CreateEvent(struct ezTaskWorker *worker);
static EZ_RTOS_STATUS ezFreeRTOSPort_SetEvent(struct ezTaskWorker *worker,
                                              uint32_t events);
static EZ_RTOS_STATUS ezFreeRTOSPort_GetEvent(struct ezTaskWorker *worker,
                                              uint32_t events,
                                              uint32_t tick_to_wait);


/*****************************************************************************
* Public functions
*****************************************************************************/
bool ezFreeRTOSPort_Init(void)
{
    EZTRACE("ezFreeRTOSPort_Init()");
    interfaces.create_event = ezFreeRTOSPort_CreateEvent;
    interfaces.create_semaphore = ezFreeRTOSPort_CreateSemaphore;
    interfaces.create_thread = ezFreeRTOSPort_CreateThread;
    interfaces.get_events = ezFreeRTOSPort_GetEvent;
    interfaces.give_semaphore = ezFreeRTOSPort_GiveSemaphore;
    interfaces.set_events = ezFreeRTOSPort_SetEvent;
    interfaces.take_semaphore = ezFreeRTOSPort_TakeSemaphore;
    interface_initialized = true;
    used_stack_size = 0;
    return true;
}


struct ezTaskWorkerThreadInterfaces *ezFreeRTOSPort_GetInterface(void)
{
    if(interface_initialized == true)
    {
        return &interfaces;
    }
    else
    {
        return NULL;
    }
}


/*****************************************************************************
* Local functions
*****************************************************************************/

/*****************************************************************************
* Function: ezFreeRTOSPort_CreateThread
*//** 
* @brief This function create a thread by calling tx_thread_create
*
* @details
*
* @param[in]    worker: pointer to the task worker who "owns" the thread
* @param[in]    thread_func: threadx function
* @return       RTOS_STATUS_OK: success
*               RTOS_STATUS_ERR: cannot create thread
*               RTOS_STATUS_ERR_ARG: wrong input arguments
*
* @pre ezFreeRTOSPort_Init must be called first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezFreeRTOSPort_Init
*
*****************************************************************************/
static EZ_RTOS_STATUS ezFreeRTOSPort_CreateThread(struct ezTaskWorker *worker,
                                                  void *thread_func)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
#if (EZ_RTOS_USE_STATIC_ALLOC == 1)
    TaskHandle_t xHandle = NULL;
#else
    BaseType_t xReturned = pdFAIL;
#endif

    EZTRACE("ezFreeRTOSPort_CreateThread()");
    if((worker != NULL) && (thread_func != NULL))
    {
        /* stack still big enough to hold the thread */
        if(worker->stack_size + used_stack_size < FREERTOS_STACK_SIZE)
        {
            if(worker->priority > configMAX_PRIORITIES - 1U)
            {
                worker->priority = configMAX_PRIORITIES - 1U;
            }

#if (EZ_RTOS_USE_STATIC_ALLOC == 1)
            xHandle = xTaskCreateStatic(thread_func,
                                        worker->worker_name,
                                        worker->stack_size,
                                        NULL,
                                        worker->priority,
                                        &TaskStack[used_stack_size],
                                        &worker->thread);
            used_stack_size += worker->stack_size;
#else
            xReturned = xTaskCreate(thread_func,
                                  worker->worker_name,
                                  worker->stack_size,
                                  NULL,
                                  worker->priority,
                                  NULL);
#endif /* (EZ_RTOS_USE_STATIC_ALLOC == 1) */
        }
#if (EZ_RTOS_USE_STATIC_ALLOC == 1)
        if(xHandle == NULL)
#else
        if(xReturned == pdFAIL)
#endif /* (EZ_RTOS_USE_STATIC_ALLOC == 1) */
        {
            ret_status = RTOS_STATUS_ERR;
            EZERROR("Create thread failed");
        }
        else
        {
            ret_status = RTOS_STATUS_OK;
            EZINFO("Create thread for worker = %s successfully", worker->worker_name);
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezFreeRTOSPort_CreateSemaphore
*//** 
* @brief Create the semaphore using tx_semaphore_create
*
* @details
*
* @param[in]    worker: pointer to the task worker who "owns" the thread
* @return       RTOS_STATUS_OK: success
*               RTOS_STATUS_ERR: cannot create semaphore
*               RTOS_STATUS_ERR_ARG: wrong input arguments
*
* @pre ezFreeRTOSPort_Init must be called first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezFreeRTOSPort_Init
*
*****************************************************************************/
static EZ_RTOS_STATUS ezFreeRTOSPort_CreateSemaphore(struct ezTaskWorker *worker)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;

    EZTRACE("ezFreeRTOSPort_CreateSemaphore()");
    if(worker != NULL)
    {
#if (EZ_RTOS_USE_STATIC_ALLOC == 1)
        worker->semaphore_h = xSemaphoreCreateMutexStatic(&worker->sem);
#else
        worker->semaphore_h = xSemaphoreCreateMutex();
#endif /* (EZ_RTOS_USE_STATIC_ALLOC == 1) */
        if(worker->semaphore_h == NULL)
        {
            EZERROR("Create semaphore failed");
            ret_status = RTOS_STATUS_ERR;
        }
        else
        {
            EZINFO("Create semaphore for worker = %s successfully", worker->worker_name);
            ret_status = RTOS_STATUS_OK;
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezFreeRTOSPort_GiveSemaphore
*//** 
* @brief Release the threadx semaphore by calling tx_semaphore_put
*
* @details
*
* @param[in]    worker: pointer to the task worker who "owns" the thread
* @return       RTOS_STATUS_OK: success
*               RTOS_STATUS_ERR: cannot give semaphore
*               RTOS_STATUS_ERR_ARG: wrong input arguments
*
* @pre semaphore must be created first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezFreeRTOSPort_CreateSemaphore
*
*****************************************************************************/
static EZ_RTOS_STATUS ezFreeRTOSPort_GiveSemaphore(struct ezTaskWorker *worker)
{
    BaseType_t rtos_status = pdFALSE;
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;

    EZTRACE("ezFreeRTOSPort_GiveSemaphore()");
    if(worker != NULL)
    {
        rtos_status = xSemaphoreGive(worker->semaphore_h);
        if(rtos_status != pdTRUE)
        {
            ret_status = RTOS_STATUS_ERR;
            EZERROR("Give semaphore failed");
        }
        else
        {
            ret_status = RTOS_STATUS_OK;
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezFreeRTOSPort_TakeSemaphore
*//** 
* @brief Take the threadx semaphore by calling tx_semaphore_get
*
* @details
*
* @param[in]    worker: pointer to the task worker who "owns" the thread
* @param[in]    tick_to_wait: number of tick to wait for the semaphore's availability.
*               EZ_THREAD_WAIT_NO for no wait and EZ_THREAD_WAIT_FOREVER to wait until
*               semphore available
* @return       RTOS_STATUS_OK: success
*               RTOS_STATUS_OK_TIMEOUT: success but could not take semaphore within tick_to_wait
*               RTOS_STATUS_ERR_ARG: wrong input arguments
*
* @pre semaphore must be created  first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezFreeRTOSPort_CreateSemaphore
*
*****************************************************************************/
static EZ_RTOS_STATUS ezFreeRTOSPort_TakeSemaphore(struct ezTaskWorker *worker,
                                                   uint32_t tick_to_wait)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
    BaseType_t rtos_status = pdFALSE;
    TickType_t wait_option = 0;

    EZTRACE("ezFreeRTOSPort_TakeSemaphore()");
    if(worker != NULL)
    {
        if(tick_to_wait == EZ_THREAD_WAIT_NO)
        {
            wait_option = 0;
        }
        else if (tick_to_wait == EZ_THREAD_WAIT_FOREVER)
        {
            wait_option = portMAX_DELAY ;
        }
        else
        {
            wait_option = tick_to_wait;
        }

        rtos_status = xSemaphoreTake(worker->semaphore_h, wait_option);

        if(rtos_status != pdTRUE)
        {
            ret_status = RTOS_STATUS_OK_TIMEOUT;
            EZERROR("Take semaphore timeout");
        }
        else
        {
            ret_status = RTOS_STATUS_OK;
            EZDEBUG("Semaphore taken");
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezFreeRTOSPort_CreateEvent
*//** 
* @brief Create the threadx event group by calling tx_event_flags_create
*
* @details
*
* @param[in]    worker: pointer to the task worker who "owns" the thread
* @return       RTOS_STATUS_OK: success
*               RTOS_STATUS_ERR: cannot create event
*               RTOS_STATUS_ERR_ARG: wrong input arguments
*
* @pre ezFreeRTOSPort_Init must be called first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezFreeRTOSPort_Init
*
*****************************************************************************/
static EZ_RTOS_STATUS ezFreeRTOSPort_CreateEvent(struct ezTaskWorker *worker)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;

    EZTRACE("ezFreeRTOSPort_CreateEvent()");
    if(worker != NULL)
    {
#if (EZ_RTOS_USE_STATIC_ALLOC == 1)
        worker->events_h = xEventGroupCreateStatic(&worker->events);
#else
        worker->events_h = xEventGroupCreate();
#endif /* (EZ_RTOS_USE_STATIC_ALLOC == 1) */

        if(worker->events_h == NULL)
        {
            ret_status = RTOS_STATUS_ERR;
            EZERROR("Create event failed");
        }
        else
        {
            ret_status = RTOS_STATUS_OK;
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezFreeRTOSPort_SetEvent
*//** 
* @brief Set an event by calling tx_event_flags_set
*
* @details
*
* @param[in]    worker: pointer to the task worker who "owns" the thread
* @param[in]    events: events to set
* @return       RTOS_STATUS_OK: success
*
* @pre event must be created first
* @post None
*
* \b Example
* @endcode
*
* @see ezFreeRTOSPort_CreateEvent
*
*****************************************************************************/
static EZ_RTOS_STATUS ezFreeRTOSPort_SetEvent(struct ezTaskWorker *worker,
                                              uint32_t events)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;

    EZTRACE("ezFreeRTOSPort_SetEvent()");
    if(worker != NULL)
    {
        (void)xEventGroupSetBits(worker->events_h, events);
        EZDEBUG("Set event = %x successfully", events);
        ret_status = RTOS_STATUS_OK;
    }
}


/*****************************************************************************
* Function: ezFreeRTOSPort_GetEvent
*//** 
* @brief Get the event by calling tx_event_flags_get
*
* @details
*
* @param[in]    worker: pointer to the task worker who "owns" the thread
* @param[in]    events: events to get
* @param[in]    tick_to_wait: number of tick to wait for the events' availability.
*               EZ_THREAD_WAIT_NO for no wait and EZ_THREAD_WAIT_FOREVER to wait until
*               events available
* @return       true if success, else false
*
* @pre eevent must be created first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezFreeRTOSPort_CreateEvent
*
*****************************************************************************/
static EZ_RTOS_STATUS ezFreeRTOSPort_GetEvent(struct ezTaskWorker *worker,
                                              uint32_t events,
                                              uint32_t tick_to_wait)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
    EventBits_t ret_bits = 0;
    TickType_t ticks_to_wait = 0;

    EZTRACE("ezFreeRTOSPort_GetEvent()");

    if(worker != NULL)
    {
        if(tick_to_wait == EZ_THREAD_WAIT_NO)
        {
            ticks_to_wait = 0;
        }
        else if (tick_to_wait == EZ_THREAD_WAIT_FOREVER)
        {
            ticks_to_wait = portMAX_DELAY ;
        }
        else
        {
            ticks_to_wait = tick_to_wait;
        }

        ret_bits = xEventGroupWaitBits(worker->events_h,
                                       events,
                                       pdTRUE,
                                       pdFALSE,
                                       ticks_to_wait);

        if((ret_bits & events) == events)
        {
            ret_status = RTOS_STATUS_OK;
            EZDEBUG("Get event = %x successfully", ret_bits);
        }
        else
        {
            ret_status = RTOS_STATUS_OK_TIMEOUT;
            EZERROR("Get event timeout");
        }
    }

    return ret_status;
}


#endif /* EZ_FREERTOS_PORT == 1 */
/* End of file*/
