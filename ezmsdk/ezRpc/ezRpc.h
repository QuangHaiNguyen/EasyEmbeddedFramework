
/*******************************************************************************
* Filename:         ezRpc.h
* Author:           Hai Nguyen
* Original Date:    31.08.2022
* Last Update:      31.08.2022
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
* Written by Hai Nguyen 31.08.2022
*
*******************************************************************************/

/** @file   ezRpc.h
 *  @author Hai Nguyen
 *  @date   31.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _EZRPC_H
#define _EZRPC_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (CONFIG_RPC == 1U)

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

#endif /* CONFIG_RPC == 1U */
#endif /* _EZRPC_H */

/* End of file */

