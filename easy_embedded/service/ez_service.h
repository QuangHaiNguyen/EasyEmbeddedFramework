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

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#define A_MACRO     1   /**< a macro*/

/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/** @brief definition of a new type
 *
 */
typedef struct
{
    int a;  /**< an integer */
    int b;  /**< an integer */
}aType;


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Function Prototypes
*****************************************************************************/

/*****************************************************************************
* Function: sum
*//** 
* @brief one line description
*
* @details Detail description
*
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @pre None
* @post None
*
* \b Example
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*****************************************************************************/
int sum(int a, int b);

#endif /* EZ_SERVICE_ENABLE == 1 */

#ifdef __cplusplus
}
#endif

#endif /* _EZ_SERVICE_H */


/* End of file */
