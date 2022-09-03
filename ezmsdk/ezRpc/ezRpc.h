
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
#include "stdint.h"
#include "stdbool.h"

#if(CONFIG_STCMEM == 1U)
#include "utilities/ez_queue/ez_queue.h"
#else
#error STCMEM must be activated
#endif /* STCMEM */

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/


/** @brief Message type enumeration
 *
 */
typedef enum
{
    RPC_MSG_REQ,            /**< request */
    RPC_MSG_RESP,           /**< response */
}RPC_MSG_TYPE;


struct ezRpcMsgHeader
{
    uint8_t         tag;            /**< Tag of RPC, indicate type of service */
    uint8_t         is_encrypted;   /**< encrpypted flag */
    RPC_MSG_TYPE    type;           /**< RPC message type */
    uint32_t        uuid;           /**< UUID of the message */
    uint32_t        payload_size;   /**< Size of the payload */
};

/** @brief RPC message struct, omitting the SOF. SOF is sef to 0x80
 *
 * |======================================================================================|
 * | 0   | 1...4 | 5        | 6   | 7        | 8...11       | 12 13 ... n | n+1 ... n+m   |
 * |=====|=======|==========|=====|==========|==============|=============|===============|
 * | SOF | UUID  | Msg type | TAG | Encrypt. | Payload size | Payload     | CRC1 ... CRCm |
 * |======================================================================================|
 */
struct ezRpcMsg
{
    struct ezRpcMsgHeader   header;
    uint8_t                 *payload;       /**< The pointer to the payload itself*/
    uint8_t                 *crc;           /**< Pointer to the CRC value */
};


/** @brief Record of the request. Keep track of sent requests
 *
 */
struct ezRpcRequestRecord
{
    uint32_t    uuid;           /**< UUID of the request */
    uint32_t    timestamp;      /**< Time stamp when the request is created */
    char        *name;          /**< Name of the request */
    bool        is_available;   /**< Availalbe flag */
};

typedef void(*ServiceHandler)   (void *payload, uint16_t payload_size_byte);
typedef bool(*CrcVerify)        (struct ezRpcMsg *msg);
typedef void(*CrcCalculate)     (uint8_t *input, 
                                    uint32_t input_size,
                                    uint8_t *crc_output,
                                    uint32_t crc_output_size);


/** @brief Rpc service structure
 *
 */
struct ezRpcService
{
    uint8_t          tag;           /**< Stores the command code*/
    ServiceHandler   pfnService;    /**< pointer to function handling that command */
};


/** @brief definition of a new type
 *  
 */
struct ezRpc
{
    uint32_t            service_table_size; /**< Size of the command table, how many commands are there in total */
    struct ezRpcService *service_table;     /**< Poiter to the command table */
    uint8_t             parser_state;       /**< Store the state of the binary parser statemachine */
    struct ezRpcMsg     curr_msg;           /**< pointer to the current frame that the parser is working*/
    uint32_t            payload_index;      /**< Index of the payload of current frame */
    ezQueue             msg_queue;          /**< Queue to store request */
    CrcVerify           CrcVerify;          /**< Pointer to the CRC verification function */
    CrcCalculate        CrcCalculate;       /**< Pointer to the CRC calculation function */
    uint32_t            crc_size;           /**< Size of the crc value, in bytes*/
    uint8_t             is_encrypted;
    uint32_t            next_uuid;          /**< value of next uuid, assign this value to rpc message */
    struct ezRpcRequestRecord records[CONFIG_NUM_OF_REQUEST]; /* num of request*/
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/


/******************************************************************************
* Function : ezRpc_Initialization
*//**
* @Description:
*
* This function initializes RPC instance of the RPC module
*
* @param    *rpc_inst:          (IN)pointer to the rpc instance
* @param    *buff:              (IN)pointer to memory provided to this rpc instance
* @param    buff_size:          (IN)size of the buff
* @param    *service_table:     (IN)pointer to the table containing services, see
*                               ezRpcService
* @param    service_table_size: (IN)size of the table
* @return   ezSUCCESS or ezFAIL
*
*******************************************************************************/
ezSTATUS ezRpc_Initialization(struct ezRpc *rpc_inst,
                                uint8_t *buff,
                                uint32_t buff_size,
                                struct ezRpcService *service_table,
                                uint32_t service_table_size);


/******************************************************************************
* Function : ezRpc_SetCrcFunctions
*//**
* @Description:
*
* This function enables the RCR check capability of an RPC instance
*
* @param    *rpc_inst:      (IN)pointer to the rpc instance
* @param    crc_size:       (IN)size of the crc value
* @param    verify_func:    (IN)function to verify crc value
* @param    cal_func:       (IN)function to calculate crc value
* @return   ezSUCCESS or ezFAIL
*
*******************************************************************************/
ezSTATUS ezRpc_SetCrcFunctions(struct ezRpc *rpc_inst,
                                uint32_t crc_size,
                                CrcVerify verify_func,
                                CrcCalculate cal_func);


ezSTATUS ezRPC_CreateRpcMessage(struct ezRpc *rpc_inst,
                                RPC_MSG_TYPE type,
                                uint8_t tag,
                                uint8_t *payload,
                                uint32_t payload_size);


#endif /* CONFIG_RPC == 1U */
#endif /* _EZRPC_H */

/* End of file */

