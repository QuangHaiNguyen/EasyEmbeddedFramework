
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
/* None */

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
    int         timestamp;      /**< Time stamp when the request is created */
    char        *name;          /**< Name of the request */
    bool        is_available;   /**< Availalbe flag */
};

typedef uint32_t(*RpcTransmit)  (uint8_t *tx_data, uint32_t tx_size);
typedef uint32_t(*RpcReceive)   (uint8_t *rx_data, uint32_t rx_size);
typedef void(*ServiceHandler)   (void *payload, uint32_t payload_size_byte);
typedef bool(*CrcVerify)        (uint8_t *input,
                                 uint32_t input_size,
                                 uint8_t *crc,
                                 uint32_t crc_size);

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
    uint8_t             deserializer_state; /**< Store the state of the binary parser statemachine */
    struct ezRpcMsg     curr_msg;           /**< pointer to the current frame that the parser is working*/
    uint32_t            byte_count;         /**< index for deserialize rpc message */
    ezQueue             tx_msg_queue;       /**< Queue to store request */
    ezQueue             rx_msg_queue;       /**< Queue to store request */
    CrcVerify           IsCrcCorrect;       /**< Pointer to the CRC verification function */
    CrcCalculate        CrcCalculate;       /**< Pointer to the CRC calculation function */
    uint32_t            crc_size;           /**< Size of the crc value, in bytes*/
    uint8_t             is_encrypted;       /**< Flag to indicate the rpc instance using encryption */
    uint32_t            next_uuid;          /**< Value of next uuid, assign this value to rpc message */
    RpcTransmit         RpcTransmit;        /**< Function to transmit RPC message */
    RpcReceive          RpcReceive;         /**< Function to receive RPC message */
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
* This function enables the CRC check capability of an RPC instance
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


/******************************************************************************
* Function : ezRpc_SetTxRxFunctions
*//**
* @Description:
*
* This function set the interface for transmitting and receiving data
*
* @param    *rpc_inst:      (IN)pointer to the rpc instance
* @param    tx_function:    (IN)function to transmit data
* @param    rx_function:    (IN)function to receive data
* @return   ezSUCCESS or ezFAIL
*
*******************************************************************************/
ezSTATUS ezRpc_SetTxRxFunctions(struct ezRpc *rpc_inst,
                                RpcTransmit tx_function,
                                RpcReceive rx_function);


/******************************************************************************
* Function : ezRPC_CreateRpcRequest
*//**
* @Description:
*
* This function creates an RPC request and put it in the transmit queue
*
* @param    *rpc_inst:      (IN)pointer to the rpc instance
* @param    tag:            (IN)tag value
* @param    *payload:       (IN)pointer to payload to send
* @param    payload_size:   (IN)siez of the payload
* @return   ezSUCCESS or ezFAIL
*
*******************************************************************************/
ezSTATUS ezRPC_CreateRpcRequest(struct ezRpc *rpc_inst,
    uint8_t tag,
    uint8_t *payload,
    uint32_t payload_size);


/******************************************************************************
* Function : ezRPC_CreateRpcResponse
*//**
* @Description:
*
* This function creates an RPC response and put it in the transmit queue
*
* @param    *rpc_inst:      (IN)pointer to the rpc instance
* @param    uuid:           (IN)uuid value, must match the request value
* @param    tag:            (IN)tag value
* @param    *payload:       (IN)pointer to payload to send
* @param    payload_size:   (IN)siez of the payload
* @return   ezSUCCESS or ezFAIL
*
*******************************************************************************/
ezSTATUS ezRPC_CreateRpcResponse(struct ezRpc *rpc_inst,
    uint8_t tag,
    uint32_t uuid,
    uint8_t *payload,
    uint32_t payload_size);


/******************************************************************************
* Function : ezRPC_Run
*//**
* @Description: Run the RPC instance, must be call in a tick function, a loop or
* a task to advance the internal state machine
*
* @param    *rpc_inst:      (IN)pointer to the rpc instance
* @return   None
*
*******************************************************************************/
void ezRPC_Run(struct ezRpc *rpc_inst);


/******************************************************************************
* Function : ezRPC_NumOfTxPendingMsg
*//**
* @Description: Return the number of messages waiting to be transmitted.
*               It is used for disagnostic or testing purpose
*
* @param    *rpc_inst:      (IN)pointer to the rpc instance
* @return   number of messages
*
*******************************************************************************/
uint32_t ezRPC_NumOfTxPendingMsg(struct ezRpc *rpc_inst);


/******************************************************************************
* Function : ezRPC_NumOfPendingRecords
*//**
* @Description: Return the number pending records, i.e. requests that is waiting
* for the responses
*
* @param    *rpc_inst:      (IN)pointer to the rpc instance
* @return   number of records
*
*******************************************************************************/
uint32_t ezRPC_NumOfPendingRecords(struct ezRpc *rpc_inst);


/******************************************************************************
* Function : ezRpc_IsRpcInstanceReady
*//**
* @Description: Return the status if the rpc instance is ready
*
* @param    *rpc_inst:      (IN)pointer to the rpc instance
* @return   true if ready, else false
*
*******************************************************************************/
bool ezRpc_IsRpcInstanceReady(struct ezRpc *rpc_inst);


#endif /* CONFIG_RPC == 1U */
#endif /* _EZRPC_H */

/* End of file */

