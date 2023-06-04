
/*******************************************************************************
* Filename:         ezKernel.h
* Author:           Hai Nguyen
* Original Date:    13.09.2022
* Last Update:      13.09.2022
*
* -----------------------------------------------------------------------------
* Company:          Embedded Easy
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Embedded Easy
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 13.09.2022
*
*******************************************************************************/

/** @file   ezKernel.h
 *  @author Hai Nguyen
 *  @date   13.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _EZKERNEL_H
#define _EZKERNEL_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezApp/ezSdk_config.h"

#if(EZ_KERNEL == 1U)
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/


/** @brief definition of the task function pointer
 *
 */
typedef KERNEL_TASK_STATUS(*ezKernelTaskFunction)(
    void * task_data,
    uint32_t task_data_size);

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
* Function : ezKernel_Initialization
*//**
* @Description: Initialized the kernel module
*
* @param    None
* @return   None
*
*******************************************************************************/
void ezKernel_Initialization(void);


/******************************************************************************
* Function : ezKernel_UpdateTickMillis
*//**
* @Description: update the kernel tick in milli
*
* @param    None
* @return   None
*
*******************************************************************************/
void ezKernel_UpdateTickMillis(void);


/******************************************************************************
* Function : ezKernel_AddTask
*//**
* @Description:
*
* @param    function: function to be executed
* @param    delay_millis: how many millis until the function will be executed
* @param    task_data: data will be processed by the task, can be null
* @param    task_data_size: size of the data, can be zero
*
* @return   ezSUCCESS or ez FAIL
*
*******************************************************************************/
ezSTATUS ezKernel_AddTask(ezKernelTaskFunction function,
    int32_t delay_millis,
    void *task_data,
    uint32_t task_data_size);


/******************************************************************************
* Function : ezKernel_Run
*//**
* @Description: Run the kernel, must be called in the tick function
*
* @param    None
* @return   None
*
*******************************************************************************/
void ezKernel_Run(void);


/******************************************************************************
* Function : ezKernel_GetTickMillis
*//**
* @Description: return the current tick of the kernel, in millisecond
*
* @param    None
* @return   current tick
*
*******************************************************************************/
uint32_t ezKernel_GetTickMillis(void);


/******************************************************************************
* Function : ezKernel_GetNumOfTasks
*//**
* @Description: return number of tasks
*
* @param    None
* @return   number of tasks
*
*******************************************************************************/
uint32_t ezKernel_GetNumOfTasks(void);


#endif /* EZ_KERNEL == 1U */
#endif /* _EZKERNEL_H */

/* End of file */

