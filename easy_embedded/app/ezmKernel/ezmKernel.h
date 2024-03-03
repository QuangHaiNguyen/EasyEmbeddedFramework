/*******************************************************************************
* Title                 :   module
* Filename              :   module.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
*******************************************************************************/

/** @file   module.h
 *  @brief  Header template for a module
 */


#ifndef _EZM_KERNEL_H
#define _EZM_KERNEL_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezApp/ezSdk_config.h"

#if (CONFIG_KERNEL == 1U)
#include "utilities/linked_list/linked_list.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef uint8_t(*process_handler)(void);
typedef struct Process EzmProcess;

typedef enum
{
    PROC_ONCE,      /* executed only once */
    PROC_REPEATED,  /* executed repeatedly, based on period value */
}PROCESS_TYPE;

/*
 * @brief
 */
struct Process
{
    struct Node     node;                   /**< node of the link list*/
    PROCESS_TYPE    proc_type;              /**< type of the process, see PROCESS_TYPE*/
    uint32_t        period;                 /**< execution cycle*/
    int32_t         exec_cnt_down;          /**< count down until execution */
    process_handler handler;                /**< the function will be execited */
};
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool    ezmKernel_AddProcess(EzmProcess* proc, PROCESS_TYPE proc_type, uint32_t period_ms, process_handler handler);
bool    ezmKernel_RemoveProcess(EzmProcess* proc);
void    ezmKernel_UpdateClock(void);
void    ezmKernel_Run(void);
uint8_t ezmKernel_GetLoad(void);
int     ezmKernel_GetTickMillis(void);
int     ezmKernel_GetNumOfActiveProc(void);
#endif /* CLI */
#endif /* _EZM_KERNEL_H */

/* End of file*/
