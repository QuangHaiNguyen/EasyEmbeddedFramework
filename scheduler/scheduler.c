/*******************************************************************************
* Title                 :   scheduler 
* Filename              :   scheduler.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.05.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 24.02.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  scheduler.c
 *  @brief This is the source code of the scheduler module
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "scheduler.h"
#include "../ezmDebug/ezmDebug.h"
#include "../app/app_config.h"

#if (MODULE_DEBUG == 1U) && (SCHEDULER_DBG == 1U)
    #define SCHEDULERPRINT1(a)              PRINT_DEBUG1(a)               
    #define SCHEDULERPRINT2(a,b)            PRINT_DEBUG2(a,b)             
    #define SCHEDULERPRINT3(a,b,c)          PRINT_DEBUG3(a,b,c)
#else 
    #define SCHEDULERPRINT1(a)           
    #define SCHEDULERPRINT2(a,b)           
    #define SCHEDULERPRINT3(a,b,c)
#endif

#if (NUM_OF_TASK > 256)
#error does not support more than 256 task 
#endif

#define DEFAULT_TASK_ID     0U
#define UNINIT_TASK_ID      0xFF

#if (TASK_DIAGNOSE == 1U)
    #if((MODULE_DEBUG == 0U) || (SCHEDULER_DBG == 0U))
        #warning Turn debug on the see task diagnose
    #endif
#endif

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of scheduler type
 *  
 */
typedef struct 
{
    uint32_t (*fpSysTickInMillis)(void);
    /**< pointer to the tick function*/

    uint8_t u8NumOfActiveTask;
    /**< number of active task*/

    ezmScheduler_Task astTasks[NUM_OF_TASK];
    /**< array of all tasks*/
}ezmScheduler_Scheduler;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static ezmScheduler_Scheduler stScheduler;

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezmScheduler_DefaulTask(void);  /* Default task, it can be overwriten */

/******************************************************************************
* Function : ezmScheduler_Init
*//** 
* \b Description:
*
* This function initializes the scheduler, it also adds the default task into the list with id 0
* and interval 100ms
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    pfSysTickInMillis: (IN)pointer to system tick funtion, in milliseconds
* @return   True if success
*
* \b Example Example:
* @code
* (void) ezmScheduler_Init(&SystemTickFunctionInMillis());
* @endcode
*
* @see ezmScheduler_Init
*
*******************************************************************************/
bool ezmScheduler_Init(uint32_t (*pfSysTickInMillis)(void))
{
    bool bSuccess = false;

    if(pfSysTickInMillis)
    {
        stScheduler.fpSysTickInMillis = pfSysTickInMillis;
        stScheduler.u8NumOfActiveTask = 1; /*defaul task*/
        
        /** Init default task,
         * Default task will always have id 0
         */
        stScheduler.astTasks[DEFAULT_TASK_ID].u8TaskId = DEFAULT_TASK_ID;
        stScheduler.astTasks[DEFAULT_TASK_ID].fpApplication = &ezmScheduler_DefaulTask;
        stScheduler.astTasks[DEFAULT_TASK_ID].u32IntervalMillis = 100U;
        stScheduler.astTasks[DEFAULT_TASK_ID].u32LastTimeMillis = 0U;
        
        /**Init task to default state
         * id 0xFF means task is not initlialized 
         */
        for(uint8_t i = DEFAULT_TASK_ID + 1; i < NUM_OF_TASK; i++)
        {
            stScheduler.astTasks[i].u8TaskId = UNINIT_TASK_ID;
            stScheduler.astTasks[i].fpApplication = NULL;
            stScheduler.astTasks[i].u32IntervalMillis = 0U;
            stScheduler.astTasks[i].u32LastTimeMillis = 0U;
        }

        bSuccess = true;
    }

    return bSuccess;
}

/******************************************************************************
* Function : ezmScheduler_Init
*//** 
* \b Description:
*
* This function initializes the scheduler, it also adds the default task into the list with id 0
* and interval 100ms
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    pfSysTickInMillis: (IN)pointer to system tick funtion, in milliseconds
* @return   True if success
*
* \b Example Example:
* @code
* (void) ezmScheduler_Init(&SystemTickFunctionInMillis());
* @endcode
*
* @see ezmScheduler_Init
*
*******************************************************************************/
void ezmScheduler_Run(void)
{
    uint32_t u32CurrentTickMillis;

    u32CurrentTickMillis = stScheduler.fpSysTickInMillis();
    
    for(uint8_t i = 0; i < NUM_OF_TASK; i++)
    {
        if(stScheduler.astTasks[i].u8TaskId != UNINIT_TASK_ID)
        {
            if((u32CurrentTickMillis - stScheduler.astTasks[i].u32LastTimeMillis) >= stScheduler.astTasks[i].u32IntervalMillis)
            {
                stScheduler.astTasks[i].u32LastTimeMillis = u32CurrentTickMillis;
                stScheduler.astTasks[i].fpApplication();

#if (TASK_DIAGNOSE == 1)
                SCHEDULERPRINT2("Running task id: %d", i);
                uint32_t u32TaskFinishedTickMillis = stScheduler.fpSysTickInMillis();
                SCHEDULERPRINT2("Start: %d ms", stScheduler.astTasks[i].u32LastTimeMillis);
                SCHEDULERPRINT2("Stop: %d ms", u32TaskFinishedTickMillis);
                SCHEDULERPRINT2("Execution time: %d ms", u32TaskFinishedTickMillis - u32CurrentTickMillis);
#endif
            }
        }
    }
}

/******************************************************************************
* Function : ezmScheduler_Init
*//** 
* \b Description:
*
* This function initializes the scheduler, it also adds the default task into the list with id 0
* and interval 100ms
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    pfSysTickInMillis: (IN)pointer to system tick funtion, in milliseconds
* @return   True if success
*
* \b Example Example:
* @code
* (void) ezmScheduler_Init(&SystemTickFunctionInMillis());
* @endcode
*
* @see ezmScheduler_Init
*
*******************************************************************************/
uint8_t ezmScheduler_RegisterTask (void (*fpNewTask)(void), uint32_t u32NewIntervalMillis)
{
    uint8_t u8TaskId = UNINIT_TASK_ID; /* 0xFF: uninitialized Id*/

    for(uint8_t i = 0; i < NUM_OF_TASK; i++)
    {
        if(stScheduler.astTasks[i].u8TaskId == UNINIT_TASK_ID)
        {
            u8TaskId = i; /* Task id equal to task index*/
            stScheduler.astTasks[i].u8TaskId = u8TaskId;
            stScheduler.astTasks[i].fpApplication = fpNewTask;
            stScheduler.astTasks[i].u32IntervalMillis = u32NewIntervalMillis;
            stScheduler.astTasks[i].u32LastTimeMillis = stScheduler.fpSysTickInMillis();
            stScheduler.u8NumOfActiveTask++;
            SCHEDULERPRINT2("Add task with ID %d to the scheduler", u8TaskId);
            break;
        }
    }

    return u8TaskId;
}

/******************************************************************************
* Function : ezmScheduler_GetTask
*//** 
* \b Description:
*
* This function return the pointer to a task with the specific id number
*
* PRE-CONDITION: scheduler must be initialized
*
* POST-CONDITION: None
* 
* @param    u8TaskId: (IN)Task id of the task we want to get
* @return   pointer to the task or NULL if task does not exist
*
* \b Example Example:
* @code
* ezmScheduler_Task * pstTask;
* pstTask = ezmScheduler_GetTask(0x00U); //get the default task
* @endcode
*
* @see ezmScheduler_Init
*
*******************************************************************************/
ezmScheduler_Task * ezmScheduler_GetTask (uint8_t u8TaskId)
{
    ezmScheduler_Task * pstTask = NULL;

    for(uint8_t i = 0; i < NUM_OF_TASK; i++)
    {
        if(stScheduler.astTasks[i].u8TaskId == u8TaskId)
        {
            pstTask = &stScheduler.astTasks[i];
            break;
        }
    }

    return pstTask;
}

/******************************************************************************
* Function : ezmScheduler_RemoveTask
*//** 
* \b Description:
*
* This function removes a task with a specific id
*
* PRE-CONDITION: scheduler must be initialized
*
* POST-CONDITION: None
* 
* @param    u8TaskId: (IN)Id of the task to be removed
* @return   None
*
* \b Example Example:
* @code
* ezmScheduler_RemoveTask(0x01); //remove task with id 0x01
* @endcode
*
* @see ezmScheduler_Init
*
*******************************************************************************/
void ezmScheduler_RemoveTask (uint8_t u8TaskId)
{
    for(uint8_t i = 0; i < NUM_OF_TASK; i++)
    {
        if(stScheduler.astTasks[i].u8TaskId == u8TaskId)
        {
            stScheduler.astTasks[i].fpApplication = NULL;
            stScheduler.astTasks[i].u32IntervalMillis = 0U;
            stScheduler.astTasks[i].u32LastTimeMillis = 0U;
            stScheduler.astTasks[i].u8TaskId = UNINIT_TASK_ID;
            
            stScheduler.u8NumOfActiveTask--;
            SCHEDULERPRINT2("remove task with ID %d from the scheduler", u8TaskId);
            break;
        }
    }
}

/******************************************************************************
* Function : ezmScheduler_OverwriteDefaulTask
*//** 
* \b Description:
*
* This function allows the user to overwrite the default task, or the idle task. The idle
* task is called every 100ms and after the initialization, it has no operation.
*
* New task must follow this prototype: void NewDefaultTask(void)
*
* PRE-CONDITION: scheduler must be initialized
*
* POST-CONDITION: None
* 
* @param    fpNewDefaultTask: (IN)pointer to the new default task
* @param    u32NewIntervalMillis: (IN)Interval of the new task
* @return   None
*
* \b Example Example:
* @code
* ezmScheduler_OverwriteDefaulTask(&NewDefaultTask, 200);
* @endcode
*
* @see ezmScheduler_Init
*
*******************************************************************************/
void ezmScheduler_OverwriteDefaulTask (void (*fpNewDefaultTask)(void), uint32_t u32NewIntervalMillis)
{
    stScheduler.astTasks[DEFAULT_TASK_ID].fpApplication = fpNewDefaultTask;
    stScheduler.astTasks[DEFAULT_TASK_ID].u32IntervalMillis = u32NewIntervalMillis;
    stScheduler.astTasks[DEFAULT_TASK_ID].u32LastTimeMillis = stScheduler.fpSysTickInMillis();

    SCHEDULERPRINT1("Overwrite default task");
}

/******************************************************************************
* Function : ezmScheduler_GetCurrentTickMillis
*//** 
* \b Description:
*
* This function returns the current tick in milliseconds
*
* PRE-CONDITION: scheduler must be initialized
*
* POST-CONDITION: None
* 
* @param    None
* @return   Number os milliseconds since the system has started
*
* \b Example Example:
* @code
* uint32_t u32CurrentTick = ezmScheduler_GetCurrentTickMillis();
* @endcode
*
* @see ezmScheduler_Init
*
*******************************************************************************/
uint32_t ezmScheduler_GetCurrentTickMillis(void)
{
    return stScheduler.fpSysTickInMillis();
}

/******************************************************************************
* Function : ezmScheduler_GetNumOfActiveTasks
*//** 
* \b Description:
*
* This function returns of number of tasks running
*
* PRE-CONDITION: scheduler must be initialized
*
* POST-CONDITION: None
* 
* @param    None
* @return   Number of running tasks
*
* \b Example Example:
* @code
* uint8_t u8NumberOfTasks = ezmScheduler_GetNumOfActiveTasks();
* @endcode
*
* @see ezmScheduler_Init
*
*******************************************************************************/
uint8_t ezmScheduler_GetNumOfActiveTasks(void)
{
    return stScheduler.u8NumOfActiveTask;
}

/******************************************************************************
* Function : ezmScheduler_DefaulTask
*//** 
* \b Description:
*
* This function is the default task. After initialization, this function does nothing.
* If debug out is turned on, it prints "Running default task"
*
* PRE-CONDITION: scheduler must be initialized
*
* POST-CONDITION: None
* 
* @param    None
* @return   None
*
* @see ezmScheduler_Init
*
*******************************************************************************/
static void ezmScheduler_DefaulTask(void)
{
    SCHEDULERPRINT1("Running default task");
}
/* End of file*/