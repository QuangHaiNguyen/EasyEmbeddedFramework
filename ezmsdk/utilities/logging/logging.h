
/*******************************************************************************
* Filename:         logging.h
* Author:           Hai Nguyen
* Original Date:    01.06.2022
* Last Update:      01.06.2022
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
* Written by Hai Nguyen 01.06.2022
*
*******************************************************************************/

/** @file   logging.h
 *  @author Hai Nguyen
 *  @date   01.06.2022
 *  @brief  This is the header file for logging module
 *  
 *  @details Logging module provides user with multi-level and multi-format
 *  logging. Originally, logging will use printf to print file on the console,
 *  but  it can be tailored to other IO
 * 
 */

#ifndef _LOGGING_H
#define _LOGGING_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include <stdio.h>
#include "app/app_config.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define LVL_NO_LOG      0   /**< set no logging */
#define LVL_CRITICAL    1   /**< set logging to level critical */
#define LVL_ERROR       2   /**< set logging to level error */
#define LVL_WARNING     3   /**< set logging to level warning */
#define LVL_NOTICE      4   /**< set logging to level notice */
#define LVL_INFO        5   /**< set logging to level info */
#define LVL_DEBUG       6   /**< set logging to level debug */
#define LVL_TRACE       7   /**< set logging to level trace */

#define PRINT_MODULE_NAME       1U  /**< logging shows module name */
#define PRINT_FILENAME          0U  /**< logging shows file name */
#define PRINT_LINE              0U  /**< logging shows line number */
#define PRINT_FUNCTION_NAME     0U  /**< logging shows function name */

#define dbg_print(fmt, ...)     printf(fmt, ##__VA_ARGS__)

#if (PRINT_MODULE_NAME == 1U)
#define print_module(mod_name)      dbg_print("[%s]::", mod_name)
#else
#define print_module(mod_name)
#endif

#if (PRINT_FILENAME == 1U)
#define print_file()     dbg_print("%s::", __FILE__)
#else
#define print_file()
#endif

#if (PRINT_LINE == 1U)
#define print_line()     dbg_print("(%d)::", __LINE__)
#else
#define print_line()
#endif

#if (PRINT_FUNCTION_NAME == 1U)
#define print_func()     dbg_print("%s::", __func__)
#else
#define print_func()
#endif

#if (LOGGING == 1U)
#if DEBUG_LVL >= LVL_CRITICAL
#define CRITICAL(fmt, ...)   do {\
                                dbg_print("[CRITICAL]::");\
                                print_module(MOD_NAME);\
                                print_file();\
                                print_line();\
                                print_func();\
                                dbg_print(fmt "\n", ##__VA_ARGS__);\
                            }while(0)
#else
#define CRITICAL(fmt, ...)
#endif

#if DEBUG_LVL >= LVL_ERROR
#define ERROR(fmt, ...)     do {\
                                dbg_print("[ERROR]::");\
                                print_module(MOD_NAME);\
                                print_file();\
                                print_line();\
                                print_func();\
                                dbg_print(fmt "\n", ##__VA_ARGS__);\
                            }while(0)
#else
#define ERROR(fmt, ...)
#endif

#if DEBUG_LVL >= LVL_WARNING
#define WARNING(fmt, ...)   do {\
                                if (DEBUG_LVL >= LVL_WARNING)\
                                {\
                                    dbg_print("[WARNING]::");\
                                    print_module(MOD_NAME);\
                                    print_file();\
                                    print_line();\
                                    print_func();\
                                    dbg_print(fmt "\n", ##__VA_ARGS__);\
                                }\
                            }while(0)
#else
#define WARNING(fmt, ...)
#endif

#if DEBUG_LVL >= LVL_NOTICE
#define NOTICE(fmt, ...)    do {\
                                dbg_print("[NOTICE]::");\
                                print_module(MOD_NAME);\
                                print_file();\
                                print_line();\
                                print_func();\
                                dbg_print(fmt "\n", ##__VA_ARGS__);\
                            }while(0)
#else
#define NOTICE(fmt, ...)
#endif

#if DEBUG_LVL >= LVL_INFO
#define INFO(fmt, ...)      do {\
                                dbg_print("[INFO]::");\
                                print_module(MOD_NAME);\
                                print_file();\
                                print_line();\
                                print_func();\
                                dbg_print(fmt "\n", ##__VA_ARGS__);\
                            }while(0)
#else
#define INFO(fmt, ...)
#endif

#if DEBUG_LVL >= LVL_DEBUG
#define DEBUG(fmt, ...)     do {\
                                dbg_print("[DEBUG]::");\
                                print_module(MOD_NAME);\
                                print_file();\
                                print_line();\
                                print_func();\
                                dbg_print(fmt "\n", ##__VA_ARGS__);\
                            }while(0)
#else
#define DEBUG(fmt, ...)
#endif

#if DEBUG_LVL >= LVL_TRACE
#define TRACE(fmt, ...)     do {\
                                dbg_print("[TRACE]::");\
                                print_module(MOD_NAME);\
                                print_file();\
                                print_line();\
                                print_func();\
                                dbg_print(fmt "\n", ##__VA_ARGS__);\
                            }while(0)
#else
#define TRACE(fmt, ...)
#endif

#else
#define CRITICAL(fmt, ...)
#define ERROR(fmt, ...)
#define WARNING(fmt, ...)
#define NOTICE(fmt, ...)
#define INFO(fmt, ...)
#define TRACE(fmt, ...)
#define DEBUG(fmt, ...)
#endif

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void Logging_DemoFeatures(void);

#endif /* _LOGGING_H */

/* End of file */

