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
#include "app/app_config.h"

#if (KERNEL == 1U)
#include "helper/linked_list/linked_list.h"
#include "stdint.h"
#include "stdbool.h"

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
void    ezmKernel_UpdateClock(void);
void    ezmKernel_Run(void);
uint8_t ezmKernel_GetLoad(void);

#endif /* CLI */
#endif /* _EZM_KERNEL_H */

/* End of file*/
