/*******************************************************************************
* Title                 :   module
* Filename              :   module.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  21.02.2021   1.0.0       Quang Hai Nguyen    Interface Created.
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
#include "../app/app_config.h"

#if (KERNEL == 1U)
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

typedef enum
{
    PROC_ONCE,
    PROC_REPEATED,
}PROCESS_TYPE;
/*
 * @brief
 */
typedef struct
{
    uint32_t        uuid;                   /**< */
    PROCESS_TYPE    proc_type;              /**< */
    uint32_t        period;                 /**< */
    int32_t         exec_cnt_down;          /**< */
    process_handler handler;                /**< */
}process;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/

void ezmKernel_Initialization(void);
bool ezmKernel_AddProcess(process * proc, PROCESS_TYPE proc_type, uint32_t period_ms, process_handler handler);
void ezmKernel_Clock(void);
void ezmKernel_Run(void);
uint8_t ezmKernel_GetLoad(void);

#endif /* CLI */
#endif /* _EZM_KERNEL_H */

/* End of file*/
