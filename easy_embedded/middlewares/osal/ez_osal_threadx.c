/*****************************************************************************
* Filename:         ez_osal_threadx.c
* Author:           Hai Nguyen
* Original Date:    21.03.2025
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_osal_threadx.c
 *  @author Hai Nguyen
 *  @date   21.03.2025
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "ez_osal_threadx.h"

#if (EZ_THREADX_PORT == 1)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ez_osal_threadx"       /**< module name */
#include "ez_logging.h"
#include "tx_api.h"


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
static TX_BYTE_POOL threadx_byte_pool;
static CHAR *mem_pointer = NULL;

/*****************************************************************************
* Function Definitions
*****************************************************************************/

static ezSTATUS ezOsal_ThreadXInit(void* argument);
static ezOsal_TaskHandle_t ezOsal_ThreadXTaskCreate(ezOsal_TaskConfig_t* config);
static ezSTATUS ezOsal_ThreadXTaskDelete(ezOsal_TaskHandle_t task_handle);
static ezSTATUS ezOsal_ThreadXTaskSuspend(ezOsal_TaskHandle_t task_handle);
static ezSTATUS ezOsal_ThreadXTaskResume(ezOsal_TaskHandle_t task_handle);
static ezSTATUS ezOsal_ThreadXTaskDelay(unsigned long num_of_ticks);
static unsigned long ezOsal_ThreadXTaskGetTickCount(void);
static void ezOsal_ThreadXTaskStartScheduler(void);

static ezOsal_SemaphoreHandle_t ezOsal_ThreadXSemaphoreCreate(ezOsal_SemaphoreConfig_t* config);
static ezSTATUS ezOsal_SemaphoreThreadXDelete(ezOsal_SemaphoreHandle_t semaphore_handle);
static ezSTATUS ezOsal_SemaphoreThreadXTake(ezOsal_SemaphoreHandle_t semaphore_handle, uint32_t timeout_ticks);
static ezSTATUS ezOsal_SemaphoreThreadXGive(ezOsal_SemaphoreHandle_t semaphore_handle);

static ezOsal_TimerHandle_t ezOsal_ThreadXTimerCreate(ezOsal_TimerConfig_t *config);
static ezSTATUS ezOsal_ThreadXTimerDelete(ezOsal_TimerHandle_t timer_handle);
static ezSTATUS ezOsal_ThreadXTimerStart(ezOsal_TimerHandle_t timer_handle);
static ezSTATUS ezOsal_ThreadXTimerStop(ezOsal_TimerHandle_t timer_handle);

static const ezOsal_Interfaces_t threadx_interface = {
    .Init = ezOsal_ThreadXInit,
    .TaskCreate = ezOsal_ThreadXTaskCreate,
    .TaskDelete = ezOsal_ThreadXTaskDelete,
    .TaskSuspend = ezOsal_ThreadXTaskSuspend,
    .TaskResume = ezOsal_ThreadXTaskResume,
    .TaskDelay = ezOsal_ThreadXTaskDelay,
    .TaskGetTickCount = ezOsal_ThreadXTaskGetTickCount,
    .TaskStartScheduler = ezOsal_ThreadXTaskStartScheduler,

    .SemaphoreCreate = ezOsal_ThreadXSemaphoreCreate,
    .SemaphoreDelete = ezOsal_SemaphoreThreadXDelete,
    .SemaphoreTake = ezOsal_SemaphoreThreadXTake,
    .SemaphoreGive = ezOsal_SemaphoreThreadXGive,

    .TimerCreate = ezOsal_ThreadXTimerCreate,
    .TimerDelete = ezOsal_ThreadXTimerDelete,
    .TimerStart = ezOsal_ThreadXTimerStart,
    .TimerStop = ezOsal_ThreadXTimerStop,
};

static void ezOsal_ThreadXPrintStatusCode(UINT code);


/*****************************************************************************
* Public functions
*****************************************************************************/
const ezOsal_Interfaces_t *ezOsal_ThreadXGetInterface(void)
{
    return &threadx_interface;
}

/*****************************************************************************
* Local functions
*****************************************************************************/
static ezSTATUS ezOsal_ThreadXInit(void* argument)
{
    UINT threadx_status = TX_THREAD_ERROR;

    threadx_status = tx_byte_pool_create(&threadx_byte_pool, NULL, argument, 4096);
    
    if(threadx_status == TX_SUCCESS)
    {
        EZINFO("Initialization success");
        return ezSUCCESS;
    }

    ezOsal_ThreadXPrintStatusCode(threadx_status);
    return ezFAIL;
}


static ezOsal_TaskHandle_t ezOsal_ThreadXTaskCreate(ezOsal_TaskConfig_t* config)
{
    UINT status = TX_THREAD_ERROR;
    if(config != NULL)
    {
        status = tx_byte_allocate(&threadx_byte_pool, 
            (void**)&mem_pointer,
            config->stack_size,
            TX_NO_WAIT);

        if(status == TX_SUCCESS)
        {
            status = tx_thread_create((TX_THREAD*)config->static_resource,
                (CHAR *)config->task_name,
                (VOID (*)(ULONG))config->task_function,
                (ULONG)((uintptr_t)config->argument),
                mem_pointer,
                config->stack_size,
                config->priority,
                config->priority,
                TX_NO_TIME_SLICE,
                TX_AUTO_START);

            if(status == TX_SUCCESS)
            {
                return (ezOsal_TaskHandle_t)config->static_resource;
            }
            else
            {
                ezOsal_ThreadXPrintStatusCode(status);
            }
        }
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return NULL;
}


static ezSTATUS ezOsal_ThreadXTaskDelete(ezOsal_TaskHandle_t task_handle)
{
    UINT status = tx_thread_terminate(task_handle);
    if(status == TX_SUCCESS)
    {
        status = tx_thread_delete(task_handle);
        if(status == TX_SUCCESS)
        {
            task_handle = NULL;
            return ezSUCCESS;
        }
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return ezFAIL;
}


static ezSTATUS ezOsal_ThreadXTaskSuspend(ezOsal_TaskHandle_t task_handle)
{
    UINT status = tx_thread_suspend(task_handle);
    if(status == TX_SUCCESS)
    {
        return ezSUCCESS;
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return ezFAIL;
}


static ezSTATUS ezOsal_ThreadXTaskResume(ezOsal_TaskHandle_t task_handle)
{
    UINT status = tx_thread_resume(task_handle);
    if(status == TX_SUCCESS)
    {
        return ezSUCCESS;
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return ezFAIL;
}


static ezSTATUS ezOsal_ThreadXTaskDelay(unsigned long num_of_ticks)
{
    tx_thread_sleep(num_of_ticks);
    return ezFAIL;
}


static unsigned long ezOsal_ThreadXTaskGetTickCount(void)
{
    return 0;
}


static void ezOsal_ThreadXTaskStartScheduler(void)
{
    tx_kernel_enter();
}

static ezOsal_SemaphoreHandle_t ezOsal_ThreadXSemaphoreCreate(ezOsal_SemaphoreConfig_t* config)
{
    UINT status = TX_THREAD_ERROR;
    if(config != NULL)
    {
        status = tx_semaphore_create((TX_SEMAPHORE*)config->static_resource, (CHAR *)NULL, config->max_count);
        if(status == TX_SUCCESS)
        {
            return (ezOsal_SemaphoreHandle_t)config->static_resource;
        }
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return NULL;
}


static ezSTATUS ezOsal_SemaphoreThreadXDelete(ezOsal_SemaphoreHandle_t semaphore_handle)
{
    UINT status = TX_THREAD_ERROR;
    status = tx_semaphore_delete(semaphore_handle);
    if(status == TX_SUCCESS)
    {
        semaphore_handle = NULL;
        return ezSUCCESS;
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return ezFAIL;
}


static ezSTATUS ezOsal_SemaphoreThreadXTake(ezOsal_SemaphoreHandle_t semaphore_handle,
    uint32_t timeout_ticks)
{
    UINT status = TX_THREAD_ERROR;
    status = tx_semaphore_get(semaphore_handle, timeout_ticks);
    if(status == TX_SUCCESS)
    {
        return ezSUCCESS;
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return ezFAIL;
}


static ezSTATUS ezOsal_SemaphoreThreadXGive(ezOsal_SemaphoreHandle_t semaphore_handle)
{
    UINT status = TX_THREAD_ERROR;
    status = tx_semaphore_put(semaphore_handle);
    if(status == TX_SUCCESS)
    {
        return ezSUCCESS;
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return ezFAIL;
}

static ezOsal_TimerHandle_t ezOsal_ThreadXTimerCreate(ezOsal_TimerConfig_t *config)
{
    UINT status = TX_THREAD_ERROR;
    if(config != NULL)
    {
        status = tx_timer_create((TX_TIMER*)config->static_resource,
            (CHAR *)config->timer_name,
            (VOID (*)(ULONG))config->timer_callback,
            (ULONG)((uintptr_t)config->argument),
            config->period_ticks,
            config->period_ticks,
            TX_NO_ACTIVATE);
        if(status == TX_SUCCESS)
        {
            return (ezOsal_TimerHandle_t)config->static_resource;
        }
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return NULL;
}


static ezSTATUS ezOsal_ThreadXTimerDelete(ezOsal_TimerHandle_t timer_handle)
{
    UINT status = tx_timer_delete(timer_handle);
    if(status == TX_SUCCESS)
    {
        timer_handle = NULL;
        return ezSUCCESS;
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return ezFAIL;
}


static ezSTATUS ezOsal_ThreadXTimerStart(ezOsal_TimerHandle_t timer_handle)
{
    UINT status = tx_timer_activate(timer_handle);
    if(status == TX_SUCCESS)
    {
        return ezSUCCESS;
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return ezFAIL;
}


static ezSTATUS ezOsal_ThreadXTimerStop(ezOsal_TimerHandle_t timer_handle)
{
    UINT status = tx_timer_deactivate(timer_handle);
    if(status == TX_SUCCESS)
    {
        return ezSUCCESS;
    }
    ezOsal_ThreadXPrintStatusCode(status);
    return ezFAIL;
}


static void ezOsal_ThreadXPrintStatusCode(UINT code)
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
        case TX_NO_INSTANCE:        EZERROR("TX_NO_INSTANCE "); break;
        default: EZERROR("Unknown code"); break;
    }
}

#endif /* EZ_OSAL_THREADX_ENABLE == 1 */
/* End of file*/
