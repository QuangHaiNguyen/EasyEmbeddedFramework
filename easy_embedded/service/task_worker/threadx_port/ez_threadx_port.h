/*****************************************************************************
* Filename:         ez_threadx_port.h
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

/** @file   ez_threadx_port.h
 *  @author Hai Nguyen
 *  @date   06.04.2024
 *  @brief  Public API of the threadx porting for the task worker rtos interface
 *
 *  @details
 */

#ifndef _EZ_THREADX_PORT_H
#define _EZ_THREADX_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_THREADX_PORT_ENABLE == 1)
#include "ez_task_worker.h"

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#ifndef THREADX_BYTE_POOL_SIZE
#define THREADX_BYTE_POOL_SIZE 9120 /**< Default size of the byte pool */
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
* Function: ezThreadXPort_Init
*//** 
* @brief Initialize the threadx implementation of the task worker rtos interfaces
*
* @details This function link the rtos interfaces to their implementation
*
* @param[out]   first_unused_memory: (IN)pointer to the first unused memory.
* @return       true if success, else false
*
* @pre None
* @post None
*
* \b Example
* @code
* void tx_application_define(void *first_unused_memory)
* {
*     ezThreadXPort_Init(first_unused_memory);
* }
* @endcode
*
* @see
*
*****************************************************************************/
bool ezThreadXPort_Init(void *first_unused_memory);


/*****************************************************************************
* Function: ezThreadXPort_GetInterface
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
struct ezTaskWorkerThreadInterfaces * ezThreadXPort_GetInterface(void);

#ifdef __cplusplus
}
#endif

#endif /* EZ_THREADX_PORT_ENABLE == 1 */
#endif /* _EZ_THREADX_PORT_H */


/* End of file */
