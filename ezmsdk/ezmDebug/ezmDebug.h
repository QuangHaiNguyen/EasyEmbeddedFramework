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

#if (USING_EZM_PRINTF == 0U)
#include <stdio.h>
#else
#endif

/*print, always activated, independent from the debug level*/
#if (USING_EZM_PRINTF == 1U)
    #define PRINTF(format, ...)                 ezm_printf(format "\n", __VA_ARGS__ )
    #define PRINTF_NO_NL(format, ...)           ezm_printf(format, __VA_ARGS__ )
    #define PRINTF_MOD(MOD_NAME, format, ...)   ezm_printf("[%s]::" format "\n", MOD_NAME, __VA_ARGS__ )
#else
    #define PRINTF(format, ...)                 printf(format "\n", __VA_ARGS__ )
    #define PRINTF_NO_NL(format, ...)           printf(format, __VA_ARGS__ )
    #define PRINTF_MOD(MOD_NAME, format, ...)   printf("[%s]::" format "\n", MOD_NAME, __VA_ARGS__ )
#endif

#define PRINT1(a)                           PRINTF("%s", a)
#define PRINT2(a,b)                         PRINTF(a, b)
#define PRINT3(a,b,c)                       PRINTF(a, b, c)

/*print, dependent on debug level*/
#if DEBUG_LEVEL >= 3
    #define PRINT_ERROR( format, ... )      PRINTF( "[ERROR]::%s(%d) " format "\n", __FUNCTION__,  __LINE__, __VA_ARGS__ )
    #define PRINT_ERR(a)                    PRINT_ERROR("%s", a)
    #define PRINT_ERR1(a,b)                 PRINT_ERROR(a,b)
    #define PRINT_ERR2(a,b,c)               PRINT_ERROR(a,b,c)
    #define PRINT_ERR3(a,b,c,d)             PRINT_ERROR(a,b,c,d)
    #define PRINT_ERR4(a,b,c,d,e)           PRINT_ERROR(a,b,c,d,e)
#else
    #define PRINT_ERROR(...)                printf( "[ERROR]::%s(%d) " format "\n", __FUNCTION__,  __LINE__, __VA_ARGS__ )
    #define PRINT_ERR(a)                    PRINT_ERROR("%s", a)
    #define PRINT_ERR1(a,b)                 PRINT_ERROR(a,b)
    #define PRINT_ERR2(a,b,c)               PRINT_ERROR(a,b,c)
    #define PRINT_ERR3(a,b,c,d)             PRINT_ERROR(a,b,c,d)
    #define PRINT_ERR4(a,b,c,d,e)           PRINT_ERROR(a,b,c,d,e)
#endif

#if DEBUG_LEVEL >= 2
    #define PRINT_DEBUG(MOD_NAME,a)            PRINTF_MOD(MOD_NAME,"%s", a)
    #define PRINT_DEBUG1(MOD_NAME,a,b)         PRINTF_MOD(MOD_NAME,a,b)
    #define PRINT_DEBUG2(MOD_NAME,a,b,c)       PRINTF_MOD(MOD_NAME,a,b,c)
    #define PRINT_DEBUG3(MOD_NAME,a,b,c,d)     PRINTF_MOD(MOD_NAME,a,b,c,d)
    #define PRINT_DEBUG4(MOD_NAME,a,b,c,d,e)   PRINTF_MOD(MOD_NAME,a,b,c,d,e)
#else
    #define PRINT_DEBUG(MOD_NAME,...)
    #define PRINT_DEBUG1(MOD_NAME,a,b)
    #define PRINT_DEBUG2(MOD_NAME,a,b,c)
    #define PRINT_DEBUG3(MOD_NAME,a,b,c,d)
    #define PRINT_DEBUG4(MOD_NAME,a,b,c,d,e)
#endif

#if DEBUG_LEVEL >= 1
    #define PRINTINFO(format, ...)          PRINTF( "[INFO] " format "\n", __VA_ARGS__ )
    #define PRINT_INFO(a)                   PRINTINFO("%s", a)
    #define PRINT_INFO1(a,b)                PRINTINFO(a,b)
    #define PRINT_INFO2(a,b,c)              PRINTINFO(a,b,c)
    #define PRINT_INFO3(a,b,c,d)            PRINTINFO(a,b,c,d)
    #define PRINT_INFO4(a,b,c,d,e)          PRINTINFO(a,b,c,d,e)
#else
    #define PRINT_INFO(...)
    #define PRINT_INFO(a)
    #define PRINT_INFO1(a,b)
    #define PRINT_INFO2(a,b,c)
    #define PRINT_INFO3(a,b,c,d)
    #define PRINT_INFO4(a,b,c,d,e)
#endif

typedef void(*PRINT_STRING)(char *string, int string_size);

int ezm_printf(char* fmt, ...);
int ezm_snprintf(char* buff, int buff_size, char* fmt, ...);
unsigned int ezm_StringLen(char * string);
int ezm_StringCopy(char* src, char* dst, int size_dst);
#endif	/* DEBUG_H */

/* End of file*/
