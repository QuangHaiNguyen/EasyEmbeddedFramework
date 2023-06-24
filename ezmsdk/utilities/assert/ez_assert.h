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

#if (EZ_ASSERT == 1U)
#include "logging/ez_logging.h"
#define ASSERT(expr) \
            if(!(expr)) {dbg_print("\nassert:%s:%d\n", __func__ , __LINE__);while(1);}

#define ASSERT_MSG(expr, msg)                                                           \
            do                                                                          \
            {                                                                           \
                if(!(expr))                                                             \
                {dbg_print("\nassert:%s:%d - %s\n", __func__ , __LINE__, msg);while(1);} \
            }while(0);                                                                  \

#define ASSERT_CUST_MSG(expr, fmt, ...) \
            do                                                                                      \
            {                                                                                       \
                if(!(expr))                                                                         \
                {printf("\nassert:%s:%d - " fmt "\n", __func__ , __LINE__, ##__VA_ARGS__);while(1);}\
            }while(0);                                                                              \
            
#else
#define ASSERT(expr)
#define ASSERT_MSG(expr, msg)
#define ASSERT_CUST_MSG(expr, msg, ...)
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
