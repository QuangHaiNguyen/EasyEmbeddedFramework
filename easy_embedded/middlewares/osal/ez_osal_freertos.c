/*****************************************************************************
* Filename:         ez_osal_freertos.c
* Author:           Hai Nguyen
* Original Date:    16.03.2025
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_osal_freertos.c
 *  @author Hai Nguyen
 *  @date   16.03.2025
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "ez_osal_freertos.h"

#if (EZ_FREERTOS_PORT == 1)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ez_osal_freertos"       /**< module name */
#include "ez_logging.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

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
static ezOsal_Interfaces_t freertos_interface;

/*****************************************************************************
* Function Definitions
*****************************************************************************/
static ezOsal_TaskHandle_t ezOsal_FreeRTOSTaskCreate(ezOsal_TaskConfig_t* config);
static ezSTATUS ezOsal_FreeRTOSTaskDelete(ezOsal_TaskHandle_t task_handle);
static ezSTATUS ezOsal_FreeRTOSTaskSuspend(ezOsal_TaskHandle_t task_handle);
static ezSTATUS ezOsal_FreeRTOSTaskResume(ezOsal_TaskHandle_t task_handle);

static ezOsal_SemaphoreHandle_t ezOsal_FreeRTOSSemaphoreCreate(ezOdal_SemaphoreConfig_t* config);
static ezSTATUS ezOsal_SemaphoreFreeRTOSDelete(ezOsal_SemaphoreHandle_t semaphore_handle);
static ezSTATUS ezOsal_SemaphoreFreeRTOSTake(ezOsal_SemaphoreHandle_t semaphore_handle, uint32_t timeout_ticks);
static ezSTATUS ezOsal_SemaphoreFreeRTOSGive(ezOsal_SemaphoreHandle_t semaphore_handle);

static ezOsal_TimerHandle_t ezOsal_FreeRTOSTimerCreate(const char* timer_name,
    uint32_t period_ticks,
    ezOsal_fpTimerElapseCallback timer_callback,
    void *argument);
static ezSTATUS ezOsal_FreeRTOSTimerDelete(ezOsal_TimerHandle_t timer_handle);
static ezSTATUS ezOsal_FreeRTOSTimerStart(ezOsal_TimerHandle_t timer_handle);
static ezSTATUS ezOsal_FreeRTOSTimerStop(ezOsal_TimerHandle_t timer_handle);

/*****************************************************************************
* Public functions
*****************************************************************************/
ezSTATUS ezOsal_FreeRTOSInit(void)
{
    freertos_interface.TaskCreate = ezOsal_FreeRTOSTaskCreate;
    freertos_interface.TaskDelete = ezOsal_FreeRTOSTaskDelete;
    freertos_interface.TaskSuspend = ezOsal_FreeRTOSTaskSuspend;
    freertos_interface.TaskResume = ezOsal_FreeRTOSTaskResume;

    freertos_interface.SemaphoreCreate = ezOsal_FreeRTOSSemaphoreCreate;
    freertos_interface.SemaphoreDelete = ezOsal_SemaphoreFreeRTOSDelete;
    freertos_interface.SemaphoreTake = ezOsal_SemaphoreFreeRTOSTake;
    freertos_interface.SemaphoreGive = ezOsal_SemaphoreFreeRTOSGive;

    freertos_interface.TimerCreate = ezOsal_FreeRTOSTimerCreate;
    freertos_interface.TimerDelete = ezOsal_FreeRTOSTimerDelete;
    freertos_interface.TimerStart = ezOsal_FreeRTOSTimerStart;
    freertos_interface.TimerStop = ezOsal_FreeRTOSTimerStop;
}


const ezOsal_Interfaces_t *ezOsal_FreeRTOSGetInterface(void)
{
    return &freertos_interface;
}

/*****************************************************************************
* Local functions
*****************************************************************************/
static ezOsal_TaskHandle_t ezOsal_FreeRTOSTaskCreate(ezOsal_TaskConfig_t* config)
{
    TaskHandle_t task_handle = NULL;
    if(config != NULL)
    {
        EZTRACE("ezOsal_FreeRTOSTaskCreate(task_name = %s, stack_size = %d, priority = %d)",
            config->task_name, config->stack_size, config->priority);
#if (EZ_OSAL_USE_STATIC == 1)
        task_handle = xTaskCreateStatic(config->task_function,
            config->task_name,
            config->stack_size,
            config->argument,
            config->priority,
            (StackType_t *)config->stack,
            &config->task_block);
#else
        if(xTaskCreate(task_function,
            task_name, 
            stack_size,
            argument,
            priority,
            &task_handle) == pdFAIL)
        {
            task_handle == NULL
        }
#endif /* (EZ_OSAL_USE_STATIC == 1) */
        if(task_handle == NULL)
        {
            EZERROR("Create task = %s failed", config->task_name);
        }
        else
        {
            EZDEBUG("Create task = %s successfully", config->task_name);
        }
    }
    return (ezOsal_TaskHandle_t)task_handle;;
}

static ezSTATUS ezOsal_FreeRTOSTaskDelete(ezOsal_TaskHandle_t task_handle)
{
    TaskHandle_t rtos_task_handle = (TaskHandle_t)task_handle;
    if(rtos_task_handle != NULL)
    {
        vTaskDelete(rtos_task_handle);
        return ezSUCCESS;
    }
    return ezFAIL;
}


static ezSTATUS ezOsal_FreeRTOSTaskSuspend(ezOsal_TaskHandle_t task_handle)
{
    TaskHandle_t rtos_task_handle = (TaskHandle_t)task_handle;
    if(rtos_task_handle != NULL)
    {
        vTaskSuspend(rtos_task_handle);
        return ezSUCCESS;
    }
    return ezFAIL;
}

static ezSTATUS ezOsal_FreeRTOSTaskResume(ezOsal_TaskHandle_t task_handle)
{
    TaskHandle_t rtos_task_handle = (TaskHandle_t)task_handle;
    if(rtos_task_handle != NULL)
    {
        vTaskResume(rtos_task_handle);
        return ezSUCCESS;
    }
    return ezFAIL;
}

static ezOsal_SemaphoreHandle_t ezOsal_FreeRTOSSemaphoreCreate(ezOdal_SemaphoreConfig_t* config)
{
    SemaphoreHandle_t semaphore_handle = NULL;
    if(config != NULL)
    {
        EZTRACE("ezOsal_FreeRTOSSemaphoreCreate(max_count = %d)", config->max_count);
#if (EZ_OSAL_USE_STATIC == 1)
        semaphore_handle = xSemaphoreCreateCountingStatic(config->max_count, 0, &config->xSemaphoreBuffer);
#else
        semaphore_handle = xSemaphoreCreateCounting(config->max_count, 0); 
#endif
    return (ezOsal_SemaphoreHandle_t)semaphore_handle;
    }
}

static ezSTATUS ezOsal_SemaphoreFreeRTOSDelete(ezOsal_SemaphoreHandle_t semaphore_handle)
{
    SemaphoreHandle_t rtos_semaphore_handle = (SemaphoreHandle_t)semaphore_handle;
    if(rtos_semaphore_handle != NULL)
    {
        vSemaphoreDelete(rtos_semaphore_handle);
        return ezSUCCESS;
    }
    return ezFAIL;
}

static ezSTATUS ezOsal_SemaphoreFreeRTOSTake(ezOsal_SemaphoreHandle_t semaphore_handle, uint32_t timeout_ticks)
{
    SemaphoreHandle_t rtos_semaphore_handle = (SemaphoreHandle_t)semaphore_handle;
    if(rtos_semaphore_handle != NULL)
    {
        if(xSemaphoreTake(rtos_semaphore_handle, timeout_ticks) == pdTRUE)
        {
            return ezSUCCESS;
        }
    }
    return ezFAIL;
}

static ezSTATUS ezOsal_SemaphoreFreeRTOSGive(ezOsal_SemaphoreHandle_t semaphore_handle)
{
    SemaphoreHandle_t rtos_semaphore_handle = (SemaphoreHandle_t)semaphore_handle;
    if(rtos_semaphore_handle != NULL)
    {
        xSemaphoreGive(rtos_semaphore_handle);
        return ezSUCCESS;
    }
    return ezFAIL;
}

static ezOsal_TimerHandle_t ezOsal_FreeRTOSTimerCreate(const char* timer_name,
    uint32_t period_ticks,
    ezOsal_fpTimerElapseCallback timer_callback,
    void *argument)
{
    TimerHandle_t timer_handle = NULL;
    if(timer_name != NULL)
    {
        EZTRACE("ezOsal_FreeRTOSTimerCreate(name = %s, period_ticks = %d)", timer_name, period_ticks);
        timer_handle = xTimerCreate(timer_name, period_ticks, pdTRUE, argument, (TimerCallbackFunction_t)timer_callback);
    }
    return timer_handle;
}

static ezSTATUS ezOsal_FreeRTOSTimerDelete(ezOsal_TimerHandle_t timer_handle)
{
    TimerHandle_t rtos_timer_handle = (TimerHandle_t)timer_handle;
    if(rtos_timer_handle != NULL)
    {
        xTimerDelete(rtos_timer_handle, 0);
        return ezSUCCESS;
    }
    return ezFAIL;
}

static ezSTATUS ezOsal_FreeRTOSTimerStart(ezOsal_TimerHandle_t timer_handle)
{
    timer_handle = (TimerHandle_t)timer_handle;
    if(timer_handle != NULL)
    {
        if(xTimerStart(timer_handle, 0) == pdPASS)
        {
            return ezSUCCESS;
        }
    }
    return ezFAIL;
}

static ezSTATUS ezOsal_FreeRTOSTimerStop(ezOsal_TimerHandle_t timer_handle)
{
    timer_handle = (TimerHandle_t)timer_handle;
    if(timer_handle != NULL)
    {
        if(xTimerStop(timer_handle, 0) == pdPASS)
        {
            return ezSUCCESS;
        }
    }
    return ezFAIL;
}



#endif /* EZ_FREERTOS_PORT == 1 */
/* End of file*/
