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
#define SMALLOC                 0U
#define BIN_PARSER              0U
#define HELPER_LINKEDLIST       1U
#define HELPER_HEXDUMP          0U
#define DEBUG                   1U
#define RING_BUFFER             0U
#define HELPER_ASSERT           0U
#define STATEMACHINE            1U
#define IPC                     1U

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

/* SMALLOC SECTION ***********************************************************/
#if (SMALLOC == 1U)
#if (HELPER_LINKEDLIST == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif

#define SMALLOC_MOD_ID              0x03U
/**< ID of smaloc module*/

#define STATIC_MEMORY_SIZE          512U
/**< number of bytes reserved for the buffer*/

#define ENABLE_STATS                0U
/**< Enable statisic*/

#endif /* SMALLOC */

/* BIN_PARSER SECTION ********************************************************/
#if (BIN_PARSER == 1U)

#if (SMALLOC == 0U)
#error module HELPER_LINKEDLIST must be activated
#endif /* SMALLOC */

#define BIN_PARSER_MOD_ID           0x04U
/**< ID of bin cmd parser module*/

#define PAYLOAD_MAX_SIZE            32U
/**< maximum size of the payload, in binary*/

#define SOF                         0x00U
/**< start of frame value*/

#define CRC_SIZE                    0x02U
/**< size of the crc, in byte*/

#endif /* BIN_PARSER */

/* DEBUG SECTION **************************************************************/
#if (DEBUG == 1U)
#define DEBUG_MOD_ID                0x05U
#include "../ezmDebug/ezmDebug.h"
#endif /* DEBUG */

/* HELPER_LINKEDLIST SECTION **************************************************/
#if (HELPER_LINKEDLIST == 1U)
#define HELPER_LINKEDLIST_MOD_ID    0x06U
#include "../helper/linked_list/linked_list.h"
#endif /* HELPER_LINKEDLIST */


/* HELPER_HEXDUMP SECTION *****************************************************/
#if (HELPER_HEXDUMP == 1U)
#define HELPER_HEXDUMP_MOD_ID       0x07U
#include "../helper/hexdump/hexdump.h"

#if (DEBUG == 0U)
#error DEBUG module must be activated
#endif /* DEBUG */

#endif /* HELPER_HEXDUMP */

/* HELPER_HEXDUMP SECTION *****************************************************/
#if (RING_BUFFER == 1U)
#define RING_BUFFER_MOD_ID          0x08U
#include "../helper/ring_buffer/ring_buffer.h"

#define STATIC_MEM  1

#if (STATIC_MEM == 1)
    #define STATIC_MEM_SIZE 512U
    #if STATIC_MEM_SIZE > 0xFFFF
        #error size is bigger than 16 bits
    #endif /* STATIC_MEM_SIZE */
#else
    #if (SMALLOC == 0U)
        #error SMALLOC module must be activated
    #endif /* SMALLOC */
#endif /*STATIC_MEM*/

#endif /* RING_BUFFER */


/* HELPER_ASSERT SECTION *****************************************************/
#if (HELPER_ASSERT == 1U)
#define HELPER_ASSERT_MOD_ID        0x09U
#include "../helper/ezmAssert/ezmAssert.h"
#endif /* HELPER_ASSERT */

#if (STATEMACHINE == 1U)
#define STATEMACHINE_MOD_ID        0x0AU
#include "../statemachine/statemachine.h"
#endif

#if (IPC == 1U)
#define IPC_MOD_ID                  0x0BU
#endif

#endif /* _APP_CONFIG_H */
