/*****************************************************************************
* Filename:         ez_freertos_port.h
* Author:           Hai Nguyen
* Original Date:    06.04.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_freertos_port.h
 *  @author Hai Nguyen
 *  @date   06.04.2024
 *  @brief  Public API of the threadx porting for the task worker rtos interface
 *
 *  @details
 */

#ifndef _EZ_FREERTOS_PORT_H
#define _EZ_FREERTOS_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#if (EZ_FREERTOS_PORT_ENABLE == 1)

/*****************************************************************************
* Includes
*****************************************************************************/
#include "ez_task_worker.h"

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#ifndef FREERTOS_STACK_SIZE
#define FREERTOS_STACK_SIZE (10 * 512) /**< Default size of the stack in word */
#endif

/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None */

/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function: ezFreeRTOSPort_Init
*//** 
* @brief Initialize the threadx implementation of the task worker rtos interfaces
*
* @details This function link the rtos interfaces to their implementation
*
* @param        None
* @return       true if success, else false
*
* @pre None
* @post None
*
* \b Example
* @code
* ezFreeRTOSPort_Init();
* @endcode
*
* @see
*
*****************************************************************************/
bool ezFreeRTOSPort_Init(void);


/*****************************************************************************
* Function: ezFreeRTOSPort_GetInterface
*//** 
* @brief Get the rtos interface implementation
*
* @details
*
* @param    None
* @return   pointer to the interface if success, else NULL
*
* @pre ezThreadXPort_Init must be called first
* @post None
*
* \b Example
* @code
* struct ezTaskWorkerThreadInterfaces *threadx_interfaces = NULL;
* threadx_interfaces = ezThreadXPort_GetInterface();
* @endcode
*
* @see ezThreadXPort_Init
*
*****************************************************************************/
struct ezTaskWorkerThreadInterfaces * ezFreeRTOSPort_GetInterface(void);

#ifdef __cplusplus
}
#endif

#endif /* EZ_FREERTOS_PORT_ENABLE == 1 */
#endif /* _EZ_FREERTOS_PORT_H */


/* End of file */
