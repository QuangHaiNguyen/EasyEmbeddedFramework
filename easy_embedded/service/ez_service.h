/*****************************************************************************
* Filename:         ez_service.h
* Author:           Hai Nguyen
* Original Date:    20.11.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_service.h
 *  @author Hai Nguyen
 *  @date   20.11.2024
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 */

#ifndef _EZ_SERVICE_H
#define _EZ_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_BUILD_WITH_CMAKE == 0U)
#include "ez_target_config.h"
#endif

#if (EZ_SERVICE_ENABLE == 1)
#include "ez_utilities_common.h"


/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None */


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
* Function: ezService_Intialize
*//** 
* @brief Initialize the service component
*
* @details Place holder function. They maybe used later.
*
* @param    None
* @return   None
*
* @pre None
* @post None
*
* \b Example
* @code
* ezSTATUS status = ezService_Intialize();
* @endcode
*
* @see None
*
*****************************************************************************/
ezSTATUS ezService_Intialize(void);

#endif /* EZ_SERVICE_ENABLE == 1 */

#ifdef __cplusplus
}
#endif

#endif /* _EZ_SERVICE_H */


/* End of file */
