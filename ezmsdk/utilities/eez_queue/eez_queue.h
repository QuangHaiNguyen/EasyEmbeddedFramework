
/*******************************************************************************
* Filename:         eez_queue.h
* Author:           Hai Nguyen
* Original Date:    20.08.2022
* Last Update:      20.08.2022
*
* -----------------------------------------------------------------------------
* Comany:           Easy Embedded
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Easy Embedded
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 20.08.2022
*
*******************************************************************************/

/** @file   eez_queue.h
 *  @author Hai Nguyen
 *  @date   20.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _EEZ_QUEUE_H
#define _EEZ_QUEUE_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (CONFIG_EEZ_QUEUE == 1U)

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

#endif /* CONFIG_EEZ_QUEUE */
#endif /* _EEZ_QUEUE_H */

/* End of file */

