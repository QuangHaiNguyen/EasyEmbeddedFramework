/*******************************************************************************
* Title                 :   Scheduler module
* Filename              :   scheduler.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.05.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  24.05.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   scheduler.h
 *  @brief  Header for the Scheduler module
 */


#ifndef _SCHEDULER_H
#define _SCHEDULER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define NUM_OF_TASK     5U
/**< number of supported task, maximum is 255 tasks*/

#define TASK_DIAGNOSE   0U
/**< turn on task timing debug out*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of task type
 *  
 */
typedef struct
{
    uint8_t u8TaskId;
    /**< ID of the task*/

    uint32_t u32IntervalMillis;
    /**< interval, which the task will be called, in milisecond*/
    
    uint32_t u32LastTimeMillis;
    /**< last time when task is called, in milisecond*/
    
    void (*fpApplication)(void);
    /**< pointer to the application code*/
}ezmScheduler_Task;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool ezmScheduler_Init (uint32_t (*pfSysTickInMillis)(void));                                           /* Initialize scheduler*/ 
void ezmScheduler_OverwriteDefaulTask (void (*fpNewDefaultTask)(void), uint32_t u32NewIntervalMillis);  /* Overwrite the default task with a different task*/
uint8_t ezmScheduler_RegisterTask (void (*fpNewTask)(void), uint32_t u32NewIntervalMillis);             /* Register a new task*/
ezmScheduler_Task * ezmScheduler_GetTask (uint8_t u8TaskId);                                            /* Get the info of a task */
void ezmScheduler_RemoveTask (uint8_t u8TaskId);                                                        /* Remove an existing task*/
void ezmScheduler_Run (void);                                                                           /* Run the scheduler */
uint32_t ezmScheduler_GetCurrentTickMillis(void);                                                       /* Get the current tick in milliseconds*/
uint8_t ezmScheduler_GetNumOfActiveTasks(void);                                                         /* Get the number of tasks running*/

#endif /* _SCHEDULER_H */

/* End of file*/
