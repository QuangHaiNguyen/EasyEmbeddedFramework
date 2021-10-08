/* 
 * File:   debug.h
 * Author: haing
 *
 * Created on April 8, 2020, 12:41 AM
 */

#ifndef DEBUG_H
#define	DEBUG_H

#define DEBUG_LEVEL 2

#if !defined DEBUG_LEVEL
    #define DEBUG_LEVEL = -1
#endif


#if DEBUG_LEVEL >= 2
    #define PRINT_DEBUG( format, ... )   printf( "[DEBUG]:%s::%s(%d) " format, __FILE__, __FUNCTION__,  __LINE__, __VA_ARGS__ )
#else
    #define PRINT_DEBUG(...)
#endif


#if DEBUG_LEVEL >= 1
    #define PRINT_ERROR( format, ... )   printf( "[ERROR]:%s::%s(%d) " format, __FILE__, __FUNCTION__,  __LINE__, __VA_ARGS__ )
#else
    #define PRINT_ERROR(...)
#endif

#if DEBUG_LEVEL >= 0
    #define PRINT_INFO( format, ... )   printf( "[INFO] " format, __VA_ARGS__ )
#else
    #define PRINT_INFO(...)
#endif

#endif	/* DEBUG_H */

