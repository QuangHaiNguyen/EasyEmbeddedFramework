/*****************************************************************************
* Filename:         ez_app.h
* Author:           Hai Nguyen
* Original Date:    09.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_app.h
 *  @author Hai Nguyen
 *  @date   09.03.2024
 *  @brief  Public API for app component
 *
 *  @details This component is responsible for initializing the sub-components
 *           of the app components
 */

#ifndef _EZ_APP_H
#define _EZ_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
* Includes
*****************************************************************************/
/* INCLUDE HEADER HERE */

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
* Function: ezApp_Initialize
*//** 
* @brief Initialize the sub-commponents of the app component
*
* @details
*
* @param    None
* @return   None
*
* @pre None
* @post None
*
* \b Example
* @code
* ezApp_Initialize();
* @endcode
*
* @see
*
*****************************************************************************/
void ezApp_Initialize(void);


#ifdef __cplusplus
}
#endif

#endif /* _EZ_APP_H */
/* End of file */
