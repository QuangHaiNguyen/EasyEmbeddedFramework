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

#ifndef DEBUG_CONF_H
#define	DEBUG_CONF_H

#include "stdio.h"

#define MODULE_DEBUG                1U  /*main switch to activate the debug output*/

/******************************************************************************
* debug output switch for each module
*******************************************************************************/
#define HELPER_LINKEDLIST_DEBUG     0U /* debug out for helper module linkedlist */
#define SMALLOC_DEBUG               0U /* debug out for smalloc module */
#define HEXDUMP_DEBUG               0U /* debug out for hexdump module */
#define SCHEDULER_DBG               0U /* debug out for scheduler module */
#define ASSERT_DBG                  1U /* debug out for scheduler module */
#define APP_DEBUG                   1U /* debug out for main module */
#define STATEMACHINE_DEBUG          0U /* debug out for state machine module */
#define IPC_DEBUG                   0U /* debug out for IPC module */
#define PARSER_DEBUG                1U /* debug out for parser module */
#define CLI_DEBUG                   0U /* debug out for cli module */
#define STCMEM_DEBUG                0U /* debug out for StcMem module */
#define KERNEL_DEBUG                1U /* debug out for kernel module*/
#define UART_DEBUG                  0U /* debug out for hal uart */

#endif