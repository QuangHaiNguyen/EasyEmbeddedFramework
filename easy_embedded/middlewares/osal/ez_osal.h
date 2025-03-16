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
        #if (((EZ_OSAL_USE_STATIC) & (configSUPPORT_STATIC_ALLOCATION)) == 0)
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


typedef void (*ezOsal_fpTaskFunction)(void *argument);
typedef void (*ezOsal_fpTimerElapseCallback)(void *argument);


/**@brief OSAL task configuration structure
 */
typedef struct
{
    const char* task_name;  /**< Task name */
    uint32_t priority;      /**< Priority */
    void *argument;         /**< Extra argument */
    const uint8_t *stack;   /**< Buffer holding stack in case using static allocation */
    uint32_t stack_size;    /**< Stack size */
    ezOsal_fpTaskFunction task_function; /**< Task function */
#if ((EZ_FREERTOS_PORT == 1) && (EZ_OSAL_USE_STATIC == 1))
    StaticTask_t task_block; /**< Static task block */
#endif
}ezOsal_TaskConfig_t;

/* Interfaces ***************************************************************/
typedef ezSTATUS (*ezOsal_pfInit)(void);

typedef ezOsal_TaskHandle_t (*ezOsal_fpTaskCreate)  (ezOsal_TaskConfig_t* config);
typedef ezSTATUS (*ezOsal_fpTaskDelete)     (ezOsal_TaskHandle_t task_handle);
typedef ezSTATUS (*ezOsal_fpTaskSuspend)    (ezOsal_TaskHandle_t task_handle);
typedef ezSTATUS (*ezOsal_fpTaskResume)     (ezOsal_TaskHandle_t task_handle);
typedef ezSTATUS (*ezOsal_fpTaskDelay)      (unsigned long num_of_ticks);
typedef unsigned long (*ezOsal_fpTaskGetTickCount)(void);

typedef ezOsal_SemaphoreHandle_t (*ezOsal_fpSemaphoreCreate)(uint32_t max_count);
typedef ezSTATUS (*ezOsal_fpSemaphoreDelete)(ezOsal_SemaphoreHandle_t semaphore_handle);
typedef ezSTATUS (*ezOsal_fpSemaphoreTake)(ezOsal_SemaphoreHandle_t semaphore_handle, uint32_t timeout);
typedef ezSTATUS (*ezOsal_fpSemaphoreGive)(ezOsal_SemaphoreHandle_t semaphore_handle);

typedef ezOsal_TimerHandle_t (*ezOsal_pfTimerCreate)(const char* timer_name, uint32_t period_ms, ezOsal_fpTimerElapseCallback timer_callback, void *argument);
typedef ezSTATUS (*ezOsal_pfTimerDelete)(ezOsal_TimerHandle_t timer_handle);
typedef ezSTATUS (*ezOsal_pfTimerStart)(ezOsal_TimerHandle_t timer_handle);
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

ezOsal_SemaphoreHandle_t ezOsal_SemaphoreCreate(uint32_t max_count);
ezSTATUS ezOsal_SemaphoreDelete(ezOsal_SemaphoreHandle_t semaphore_handle);
ezSTATUS ezOsal_SemaphoreTake(ezOsal_SemaphoreHandle_t semaphore_handle, uint32_t timeout_ms);
ezSTATUS ezOsal_SemaphoreGive(ezOsal_SemaphoreHandle_t semaphore_handle);

ezOsal_TimerHandle_t ezOsal_TimerCreate(const char* timer_name,
    uint32_t period_ms,
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
