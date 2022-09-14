
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
#include "app/app_config.h"

#if(CONFIG_KERNEL == 1U)

#include "stdint.h"
#include "stdbool.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/


typedef struct ezKernelTask *ezKernelTask;


/** @brief definition of status of the executed task
 *
 */
typedef enum
{
    TASK_STATUS_OK,
    TASK_STATUS_EXEC_AGAIN,
    TASK_STATUS_ERROR,
}KERNEL_TASK_STATUS;


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
* @Description:
*
* @param    a:
* @return   None
*
*******************************************************************************/
void ezKernel_Initialization(void);


/******************************************************************************
* Function : ezKernel_Initialization
*//**
* @Description:
*
* @param    a:
* @return   None
*
*******************************************************************************/
void ezKernel_UpdateTickMillis(void);


/******************************************************************************
* Function : ezKernel_Initialization
*//**
* @Description:
*
* @param    a:
* @return   None
*
*******************************************************************************/
ezSTATUS ezKernel_AddTask(ezKernelTaskFunction function,
    int32_t delay_millis,
    void *task_data,
    uint32_t task_data_size);


/******************************************************************************
* Function : ezKernel_Run
*//**
* @Description:
*
* @param    a:
* @return   None
*
*******************************************************************************/
void ezKernel_Run(void);


#endif /* CONFIG_KERNEL == 1U */
#endif /* _EZKERNEL_H */

/* End of file */

