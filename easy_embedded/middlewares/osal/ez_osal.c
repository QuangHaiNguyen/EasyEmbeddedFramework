/*****************************************************************************
* Filename:         ez_osal.c
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

/** @file   ez_osal.c
 *  @author Hai Nguyen
 *  @date   15.03.2025
 *  @brief  Common interface for OS abstraction layer
 *
 *  @details None
 */

/*****************************************************************************
* Includes
*****************************************************************************/
#include "ez_osal.h"

#if (EZ_OSAL == 1)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ez_osal"       /**< module name */
#include "ez_logging.h"

/*the rest of include go here*/

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define IS_INTERFACE_IMPLEMENTED(interface_p, function_p) ((interface_p != NULL) && (interface_p->function_p != NULL))

/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
static const ezOsal_Interfaces_t *osal_interface = NULL;

/*****************************************************************************
* Function Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Public functions
*****************************************************************************/

/*****************************************************************************
* Function: ezOsal_Init
*//** 
* @brief Initialize the OS
*
* @details  None
*
* @return   ezSTATUS
*
* @pre ezOsal_SetInterface must be called before calling this function
* @post None
*
* \b Example
* @code
* ezSTATUS status = ezOsal_Init();
* @endcode
*
* @see ezOsal_SetInterface
*
*****************************************************************************/
ezSTATUS ezOsal_Init(void)
{
    EZTRACE("ezOsal_Init()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, Init))
    {
        return osal_interface->Init();
    }
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_SetInterface
*//** 
* @brief Set the implemented OS interfaces
*
* @details Must be called first before calling any other OS functions
*
* @param[in]    interface: pointer to the implemented OS interfaces
* @return       ezSTATUS
*
* @pre None
* @post None
*
* \b Example
* @code
* ezSTATUS status = ezOsal_SetInterface(&implemented_interface);
* @endcode
*
*****************************************************************************/
ezSTATUS ezOsal_SetInterface(const ezOsal_Interfaces_t *interface)
{
    EZTRACE("ezOsal_SetInterface()");
    if(interface != NULL)
    {
        osal_interface = interface;
        return ezSUCCESS;
    }
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_TaskCreate
*//** 
* @brief Create OS task
*
* @details None
*
* @param[in]    config: task configuration
* @return       ezOsal_TaskHandle_t or NULL if failed
*
* @pre OS interface must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_TaskHandle_t task = ezOsal_TaskCreate("task1", 1024, 1, task_function, NULL);
* @endcode
*
*****************************************************************************/
ezOsal_TaskHandle_t ezOsal_TaskCreate(ezOsal_TaskConfig_t* config)
{
    EZTRACE("ezOsal_TaskCreate()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TaskCreate))
    {
        return osal_interface->TaskCreate(config);
    }
    EZWARNING("Interface is not implemented");
    return NULL;
}


/*****************************************************************************
* Function: ezOsal_TaskDelete
*//** 
* @brief Delete OS task
*
* @details None
*
* @param[in]    task_handle: task handle
* @return       ezSUCCESS or ezFAIL
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_TaskHandle_t task = ezOsal_TaskCreate("task1", 1024, 1, task_function, NULL);
* ezSTATUS status = ezOsal_TaskDelete(task);
* @endcode
*
* @see ezOsal_TaskCreate
*
*****************************************************************************/
ezSTATUS ezOsal_TaskDelete(ezOsal_TaskHandle_t task_handle)
{
    EZTRACE("ezOsal_TaskDelete()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TaskDelete))
    {
        return osal_interface->TaskDelete(task_handle);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_TaskSuspend
*//**
* @brief Suspend OS task
*
* @details None
*
* @param[in]    task_handle: task handle
* @return       ezSUCCESS or ezFAIL
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_TaskHandle_t task = ezOsal_TaskCreate("task1", 1024, 1, task_function, NULL);
* ezSTATUS status = ezOsal_TaskSuspend(task);
* @endcode
*
* @see ezOsal_TaskCreate
*
*****************************************************************************/
ezSTATUS ezOsal_TaskSuspend(ezOsal_TaskHandle_t task_handle)
{
    EZTRACE("ezOsal_TaskSuspend()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TaskSuspend))
    {
        return osal_interface->TaskSuspend(task_handle);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_TaskResume
*//** 
* @brief Resume OS task
*
* @details None
*
* @param[in]    task_handle: task handle
* @return       ezSUCCESS or ezFAIL
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_TaskHandle_t task = ezOsal_TaskCreate("task1", 1024, 1, task_function, NULL);
* ezSTATUS status = ezOsal_TaskResume(task);
* @endcode
*
* @see ezOsal_TaskCreate
*
*****************************************************************************/
ezSTATUS ezOsal_TaskResume(ezOsal_TaskHandle_t task_handle)
{
    EZTRACE("ezOsal_TaskResume()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TaskResume))
    {
        return osal_interface->TaskResume(task_handle);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}

/*****************************************************************************
* Function: ezOsal_TaskDelay
*//** 
* @brief    Delay OS task for cerntain number of ticks.
*
* @details  Relative delay in ticks, user must implement their own DelayUntil()
*           function
*
* @param[in]    num_of_ticks: number of delay ticks
* @return       ezSUCCESS or ezFAIL
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezOsal_TaskCreate
*
*****************************************************************************/
ezSTATUS ezOsal_TaskDelay(unsigned long num_of_ticks)
{
    EZTRACE("ezOsal_TaskDelay()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TaskDelay))
    {
        return osal_interface->TaskDelay(num_of_ticks);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_TaskGetTickCount
*//** 
* @brief    Get current tick counts
*
* @details  None
*
* @return   Number of ticks
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* @endcode
*
* @see ezOsal_TaskCreate
*
*****************************************************************************/
unsigned long ezOsal_TaskGetTickCount(void)
{
    EZTRACE("ezOsal_TaskGetTickCount()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TaskGetTickCount))
    {
        return osal_interface->TaskGetTickCount();
    }
    EZWARNING("Interface is not implemented");
    return 0;
}

/*****************************************************************************
* Function: ezOsal_TaskStartScheduler
*//**
* @brief Start OS task scheduler
*
* @details None
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_TaskStartScheduler();
* @endcode
*
*****************************************************************************/
void ezOsal_TaskStartScheduler(void)
{
    EZTRACE("ezOsal_TaskStartScheduler()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TaskStartScheduler))
    {
        osal_interface->TaskStartScheduler();
    }
    EZWARNING("Interface is not implemented");
}

/*****************************************************************************
* Function: ezOsal_SemaphoreCreate
*//** 
* @brief Create a semaphore
*
* @details None
*
* @param[in]    config: semaphore configuration
* @return   ezOsal_SemaphoreHandle_t or NULL if failed
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_SemaphoreHandle_t sem = ezOsal_SemaphoreCreate(2);
* @endcode
*
*****************************************************************************/
ezOsal_SemaphoreHandle_t ezOsal_SemaphoreCreate(ezOsal_SemaphoreConfig_t *config)
{
    EZTRACE("ezOsal_SemaphoreCreate()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, SemaphoreCreate))
    {
        return osal_interface->SemaphoreCreate(config);
    }
    EZWARNING("Interface is not implemented");
    return NULL;
}


/*****************************************************************************
* Function: ezOsal_SemaphoreDelete
*//** 
* @brief Delete a semaphore
*
* @details None
*
* @param[in]    semaphore_handle: (IN)semaphore handle
* @return       ezSTATUS
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_SemaphoreHandle_t sem = ezOsal_SemaphoreCreate(2);
* ezSTATUS status = ezOsal_SemaphoreDelete(sem);
* @endcode
*
* @see ezOsal_SemaphoreCreate
*
*****************************************************************************/
ezSTATUS ezOsal_SemaphoreDelete(ezOsal_SemaphoreHandle_t semaphore_handle)
{
    EZTRACE("ezOsal_SemaphoreDelete()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, SemaphoreDelete))
    {
        return osal_interface->SemaphoreDelete(semaphore_handle);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_SemaphoreTake
*//** 
* @brief Take semaphore
*
* @details None
*
* @param[in]    semaphore_handle: semaphore handle
* @param[in]    timeout_ms: time to wait for the semaphore in millisecond
* @return       ezSTATUS
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_SemaphoreHandle_t sem = ezOsal_SemaphoreCreate(2);
* ezSTATUS status = ezOsal_SemaezOsal_SemaphoreTakephoreDelete(sem, 100);
* @endcode
*
* @see ezOsal_SemaphoreCreate
*
*****************************************************************************/
ezSTATUS ezOsal_SemaphoreTake(ezOsal_SemaphoreHandle_t semaphore_handle, uint32_t timeout_ticks)
{
    EZTRACE("ezOsal_SemaphoreTake(timeout_ticks = %d)", timeout_ticks);
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, SemaphoreTake))
    {
        return osal_interface->SemaphoreTake(semaphore_handle, timeout_ticks);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_SemaphoreGive
*//** 
* @brief Give semaphore
*
* @details None
*
* @param[in]    semaphore_handle: semaphore handle
* @return       ezSTATUS
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_SemaphoreHandle_t sem = ezOsal_SemaphoreCreate(2);
* ezSTATUS status = ezOsal_SemaphoreGive(sem);
* @endcode
*
* @see ezOsal_SemaphoreCreate
*
*****************************************************************************/
ezSTATUS ezOsal_SemaphoreGive(ezOsal_SemaphoreHandle_t semaphore_handle)
{
    EZTRACE("ezOsal_SemaphoreGive()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, SemaphoreGive))
    {
        return osal_interface->SemaphoreGive(semaphore_handle);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_TimerCreate
*//** 
* @brief Create a timer
*
* @details None
*
* @param[in]    timer_name: timer's name
* @param[in]    period_ms:  period in millisecond
* @param[in]    timer_callback: callback function
* @param[in]    argument: extra argument
* @return       ezOsal_TimerHandle_t or NULL if failed
*
* @pre OS interfaces must be implemented
* @post None
*
* \b Example
* @code
* ezOsal_TimerHandle_t imter = ezOsal_TimerCreate("timer1", 1000, timer_callback, NULL);
* @endcode
*
*****************************************************************************/
ezOsal_TimerHandle_t ezOsal_TimerCreate(const char* timer_name, uint32_t period_ticks, ezOsal_fpTimerElapseCallback timer_callback, void *argument)
{
    EZTRACE("ezOsal_TimerCreate(name = %s, period_ticks = %d)", timer_name, period_ticks);
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TimerCreate))
    {
        return osal_interface->TimerCreate(timer_name, period_ticks, timer_callback, argument);
    }
    EZWARNING("Interface is not implemented");
    return NULL;
}


/*****************************************************************************
* Function: ezOsal_TimerDelete
*//** 
* @brief Delete a timer
*
* @details None
*
* @param    timer_handle: timer handle
* @return   ezSTATUS
*
* @pre None
* @post None
*
* \b Example
* @code
* ezOsal_TimerHandle_t imter = ezOsal_TimerCreate("timer1", 1000, timer_callback, NULL);
* ezSTATUS status = ezOsal_TimerDelete(timer);
* @endcode
*
* @see ezOsal_TimerCreate
*
*****************************************************************************/
ezSTATUS ezOsal_TimerDelete(ezOsal_TimerHandle_t timer_handle)
{
    EZTRACE("ezOsal_TimerDelete()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TimerDelete))
    {
        return osal_interface->TimerDelete(timer_handle);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_TimerStart
*//** 
* @brief Start a timer
*
* @details None
*
* @param    timer_handle: timer handle
* @return   ezSTATUS
*
* @pre None
* @post None
*
* \b Example
* @code
* ezOsal_TimerHandle_t imter = ezOsal_TimerCreate("timer1", 1000, timer_callback, NULL);
* ezSTATUS status = ezOsal_TimerStart(timer);
* @endcode
*
* @see ezOsal_TimerCreate
*
*****************************************************************************/
ezSTATUS ezOsal_TimerStart(ezOsal_TimerHandle_t timer_handle)
{
    EZTRACE("ezOsal_TimerStart()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TimerStart))
    {
        return osal_interface->TimerStart(timer_handle);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}


/*****************************************************************************
* Function: ezOsal_TimerStop
*//** 
* @brief Stop a timer
*
* @details None
*
* @param    timer_handle: timer handle
* @return   ezSTATUS
*
* @pre None
* @post None
*
* \b Example
* @code
* ezOsal_TimerHandle_t imter = ezOsal_TimerCreate("timer1", 1000, timer_callback, NULL);
* ezSTATUS status = ezOsal_TimerStop(timer);
* @endcode
*
* @see ezOsal_TimerCreate
*
*****************************************************************************/
ezSTATUS ezOsal_TimerStop(ezOsal_TimerHandle_t timer_handle)
{
    EZTRACE("ezOsal_TimerStop()");
    if(IS_INTERFACE_IMPLEMENTED(osal_interface, TimerStop))
    {
        return osal_interface->TimerStop(timer_handle);
    }
    EZWARNING("Interface is not implemented");
    return ezFAIL;
}

/*****************************************************************************
* Local functions
*****************************************************************************/

#endif /* EZ_OSAL == 1 */
/* End of file*/
