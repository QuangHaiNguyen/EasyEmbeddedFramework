/*****************************************************************************
* Filename:         ez_threadx_port.c
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

/** @file   ez_threadx_port.c
 *  @author Hai Nguyen
 *  @date   06.04.2024
 *  @brief  Porting of the task worker rtos interface
 *
 *  @details
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_THREADX_PORT == 1)
#include <stdbool.h>
#include <stdint.h>

#define DEBUG_LVL   LVL_DEBUG               /**< logging level */
#define MOD_NAME    "ez_threadx_port"       /**< module name */

#include "ez_logging.h"
#include "ez_threadx_port.h"
#include "tx_api.h"

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define SEMAPHORE_COUNT     1 /* support only semaphore with count 1 aka mutex*/

/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
static struct ezTaskWorkerThreadInterfaces interfaces;
static TX_BYTE_POOL threadx_byte_pool;
static CHAR *mem_pointer = NULL;
static bool interface_initialized = false;

/*****************************************************************************
* Function Definitions
*****************************************************************************/
static EZ_RTOS_STATUS ezThreadXPort_CreateThread(struct ezTaskWorker *worker,
                                                 void *thread_func);
static EZ_RTOS_STATUS ezThreadXPort_CreateSemaphore(struct ezTaskWorker *worker);
static EZ_RTOS_STATUS ezThreadXPort_GiveSemaphore(struct ezTaskWorker *worker);
static EZ_RTOS_STATUS ezThreadXPort_TakeSemaphore(struct ezTaskWorker *worker, uint32_t tick_to_wait);
static EZ_RTOS_STATUS ezThreadXPort_CreateEvent(struct ezTaskWorker *worker);
static EZ_RTOS_STATUS ezThreadXPort_SetEvent(struct ezTaskWorker *worker, uint32_t events);
static EZ_RTOS_STATUS ezThreadXPort_GetEvent(struct ezTaskWorker *worker,
                                             uint32_t events,
                                             uint32_t tick_to_wait);
static void ezThreadXPort_PrintThreadXStatusCode(UINT code);


/*****************************************************************************
* Public functions
*****************************************************************************/
bool ezThreadXPort_Init(void *first_unused_memory)
{
    bool ret = false;
    UINT threadx_status = TX_THREAD_ERROR;
    EZTRACE("ezThreadXPort_Init()");
    interfaces.create_event = ezThreadXPort_CreateEvent;
    interfaces.create_semaphore = ezThreadXPort_CreateSemaphore;
    interfaces.create_thread = ezThreadXPort_CreateThread;
    interfaces.get_events = ezThreadXPort_GetEvent;
    interfaces.give_semaphore = ezThreadXPort_GiveSemaphore;
    interfaces.set_events = ezThreadXPort_SetEvent;
    interfaces.take_semaphore = ezThreadXPort_TakeSemaphore;


    /* Create a byte memory pool from which to allocate the thread stacks. */
    threadx_status = tx_byte_pool_create(&threadx_byte_pool,
                                 "threadx byte pool",
                                 first_unused_memory,
                                 THREADX_BYTE_POOL_SIZE);
    if(threadx_status == TX_SUCCESS)
    {
        EZINFO("Initialization success");
        interface_initialized = true;
    }

    return ret;
}

struct ezTaskWorkerThreadInterfaces *ezThreadXPort_GetInterface(void)
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
* Function: ezThreadXPort_CreateThread
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
* @pre ezThreadXPort_Init must be called first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezThreadXPort_Init
*
*****************************************************************************/
static EZ_RTOS_STATUS ezThreadXPort_CreateThread(struct ezTaskWorker *worker,
                                                 void *thread_func)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
    UINT threadx_status = TX_THREAD_ERROR;

    EZTRACE("ezThreadXPort_CreateThread()");
    if((worker != NULL) && (thread_func != NULL))
    {
        threadx_status = tx_byte_allocate(&threadx_byte_pool,
                                          (void**)&mem_pointer,
                                          worker->stack_size,
                                          TX_NO_WAIT);

        if(threadx_status != TX_SUCCESS)
        {
            EZERROR("Allocate memory failed");
            ezThreadXPort_PrintThreadXStatusCode(threadx_status);
        }
        else
        {
            threadx_status = tx_thread_create(&worker->thread,
                                              worker->worker_name,
                                              thread_func,
                                              0,
                                              mem_pointer,
                                              worker->stack_size,
                                              (UINT)worker->priority,
                                              (UINT)worker->priority,
                                              TX_NO_TIME_SLICE,
                                              TX_AUTO_START);
        }

        if(threadx_status != TX_SUCCESS)
        {
            ret_status = RTOS_STATUS_ERR;
            EZERROR("Create thread failed");
            ezThreadXPort_PrintThreadXStatusCode(threadx_status);
        }
        else
        {
            EZINFO("Create thread for worker = %s successfully", worker->worker_name);
            ret_status = RTOS_STATUS_OK;
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezThreadXPort_CreateSemaphore
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
* @pre ezThreadXPort_Init must be called first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezThreadXPort_Init
*
*****************************************************************************/
static EZ_RTOS_STATUS ezThreadXPort_CreateSemaphore(struct ezTaskWorker *worker)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
    UINT threadx_status = TX_THREAD_ERROR;

    EZTRACE("ezThreadXPort_CreateSemaphore()");

    if(worker != NULL)
    {
        threadx_status = tx_semaphore_create(&worker->sem,
                                             worker->worker_name,
                                             SEMAPHORE_COUNT);

        if(threadx_status != TX_SUCCESS)
        {
            ret_status = RTOS_STATUS_ERR;
            EZERROR("Create semaphore failed");
            ezThreadXPort_PrintThreadXStatusCode(threadx_status);
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
* Function: ezThreadXPort_GiveSemaphore
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
* @see ezThreadXPort_CreateSemaphore
*
*****************************************************************************/
static EZ_RTOS_STATUS ezThreadXPort_GiveSemaphore(struct ezTaskWorker *worker)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
    UINT threadx_status = TX_THREAD_ERROR;

    EZTRACE("ezThreadXPort_GiveSemaphore()");

    if(worker != NULL)
    {
        threadx_status = tx_semaphore_put(&worker->sem);

        if(threadx_status != TX_SUCCESS)
        {
            ret_status = RTOS_STATUS_ERR;
            EZERROR("Give semaphore failed");
            ezThreadXPort_PrintThreadXStatusCode(threadx_status);
        }
        else
        {
            ret_status = RTOS_STATUS_OK;
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezThreadXPort_TakeSemaphore
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
*               RTOS_STATUS_ERR: take semaphore error
*               RTOS_STATUS_ERR_ARG: wrong input arguments
*
* @pre semaphore must be created first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezThreadXPort_CreateSemaphore
*
*****************************************************************************/
static EZ_RTOS_STATUS ezThreadXPort_TakeSemaphore(struct ezTaskWorker *worker, uint32_t tick_to_wait)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
    UINT threadx_status = TX_THREAD_ERROR;
    ULONG wait_ticks = TX_NO_WAIT;

    EZTRACE("ezThreadXPort_TakeSemaphore()");

    if(worker != NULL)
    {
        if(tick_to_wait == EZ_THREAD_WAIT_NO)
        {
            wait_ticks = TX_NO_WAIT;
        }
        else if (tick_to_wait == EZ_THREAD_WAIT_FOREVER)
        {
            wait_ticks = TX_WAIT_FOREVER ;
        }
        else
        {
            wait_ticks = tick_to_wait;
        }

        threadx_status = tx_semaphore_get(&worker->sem, wait_ticks);

        /* Operation OK */
        if(threadx_status == TX_SUCCESS)
        {
            ret_status = RTOS_STATUS_OK;
            EZDEBUG("Semaphore taken");
        }
        /* Operation OK but could not get the semaphore within wait_ticks */
        else if(threadx_status == TX_NO_INSTANCE)
        {
            ret_status = RTOS_STATUS_OK_TIMEOUT;
            EZDEBUG("take semaphore timeout ");
        }
        /* Error */
        else
        {
            ret_status = RTOS_STATUS_ERR;
            ezThreadXPort_PrintThreadXStatusCode(threadx_status);
            EZERROR("Take semaphore failed");
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezThreadXPort_CreateEvent
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
* @pre ezThreadXPort_Init must be called first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezThreadXPort_Init
*
*****************************************************************************/
static EZ_RTOS_STATUS ezThreadXPort_CreateEvent(struct ezTaskWorker *worker)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
    UINT threadx_status = TX_THREAD_ERROR;

    EZTRACE("ezThreadXPort_CreateEvent()");

    if(worker != NULL)
    {
        threadx_status = tx_event_flags_create(&worker->events, worker->worker_name);

        if(threadx_status != TX_SUCCESS)
        {
            ret_status = RTOS_STATUS_ERR;
            EZERROR("Create event failed");
            ezThreadXPort_PrintThreadXStatusCode(threadx_status);
        }
        else
        {
            ret_status = RTOS_STATUS_OK;
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezThreadXPort_SetEvent
*//** 
* @brief Set an event by calling tx_event_flags_set
*
* @details
*
* @param[in]    worker: pointer to the task worker who "owns" the thread
* @param[in]    events: events to set
* @return       RTOS_STATUS_OK: success
*               RTOS_STATUS_ERR: cannot set event
*               RTOS_STATUS_ERR_ARG: wrong input arguments
*
* @pre event must be created first
* @post None
*
* \b Example
* @endcode
*
* @see ezThreadXPort_CreateEvent
*
*****************************************************************************/
static EZ_RTOS_STATUS ezThreadXPort_SetEvent(struct ezTaskWorker *worker, uint32_t events)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
    UINT threadx_status = TX_THREAD_ERROR;

    EZTRACE("ezThreadXPort_SetEvent()");

    if(worker != NULL)
    {
        threadx_status = tx_event_flags_set(&worker->events, events, TX_OR);

        if(threadx_status != TX_SUCCESS)
        {
            ret_status = RTOS_STATUS_ERR;
            EZERROR("Set event failed");
            ezThreadXPort_PrintThreadXStatusCode(threadx_status);
        }
        else
        {
            ret_status = RTOS_STATUS_OK;
            EZDEBUG("Set event = %x successfully", events);
        }
    }

    return ret_status;
}


/*****************************************************************************
* Function: ezThreadXPort_GetEvent
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
* @return       RTOS_STATUS_OK: success
*               RTOS_STATUS_OK_TIMEOUT: success but there is no event within tick_to_wait
*               RTOS_STATUS_ERR: get event error
*               RTOS_STATUS_ERR_ARG: wrong input arguments
*
* @pre event must be created first
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezThreadXPort_CreateEvent
*
*****************************************************************************/
static EZ_RTOS_STATUS ezThreadXPort_GetEvent(struct ezTaskWorker *worker, uint32_t events, uint32_t tick_to_wait)
{
    EZ_RTOS_STATUS ret_status = RTOS_STATUS_ERR_ARG;
    UINT threadx_status = TX_THREAD_ERROR;
    ULONG wait_option = TX_NO_WAIT;
    ULONG actual_flags;

    EZTRACE("ezThreadXPort_GetEvent()");

    if(worker != NULL)
    {
        if(tick_to_wait == EZ_THREAD_WAIT_NO)
        {
            wait_option = TX_NO_WAIT;
        }
        else if (tick_to_wait == EZ_THREAD_WAIT_FOREVER)
        {
            wait_option = TX_WAIT_FOREVER ;
        }
        else
        {
            wait_option = tick_to_wait;
        }

        threadx_status = tx_event_flags_get(&worker->events, events, TX_OR_CLEAR, &actual_flags, wait_option);

        /* Operation OK */
        if(threadx_status == TX_SUCCESS)
        {
            ret_status = RTOS_STATUS_OK;
            EZDEBUG("Get event = %x successfully", actual_flags);
        }
        /* Operation OK but there is no event within wait_ticks */
        else if(threadx_status == TX_NO_EVENTS )
        {
            ret_status = RTOS_STATUS_OK_TIMEOUT;
            EZDEBUG("Get event = %x timeout", actual_flags);
        }
        /* Error */
        else
        {
            ret_status = RTOS_STATUS_ERR;
            EZERROR("Get event failed");
            ezThreadXPort_PrintThreadXStatusCode(threadx_status);
        }
    }

    return ret_status;
}



/*****************************************************************************
* Function: ezThreadXPort_PrintThreadXStatusCode
*//** 
* @brief Print the threadx_status error code.
*
* @details
*
* @param[in]    code: error code to be printed
* @return       None
*
* @pre None
* @post None
*
* \b Example
* @code
* ezThreadXPort_PrintThreadXStatusCode(TX_POOL_ERROR);
* @endcode
*
* @see
*
*****************************************************************************/
static void ezThreadXPort_PrintThreadXStatusCode(UINT code)
{
    switch(code)
    {
        case TX_POOL_ERROR:         EZERROR("TX_POOL_ERROR"); break;
        case TX_PTR_ERROR:          EZERROR("TX_PTR_ERROR"); break;
        case TX_SIZE_ERROR:         EZERROR("TX_SIZE_ERROR"); break;
        case TX_CALLER_ERROR:       EZERROR("TX_CALLER_ERROR"); break;
        case TX_GROUP_ERROR:        EZERROR("TX_GROUP_ERROR"); break;
        case TX_DELETED:            EZERROR("TX_DELETED"); break;
        case TX_NO_MEMORY:          EZERROR("TX_NO_MEMORY"); break;
        case TX_WAIT_ABORTED:       EZERROR("TX_WAIT_ABORTED"); break;
        case TX_WAIT_ERROR:         EZERROR("TX_WAIT_ERROR"); break;
        case TX_SEMAPHORE_ERROR:    EZERROR("TX_SEMAPHORE_ERROR"); break;
        case TX_OPTION_ERROR:       EZERROR("TX_OPTION_ERROR"); break;
        case TX_NOT_AVAILABLE:      EZERROR("TX_NOT_AVAILABLE "); break;
        case TX_MUTEX_ERROR:        EZERROR("TX_MUTEX_ERROR "); break;
        default: EZERROR("Unknown code"); break;
    }
}

#endif /* EZ_THREADX_PORT == 1 */
/* End of file*/
