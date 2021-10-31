/*******************************************************************************
* Title                 :   module
* Filename              :   module.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   28.03.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  28.03.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   debug.h
 *  @brief  macros for logging with debug level 
 */

#ifndef DEBUG_H
#define	DEBUG_H

#include "config.h"

#define DEBUG_LEVEL 3

#if !defined DEBUG_LEVEL
    #define DEBUG_LEVEL = 0
#endif

/*print, always activated, independent from the debug level*/
#define PRINTF(format, ...)         printf(format "\n", __VA_ARGS__ )
#define PRINTF_NO_NL(format, ...)   printf(format, __VA_ARGS__ )
#define PRINT1(a)                   PRINTF("%s", a)
#define PRINT2(a,b)                 PRINTF(a, b)
#define PRINT3(a,b,c)               PRINTF(a, b, c)


/*print, dependent on debug level*/
#if DEBUG_LEVEL >= 3
    #define PRINT_ERROR( format, ... )      printf( "[ERROR]::%s(%d) " format "\n", __FUNCTION__,  __LINE__, __VA_ARGS__ )
    #define PRINT_ERR1(a)                   PRINT_ERROR("%s", a)
    #define PRINT_ERR2(a,b)                 PRINT_ERROR(a,b)
    #define PRINT_ERR3(a,b,c)               PRINT_ERROR(a,b,c)
    #define PRINT_ERR4(a,b,c,d)             PRINT_ERROR(a,b,c,d)
#else
    #define PRINT_ERROR(...)
    #define PRINT_ERR1(a)
    #define PRINT_ERR2(a,b)
    #define PRINT_ERR3(a,b,c)
    #define PRINT_ERR4(a,b,c,d)
#endif

#if DEBUG_LEVEL >= 2
    #define PRINT_DEBUG( format, ... )      printf( "[DEBUG]::%s: " format "\n", __FUNCTION__, __VA_ARGS__ )
    #define PRINT_DEBUG1(a)                 PRINT_DEBUG("%s", a)
    #define PRINT_DEBUG2(a,b)               PRINT_DEBUG(a,b)
    #define PRINT_DEBUG3(a,b,c)             PRINT_DEBUG(a,b,c)
    #define PRINT_DEBUG4(a,b,c,d)           PRINT_DEBUG(a,b,c,d)
#else
    #define PRINT_DEBUG(...)
    #define PRINT_DEBUG1(a)
    #define PRINT_DEBUG2(a,b)
    #define PRINT_DEBUG3(a,b,c)
    #define PRINT_DEBUG4(a,b,c,d)
#endif

#if DEBUG_LEVEL >= 1
    #define PRINT_INFO(format, ...)         printf( "[INFO] " format "\n", __VA_ARGS__ )
    #define PRINT_INFO1(a)                  PRINT_INFO("%s", a)
    #define PRINT_INFO2(a,b)                PRINT_INFO(a,b)
    #define PRINT_INFO3(a,b,c)              PRINT_INFO(a,b,c)
    #define PRINT_INFO4(a,b,c,d)            PRINT_INFO(a,b,c,d)
#else
    #define PRINT_INFO(...)
    #define PRINT_INFO1(a)                 
    #define PRINT_INFO2(a,b)
    #define PRINT_INFO3(a,b,c)
    #define PRINT_INFO4(a,b,c,d)
#endif



#endif	/* DEBUG_H */

/* End of file*/
