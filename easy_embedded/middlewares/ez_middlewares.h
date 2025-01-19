/*****************************************************************************
* Filename:         ez_middlewares.h
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

/** @file   ez_middlewares.h
 *  @author Hai Nguyen
 *  @date   19.11.2024
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 */

#ifndef _EZ_MIDDLEWARES_H
#define _EZ_MIDDLEWARES_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_BUILD_WITH_CMAKE == 0U)
#include "ez_target_config.h"
#endif

#if (EZ_MIDDLEWARES_ENABLE == 1)
#include "ez_utilities_common.h"

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/


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
* Function: ezMidware_Initialize
*//** 
* @brief Initialize the middleware components
*
* @details Act as a place holder
*
* @param    None
* @return   ezSUCCESS if success, ezFAIL if fail
*
* @pre None
* @post None
*
* \b Example
* @code
* ezSTATUS status = ezMidware_Initialize();
* @endcode
*
* @see None
*
*****************************************************************************/
ezSTATUS ezMidware_Initialize(void);

#endif /* EZ_MIDDLEWARES_ENABLE == 1 */

#ifdef __cplusplus
}
#endif

#endif /* _EZ_MIDDLEWARES_H */


/* End of file */
