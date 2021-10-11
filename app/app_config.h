/*******************************************************************************
* Title                 :   module
* Filename              :   module.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  21.02.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   module.h
 *  @brief  Header template for a module
 */

#ifndef _APP_CONFIG_H
#define _APP_CONFIG_H


#define CLI                     1U
#define SCHEDULER               0U
#define SMALLOC                 1U
#define HELPER_LINKEDLIST       1U
#define HELPER_HEXDUMP          1U
#define DEBUG                   1U
#define RING_BUFFER             1U
#define HELPER_ASSERT           1U
#define STATEMACHINE            1U
#define IPC                     1U
#define BIN_PARSER              1U

/*Module ID section*/

/* CLI SECTION ***************************************************************/
#if (CLI == 1U)
#define CLI_MOD_ID                  0x01U
#endif

/* SCHEDULER SECTION *********************************************************/
#if (SCHEDULER == 1U)
#define SCHEDULER_MOD_ID            0x02U
/**< ID of scheduler module*/

#define NUM_OF_TASK                 5U
/**< number of supported task, maximum is 255 tasks*/

#define TASK_DIAGNOSE               0U
/**< turn on task timing debug out*/
#endif /* SCHEDULER */


#if (SMALLOC == 1U)
#if (HELPER_LINKEDLIST == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif

#define SMALLOC_MOD_ID              0x03U
#include "../helper/smalloc/smalloc.h"
#endif

#if (BIN_PARSER == 1U)
#define BIN_PARSER_MOD_ID           0x04U
#endif

#if (DEBUG == 1U)
#define DEBUG_MOD_ID                0x05U
#include "../ezmDebug/ezmDebug.h"
#endif

#if (HELPER_LINKEDLIST == 1U)
#define HELPER_LINKEDLIST_MOD_ID    0x06U
#include "../helper/linked_list/linked_list.h"
#endif

#if (HELPER_HEXDUMP == 1U)
#define HELPER_HEXDUMP_MOD_ID       0x07U
#include "../helper/hexdump/hexdump.h"
#endif

#if (RING_BUFFER == 1U)
#define RING_BUFFER_MOD_ID          0x08U
#include "../helper/ring_buffer/ring_buffer.h"
#endif

#if (HELPER_ASSERT == 1U)
#define HELPER_ASSERT_MOD_ID        0x09U
#include "../helper/ezmAssert/ezmAssert.h"
#endif

#if (STATEMACHINE == 1U)
#define STATEMACHINE_MOD_ID        0x0AU
#include "../statemachine/statemachine.h"
#endif

#if (IPC == 1U)
#define IPC_MOD_ID                  0x0BU
#endif

#endif /* _APP_CONFIG_H */
