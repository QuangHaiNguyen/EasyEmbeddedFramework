
/*******************************************************************************
* Filename:         temp.h
* Author:           Hai Nguyen
* Original Date:    21.01.2024
* Last Update:      21.01.2024
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
* Written by Hai Nguyen 21.01.2024
*
*******************************************************************************/

/** @file   temp.h
 *  @author Hai Nguyen
 *  @date   21.01.2024
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _TEMP_H
#define _TEMP_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *  
 */
typedef struct
{
    int a;  /**< an integer */
    int b;  /**< an integer */
}aType;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
int sum(int a, int b); /**< Short description of function */

#endif /* _TEMP_H */

/* End of file */

