/*****************************************************************************
* Filename:         ez_osal.h
* Author:           Hai Nguyen
* Original Date:    15.03.2025
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_osal.h
 *  @author Hai Nguyen
 *  @date   15.03.2025
 *  @brief  Common interface for OS abstraction layer
 *
 *  @details None
 */

#ifndef _EZ_OSAL_H
#define _EZ_OSAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_BUILD_WITH_CMAKE == 0U)
#include "ez_target_config.h"
#endif

#include <stdint.h>
#include "ez_utilities_common.h"

#if (EZ_OSAL == 1)

#if (EZ_FREERTOS_PORT == 1)
#include "FreeRTOS.h"
#endif

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define EZ_OSAL_HIGHEST_PRIORITY     0U   /**< Highest priority */
#define EZ_OSAL_LOWEST_PRIORITY      31U  /**< Lowest priority */

#ifndef EZ_OSAL_USE_STATIC
    #if (EZ_FREERTOS_PORT == 1)
    #define EZ_OSAL_USE_STATIC      configSUPPORT_STATIC_ALLOCATION
    #endif
#else
    #if (EZ_FREERTOS_PORT == 1)
        #if ((EZ_OSAL_USE_STATIC == 1) && (configSUPPORT_STATIC_ALLOCATION == 0))
        #error "OSAL configuration and FreeRTOS configuration are not compatible"
        #endif /* (EZ_OSAL_USE_STATIC) & (configSUPPORT_STATIC_ALLOCATION) */
    #endif 
#endif /* EZ_OSAL_USE_STATIC */


/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/**@brief OSAL task handle structure
 */
typedef void* ezOsal_TaskHandle_t;


/**@brief OSAL semaphore handle structure
 */
typedef void* ezOsal_SemaphoreHandle_t;


/**@brief OSAL timer handle structure
 */
typedef void* ezOsal_TimerHandle_t;

#if (EZ_FREERTOS_PORT == 1)
typedef StackType_t ezOsal_Stack_t;
#endif

typedef void (*ezOsal_fpTaskFunction)(void *argument);
typedef void (*ezOsal_fpTimerElapseCallback)(void *argument);


/**@brief OSAL task configuration structure
 */
typedef struct
{
    const char* task_name;  /**< Task name */
    uint32_t priority;      /**< Priority */
    void *argument;         /**< Extra argument */
    uint32_t stack_size;    /**< Stack size */
    ezOsal_fpTaskFunction task_function; /**< Task function */
#if ((EZ_FREERTOS_PORT == 1) && (EZ_OSAL_USE_STATIC == 1))
    StaticTask_t task_block; /**< Static task block */
    ezOsal_Stack_t *stack;   /**< Buffer holding stack in case using static allocation */
#endif
}ezOsal_TaskConfig_t;

/**@brief: OSAL semaphore configuration structure
 */
typedef struct
{
    uint32_t max_count;
#if ((EZ_FREERTOS_PORT == 1) && (EZ_OSAL_USE_STATIC == 1))
    StaticSemaphore_t xSemaphoreBuffer;
#endif
}ezOsal_SemaphoreConfig_t;


/* Interfaces ***************************************************************/
typedef ezSTATUS (*ezOsal_pfInit)(void);

/**@brief: Task create function
 * @param: config - Task configuration
 * @return: Task handle
 */
typedef ezOsal_TaskHandle_t (*ezOsal_fpTaskCreate)  (ezOsal_TaskConfig_t* config);

/**@brief: Task delete function
 * @param: task_handle - Task handle
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_fpTaskDelete)     (ezOsal_TaskHandle_t task_handle);

/**@brief: Task suspend function
 * @param: task_handle - Task handle
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_fpTaskSuspend)    (ezOsal_TaskHandle_t task_handle);

/**@brief: Task resume function
 * @param: task_handle - Task handle
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_fpTaskResume)     (ezOsal_TaskHandle_t task_handle);

/**@brief: Task delay function
 * @param: num_of_ticks - Number of ticks
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_fpTaskDelay)      (unsigned long num_of_ticks);

/**@brief: Task start scheduler function
 */
typedef void (*ezOsal_fpTaskStartScheduler)(void);

/**@brief: Task get tick count function
 * @return: Tick count
 */
typedef unsigned long (*ezOsal_fpTaskGetTickCount)(void);

/**@brief: Semaphore create function
 * @param: config: Semaphore configuration
 * @return: Semaphore handle
 */
typedef ezOsal_SemaphoreHandle_t (*ezOsal_fpSemaphoreCreate)(ezOsal_SemaphoreConfig_t *config);

/**@brief: Semaphore delete function
 * @param: semaphore_handle - Semaphore handle
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_fpSemaphoreDelete)(ezOsal_SemaphoreHandle_t semaphore_handle);

/**@brief: Semaphore take function
 * @param: semaphore_handle - Semaphore handle
 * @param: timeout_ticks - Timeout in ticks
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_fpSemaphoreTake)(ezOsal_SemaphoreHandle_t semaphore_handle, uint32_t timeout_ticks);

/**@brief: Semaphore give function
 * @param: semaphore_handle - Semaphore handle
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_fpSemaphoreGive)(ezOsal_SemaphoreHandle_t semaphore_handle);

/**@brief: Timer create function
 * @param: timer_name - Timer name
 * @param: period_ticks - Period in ticks
 * @param: timer_callback - Timer callback function
 * @param: argument - Extra argument
 * @return: Timer handle
 */
typedef ezOsal_TimerHandle_t (*ezOsal_pfTimerCreate)(const char* timer_name,
    uint32_t period_ticks,
    ezOsal_fpTimerElapseCallback timer_callback,
    void *argument);

/**@brief: Timer delete function
 * @param: timer_handle - Timer handle
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_pfTimerDelete)(ezOsal_TimerHandle_t timer_handle);

/**@brief: Timer start function
 * @param: timer_handle - Timer handle
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_pfTimerStart)(ezOsal_TimerHandle_t timer_handle);

/**@brief: Timer stop function
 * @param: timer_handle - Timer handle
 * @return: Status
 */
typedef ezSTATUS (*ezOsal_pfTimerStop)(ezOsal_TimerHandle_t timer_handle);

/** @brief List of interface functions must be implemented
 */
typedef struct
{
    /* Initialization function */
    ezOsal_pfInit           Init;

    /* Task functions */
    ezOsal_fpTaskCreate     TaskCreate;
    ezOsal_fpTaskDelete     TaskDelete;
    ezOsal_fpTaskResume     TaskResume;
    ezOsal_fpTaskSuspend    TaskSuspend;
    ezOsal_fpTaskDelay      TaskDelay;
    ezOsal_fpTaskGetTickCount TaskGetTickCount;
    ezOsal_fpTaskStartScheduler TaskStartScheduler;

    /* Semaphore functions */
    ezOsal_fpSemaphoreCreate SemaphoreCreate;
    ezOsal_fpSemaphoreDelete SemaphoreDelete;
    ezOsal_fpSemaphoreTake   SemaphoreTake;
    ezOsal_fpSemaphoreGive   SemaphoreGive;

    /* Timer functions */
    ezOsal_pfTimerCreate    TimerCreate;
    ezOsal_pfTimerDelete    TimerDelete;
    ezOsal_pfTimerStart     TimerStart;
    ezOsal_pfTimerStop      TimerStop;
}ezOsal_Interfaces_t;


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

ezSTATUS ezOsal_Init(void);
ezSTATUS ezOsal_SetInterface(const ezOsal_Interfaces_t *interface);

ezOsal_TaskHandle_t ezOsal_TaskCreate(ezOsal_TaskConfig_t* config);
ezSTATUS ezOsal_TaskDelete(ezOsal_TaskHandle_t task_handle);
ezSTATUS ezOsal_TaskSuspend(ezOsal_TaskHandle_t task_handle);
ezSTATUS ezOsal_TaskResume(ezOsal_TaskHandle_t task_handle);
ezSTATUS ezOsal_TaskDelay(unsigned long num_of_ticks);
unsigned long ezOsal_TaskGetTickCount(void);
void ezOsal_TaskStartScheduler(void);

ezOsal_SemaphoreHandle_t ezOsal_SemaphoreCreate(ezOsal_SemaphoreConfig_t *config);
ezSTATUS ezOsal_SemaphoreDelete(ezOsal_SemaphoreHandle_t semaphore_handle);
ezSTATUS ezOsal_SemaphoreTake(ezOsal_SemaphoreHandle_t semaphore_handle, uint32_t timeout_ticks);
ezSTATUS ezOsal_SemaphoreGive(ezOsal_SemaphoreHandle_t semaphore_handle);

ezOsal_TimerHandle_t ezOsal_TimerCreate(const char* timer_name,
    uint32_t period_ticks,
    ezOsal_fpTimerElapseCallback timer_callback,
    void *argument);
ezSTATUS ezOsal_TimerDelete(ezOsal_TimerHandle_t timer_handle);
ezSTATUS ezOsal_TimerStart(ezOsal_TimerHandle_t timer_handle);
ezSTATUS ezOsal_TimerStop(ezOsal_TimerHandle_t timer_handle);

#endif /* EZ_OSAL == 1 */

#ifdef __cplusplus
}
#endif

#endif /* _EZ_OSAL_H */

/* End of file */
