/*****************************************************************************
* Filename:         ez_bsp.h
* Author:           Hai Nguyen
* Original Date:    19.11.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_bsp.h
 *  @author Hai Nguyen
 *  @date   19.11.2024
 *  @brief  Public API of the Board Support Package
 *
 *  @details This module provides only the initialize function for the BSP
 */

#ifndef _EZ_BSP_H
#define _EZ_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_BUILD_WITH_CMAKE == 0U)
#include "ez_target_config.h"
#endif

#if (EZ_BSP_ENABLE == 1)
#include "ez_utilities_common.h"


/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None */


/*****************************************************************************
* Component Typedefs
*****************************************************************************/
/* None*/


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function: ezBsp_Initialize
*//** 
* @brief Initialize the board support package
*
* @details Place holder. They maybe used later.
*
* @return   ezSUCCESS if success, ezFAIL if fail
*
* @pre None
* @post None
*
* \b Example
* @code
* ezSTATUS status = ezBsp_Initialize();
* @endcode
*
*****************************************************************************/
ezSTATUS ezBsp_Initialize(void);

#endif /* EZ_BSP_ENABLE == 1 */

#ifdef __cplusplus
}
#endif

#endif /* _EZ_BSP_H */


/* End of file */
