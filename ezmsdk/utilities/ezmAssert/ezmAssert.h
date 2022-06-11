/*******************************************************************************
* Title                 :   ezmAssert
* Filename              :   ezmAssert.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   24.05.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  24.05.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   ezmAssert.h
 *  @brief  Header template for a Assert
 */


#ifndef _EZMASSERT_H
#define _EZMASSERT_H

/*******************************************************************************
* Includes
*******************************************************************************/
/*None*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#include "app/app_config.h"

#if (HELPER_ASSERT == 1U)
#include "utilities/logging/logging.h"
#define ASSERT(expr)            if(!(expr)) {dbg_print("assert:%s:%d\n", __func__ , __LINE__);}
#define ASSERT_MSG(expr, msg)   if(!(expr)) {dbg_print("assert:%s:%d - %s\n", __func__ , __LINE__, msg);}
#else
#define ASSERT(expr)
#define ASSERT_MSG(expr, msg)
#endif /* HELPER_ASSERT */

/**< register your module id*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/*None*/

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/*None*/

/******************************************************************************
* Function Prototypes
*******************************************************************************/


#endif /* _EZMASSERT_H */

/* End of file*/
