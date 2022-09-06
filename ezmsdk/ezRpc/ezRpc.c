
/*******************************************************************************
* Filename:         ezRpc.c
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

/** @file   ezRpc.c
 *  @author Hai Nguyen
 *  @date   31.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezRpc.h"

#if (CONFIG_RPC == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "ezRpc"       /**< module name */

#include "utilities/logging/logging.h"
#include "ezmKernel/ezmKernel.h"

#include <string.h>


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define RPC_HEADER_SIZE     12      /**< size of the RPC message header in bytes*/
#define SOF                 0x80    /**< start of frame, for syncronisation */
#define RPC_BYTE_READ       1U      /**< number of byte being read by RPC */


/******************************************************************************
* Module Typedefs
*******************************************************************************/


/** @brief Message type enumeration
 *
 */
typedef enum
{
    STATE_RX_DATA,          /**< */
    STATE_HANDLE_RX_DATA,   /**< */
    STATE_TX_DATA,          /**< */
    STATE_ERR,              /**< */
}RPC_STATES;


/** @brief Message type enumeration
 *
 */
typedef enum
{
    STATE_SOF,          /**< */
    STATE_UUID,         /**< */
    STATE_MSG_TYPE,     /**< */
    STATE_TAG,          /**< */
    STATE_ENCRYPT_FLAG, /**< */
    STATE_PAYLOAD_SIZE, /**< */
    STATE_PAYLOAD,      /**< */
    STATE_CRC,          /**< */
}RPC_DESERIALIZE_STATES;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezRpc_ResetAllRecords(struct ezRpcRequestRecord *records);
static ezSTATUS ezRpc_SerializeRpcHeader(uint8_t *buff,
                                         uint32_t buff_size,
                                         struct ezRpcMsgHeader *header);
static bool ezRpc_IsCrcActivated(struct ezRpc *rpc_inst);
static struct ezRpcRequestRecord *ezRpc_GetAvailRecord(struct ezRpc *rpc_inst);
static void ezRpc_DeserializedData(struct ezRpc *rpc_inst, uint8_t rx_byte);

/*Helper functions for debugging */
#if (DEBUG_LVL == LVL_TRACE)
void ezRpc_PrintHeader(struct ezRpcMsgHeader *header);
void ezRpc_PrintPayload(uint8_t *payload, uint32_t size);
void ezRpc_PrintCrc(uint8_t *crc, uint32_t size);
#endif /* DEBUG_LVL == LVL_TRACE */


/******************************************************************************
* External functions
*******************************************************************************/


ezSTATUS ezRpc_Initialization(struct ezRpc *rpc_inst,
                                uint8_t *buff,
                                uint32_t buff_size,
                                struct ezRpcService *service_table,
                                uint32_t service_table_size)
{
    ezSTATUS status = ezSUCCESS;

    TRACE("ezRpc_Initialization()");

    if (rpc_inst != NULL
        && buff != NULL
        && buff_size > 0
        && service_table != NULL
        && service_table_size > 0)
    {
        /* clean the struct */
        memset(rpc_inst, 0, sizeof(struct ezRpc));

        ezRpc_ResetAllRecords(rpc_inst->records);


        status = ezQueue_CreateQueue(&rpc_inst->tx_msg_queue, buff, buff_size/2);
        if (status == ezSUCCESS)
        {
            status = ezQueue_CreateQueue(
                &rpc_inst->rx_msg_queue,
                (buff + buff_size/2),
                buff_size/2);
        }

        if (status == ezSUCCESS)
        {
            rpc_inst->service_table = service_table;
            rpc_inst->service_table_size = service_table_size;

            rpc_inst->rpc_state = STATE_RX_DATA;
            rpc_inst->deserializer_state = STATE_SOF;

            rpc_inst->byte_count = 0;
        }
    }
    else
    {
        status = ezFAIL;
    }

    return status;
}


ezSTATUS ezRpc_SetCrcFunctions(struct ezRpc *rpc_inst,
                                uint32_t crc_size,
                                CrcVerify verify_func,
                                CrcCalculate cal_func)
{
    ezSTATUS status = ezSUCCESS;

    TRACE("ezRpc_SetCrcFunctions()");

    if (rpc_inst != NULL
        && verify_func != NULL
        && cal_func != NULL
        && crc_size > 0)
    {
        rpc_inst->crc_size = crc_size;
        rpc_inst->IsCrcCorrect = verify_func;
        rpc_inst->CrcCalculate = cal_func;
    }
    else
    {
        status = ezFAIL;
    }

    return status;
}


ezSTATUS ezRpc_SetTxRxFunctions(struct ezRpc *rpc_inst,
                                RpcTransmit tx_function,
                                RpcReceive rx_function)
{
    ezSTATUS status = ezSUCCESS;

    if (rpc_inst != NULL && tx_function != NULL && rx_function != NULL)
    {
        rpc_inst->RpcTransmit = tx_function;
        rpc_inst->RpcReceive = rx_function;
    }

    return status;
}


ezSTATUS ezRPC_CreateRpcMessage(struct ezRpc *rpc_inst,
                                RPC_MSG_TYPE type,
                                uint8_t tag,
                                uint8_t *payload,
                                uint32_t payload_size)
{
    uint32_t alloc_size = RPC_HEADER_SIZE + payload_size;
    uint8_t *buff = NULL;
    struct ezRpcMsgHeader temp_header = { 0 };
    ezSTATUS status = ezSUCCESS;
    struct ezRpcRequestRecord *record = NULL;
    ezReservedElement elem = NULL;

    TRACE("ezRPC_CreateRpcMessage()");

    if (rpc_inst != NULL)
    {
        if (rpc_inst->is_encrypted)
        {
            /* TODO must allocate data according to encryption algo */
            alloc_size += rpc_inst->crc_size;
        }
        else
        {
            alloc_size += rpc_inst->crc_size;
        }

        DEBUG("[payload + crc size = %d bytes]", alloc_size);

        record = ezRpc_GetAvailRecord(rpc_inst);

        if (record != NULL)
        {
            elem = ezQueue_ReserveElement(
                &rpc_inst->tx_msg_queue,
                &buff,
                alloc_size);

            if (elem != NULL)
            {
                temp_header.tag = tag;
                temp_header.type = type;
                temp_header.payload_size = payload_size;
                temp_header.uuid = ++rpc_inst->next_uuid;
                temp_header.is_encrypted = rpc_inst->is_encrypted;

                status = ezRpc_SerializeRpcHeader(buff,
                                                  alloc_size,
                                                  &temp_header);
            }
            else
            {
                status = ezFAIL;
            }
        }
        else
        {
            status = ezFAIL;
        }

        if (status == ezSUCCESS && payload != NULL && payload_size > 0)
        {
            buff += RPC_HEADER_SIZE;
            alloc_size -= RPC_HEADER_SIZE;

            memcpy(buff, payload, payload_size);

            DEBUG("payload value:");
            HEXDUMP(buff, payload_size);
        }

        if (status == ezSUCCESS && ezRpc_IsCrcActivated(rpc_inst))
        {
            alloc_size -= payload_size;

            if (alloc_size >= rpc_inst->crc_size)
            {
                buff += payload_size;

                rpc_inst->CrcCalculate(payload,
                                        payload_size,
                                        buff,
                                        rpc_inst->crc_size);

                DEBUG("crc value:");
                HEXDUMP(buff, rpc_inst->crc_size);
            }
            else
            {
                status = ezFAIL;
            }
        }

        if(record != NULL)
        {
            if (status == ezSUCCESS)
            {
                record->uuid = temp_header.uuid;
                record->name = NULL;
                record->timestamp = ezmKernel_GetTickMillis();
            }
            else
            {
                record->is_available = true;
            }
        }

        if (status == ezSUCCESS)
        {
            status = ezQueue_PushReservedElement(
                &rpc_inst->tx_msg_queue,
                elem);
#if (DEBUG_LVL == LVL_TRACE)
            TRACE("serialized header:");
            HEXDUMP(buff, alloc_size);
#endif /* DEBUG_LVL == LVL_TRACE */
        }
        else
        {
            status = ezQueue_ReleaseReservedElement(
                &rpc_inst->tx_msg_queue,
                elem);
        }
    }
    else
    {
        status = ezFAIL;
    }

    return status;
}


void ezRPC_Run(struct ezRpc *rpc_inst)
{
    ezSTATUS status = ezSUCCESS; 
    if (rpc_inst != NULL && ezRpc_IsRpcInstanceReady(rpc_inst) == true)
    {
        switch ((RPC_STATES)rpc_inst->rpc_state)
        {
        case STATE_RX_DATA:
        {
            uint8_t one_byte = 0U;

            if (rpc_inst->RpcReceive)
            {
                uint32_t ret_byte = 0;

                /**Try to read all avainale bytes, only move to the next state
                 * when there is nothing to read. This will prevent missing any
                 * byte
                 */
                while (rpc_inst->RpcReceive(&one_byte, RPC_BYTE_READ) == RPC_BYTE_READ)
                {
                    ezRpc_DeserializedData(rpc_inst, one_byte);
                }
            }
            rpc_inst->rpc_state = STATE_HANDLE_RX_DATA;
            break;
        }

        case STATE_HANDLE_RX_DATA:
        {
            if (ezQueue_GetNumOfElement(&rpc_inst->rx_msg_queue) > 0)
            {
                struct ezRpcMsgHeader *header = NULL;
                uint32_t header_size = 0U;
                uint8_t *payload = NULL;
                uint32_t payload_size = 0U;

                bool service_found = false;

                status = ezQueue_GetFront(&rpc_inst->rx_msg_queue,
                    (void *)&header,
                    &header_size);

                if (status == ezSUCCESS)
                {
#if(DEBUG_LVL == LVL_TRACE)
                    ezRpc_PrintHeader(header);
#endif /* DEBUG_LVL == LVL_TRACE */

                    if (header->type == RPC_MSG_RESP)
                    {
                        status = ezFAIL;
                        /* check in the records if we sent it */
                        for (uint32_t i = 0; i < CONFIG_NUM_OF_REQUEST; i++)
                        {
                            if (rpc_inst->records[i].uuid == header->uuid)
                            {
                                status = ezSUCCESS;

                                /* record found, so we clear it */
                                rpc_inst->records[i].is_available = true;
                            }
                        }

                        if (status == ezFAIL)
                        {
                            (void)ezQueue_PopFront(&rpc_inst->rx_msg_queue);
                            (void)ezQueue_PopFront(&rpc_inst->rx_msg_queue);
                        }
                    }
                }

                if (status == ezSUCCESS)
                {
                    for (uint32_t i = 0; i < rpc_inst->service_table_size; i++)
                    {
                        if (rpc_inst->service_table[i].tag == header->tag)
                        {
                            (void)ezQueue_PopFront(&rpc_inst->rx_msg_queue);

                            status = ezQueue_GetFront(&rpc_inst->rx_msg_queue,
                                (void *)&payload,
                                &payload_size);
#if(DEBUG_LVL == LVL_TRACE)
                            if (status == ezSUCCESS)
                            {
                                ezRpc_PrintPayload(payload, payload_size);
                            }
#endif /* DEBUG_LVL == LVL_TRACE */

                            if (status == ezSUCCESS &&
                                rpc_inst->service_table[i].pfnService != NULL)
                            {
                                rpc_inst->service_table[i].pfnService(payload, payload_size);
                            }

                            service_found = true;
                            break;
                        }
                    }

                    if (service_found == false)
                    {
                        /* no service found for the tag */
                        (void)ezQueue_PopFront(&rpc_inst->rx_msg_queue);
                    }
                    (void)ezQueue_PopFront(&rpc_inst->rx_msg_queue);
                }
            }
            rpc_inst->rpc_state = STATE_TX_DATA;
            break;
        }

        case STATE_TX_DATA:
        {
            if (ezQueue_GetNumOfElement(&rpc_inst->tx_msg_queue) > 0)
            {
                uint8_t *req;
                uint32_t req_size;

                status = ezQueue_GetFront(&rpc_inst->tx_msg_queue,
                    (void *)&req,
                    &req_size);
                if (status == ezSUCCESS)
                {
                    rpc_inst->RpcTransmit(req, req_size);
                }

                status = ezQueue_PopFront(&rpc_inst->tx_msg_queue);
            }

            rpc_inst->rpc_state = STATE_RX_DATA;
            break;
        }

        case STATE_ERR:
            rpc_inst->rpc_state = STATE_RX_DATA;
            break;

        default:
            break;
        }
    }
}


bool ezRpc_IsRpcInstanceReady(struct ezRpc *rpc_inst)
{
    bool is_ready = false;

    if (rpc_inst != NULL)
    {
        is_ready = ((rpc_inst->service_table != NULL)
                    && (rpc_inst->service_table_size > 0)
                    && (ezQueue_IsQueueReady(&rpc_inst->rx_msg_queue))
                    && (ezQueue_IsQueueReady(&rpc_inst->tx_msg_queue))
                    && (rpc_inst->RpcTransmit != NULL)
                    && (rpc_inst->RpcReceive));
    }

    return is_ready;
}


uint32_t ezRPC_NumOfTxPendingMsg(struct ezRpc *rpc_inst)
{
    uint32_t num_of_msg = 0;
    if (rpc_inst != NULL)
    {
        num_of_msg = ezQueue_GetNumOfElement(&rpc_inst->tx_msg_queue);
    }

    return num_of_msg;
}


/******************************************************************************
* Internal functions
*******************************************************************************/


/******************************************************************************
* Function : ezRpc_ResetAllRecords
*//**
* @Description: Reset all records of the rpc module
*
* @param    *records: (IN)pointer to the record
* @return   None
*
*******************************************************************************/
static void ezRpc_ResetAllRecords(struct ezRpcRequestRecord *records)
{
    if (records != NULL)
    {
        for (uint32_t i = 0; i < CONFIG_NUM_OF_REQUEST; i++)
        {
            records[i].is_available = true;
            records[i].name = NULL;
            records[i].timestamp = 0;
            records[i].uuid = 0;
        }
    }
}


/******************************************************************************
* Function : ezRpc_SerializeRpcHeader
*//**
* @Description: serialize the header of a RPC message
*
* @param    *buff: (IN)Buffer containing the serialized header
* @param    buff_size: (IN)Size of the header
* @param    *header: (IN)Pointer to the header
* @return   ezSUCCESS: success
*           ezFAIL: fail
*
*******************************************************************************/
static ezSTATUS ezRpc_SerializeRpcHeader(uint8_t *buff,
                                        uint32_t buff_size,
                                        struct ezRpcMsgHeader *header)
{
    ezSTATUS status = ezSUCCESS;

    if (buff != NULL && buff_size >= RPC_HEADER_SIZE)
    {
        *buff = SOF;
        buff++;

        *(uint32_t *)buff = header->uuid;
        buff += sizeof(uint32_t);

        *buff = (uint8_t)header->type;
        buff++;

        *buff = header->tag;
        buff++;

        *buff = header->is_encrypted;
        buff++;

        *(uint32_t *)buff = header->payload_size;
    }
    else
    {
        status = ezFAIL;
    }

    return status;
}


/******************************************************************************
* Function : ezRpc_IsCrcActivated
*//**
* @Description: Return to status if CRC calculation is activated
*
* @param    *rpc_inst: (IN)Rpc instance
* @return   true if CRC calculation is activated
*
*******************************************************************************/
static bool ezRpc_IsCrcActivated(struct ezRpc *rpc_inst)
{
    return (rpc_inst->crc_size > 0
        && rpc_inst->CrcCalculate != NULL
        && rpc_inst->IsCrcCorrect != NULL);
}


/******************************************************************************
* Function : ezRpc_GetAvailRecord
*//**
* @Description: Get an available record from the records array
*
* @param    *rpc_inst: (IN)Rpc instance
* @return   a record or NULL if no record is available
*
*******************************************************************************/
static struct ezRpcRequestRecord *ezRpc_GetAvailRecord(struct ezRpc *rpc_inst)
{
    struct ezRpcRequestRecord *ret_record = NULL;

    if (rpc_inst != NULL)
    {
        for (uint32_t i = 0; i < CONFIG_NUM_OF_REQUEST; i++)
        {
            if (rpc_inst->records[i].is_available == true)
            {
                ret_record = &rpc_inst->records[i];
                rpc_inst->records[i].is_available = false;
                break;
            }
        }
    }

    return ret_record;
}


/******************************************************************************
* Function : ezRpc_DeserializedData
*//**
* @Description: Deserialize data from the communoication interface
*
* @param    *rpc_inst: (IN)pointer to rpc instance
* @param    rx_byte: (IN)byte receive from the communication interface
* @return   None
*
*******************************************************************************/
static void ezRpc_DeserializedData(struct ezRpc *rpc_inst, uint8_t rx_byte)
{
    static uint8_t                  *payload = NULL;
    static uint8_t                  *crc = NULL;
    ezSTATUS                        status = ezSUCCESS;
    static struct ezRpcMsgHeader    *temp_header = NULL;
    static ezReservedElement        payload_elem = NULL;
    static ezReservedElement        crc_elem = NULL;
    static ezReservedElement        header_elem = NULL;

    if (rpc_inst != NULL)
    {
        switch (rpc_inst->deserializer_state)
        {
        case STATE_SOF:
            TRACE("STATE_SOF");
            rpc_inst->byte_count = 0;
            rpc_inst->curr_msg.header.uuid = 0;
            rpc_inst->curr_msg.header.payload_size = 0;

            if (rx_byte == SOF)
            {
                header_elem = ezQueue_ReserveElement(&rpc_inst->rx_msg_queue,
                    (void*)&temp_header,
                    sizeof(struct ezRpcMsgHeader));

                if (header_elem != NULL)
                {
                    rpc_inst->deserializer_state = STATE_UUID;
                    DEBUG("Got SOF");
                }
            }
            break;

        case STATE_UUID:
            TRACE("STATE_UUID");
            if (temp_header != NULL)
            {
                temp_header->uuid = (temp_header->uuid << 8) | rx_byte;
                rpc_inst->byte_count++;
                if (rpc_inst->byte_count >= sizeof(uint32_t))
                {
                    rpc_inst->byte_count = 0;
                    rpc_inst->deserializer_state = STATE_MSG_TYPE;
                }
            }
            else
            {
                rpc_inst->deserializer_state = STATE_SOF;
            }
            
            break;

        case STATE_MSG_TYPE:
            TRACE("STATE_MSG_TYPE");
            if (rx_byte == RPC_MSG_REQ || rx_byte == RPC_MSG_RESP)
            {
                temp_header->type = rx_byte;
                rpc_inst->deserializer_state = STATE_TAG;
            }
            else
            {
                DEBUG("wrong message type");
                rpc_inst->deserializer_state = STATE_SOF;
            }
            break;

        case STATE_TAG:
            TRACE("STATE_TAG");
            temp_header->tag = rx_byte;
            rpc_inst->deserializer_state = STATE_ENCRYPT_FLAG;
            break;

        case STATE_ENCRYPT_FLAG:
            TRACE("STATE_ENCRYPT_FLAG");
            temp_header->is_encrypted = rx_byte;
            rpc_inst->deserializer_state = STATE_PAYLOAD_SIZE;
            break;

        case STATE_PAYLOAD_SIZE:
            TRACE("STATE_PAYLOAD_SIZE");
            temp_header->payload_size = (temp_header->payload_size << 8) | rx_byte;
            rpc_inst->byte_count++;

            if (rpc_inst->byte_count >=  sizeof(uint32_t))
            {
                if (status == ezSUCCESS)
                {
                    payload = NULL;

                    payload_elem = ezQueue_ReserveElement(
                        &rpc_inst->rx_msg_queue,
                        (void*)&payload,
                        temp_header->payload_size);
                }

                if (payload_elem != NULL && payload != NULL)
                {
                    rpc_inst->byte_count = 0;
                    rpc_inst->deserializer_state = STATE_PAYLOAD;

#if(DEBUG_LVL == LVL_TRACE)
                    ezRpc_PrintHeader(temp_header);
#endif /* DEBUG_LVL == LVL_TRACE */
                }
                else
                {
                    (void)ezQueue_ReleaseReservedElement(&rpc_inst->rx_msg_queue, header_elem);
                    rpc_inst->deserializer_state = STATE_SOF;
                    DEBUG("Queue operation error");
                }

            }
            break;

        case STATE_PAYLOAD:
            TRACE("STATE_PAYLOAD");

            *(payload + rpc_inst->byte_count) = rx_byte;
            rpc_inst->byte_count++;

            if (rpc_inst->byte_count >= temp_header->payload_size)
            {
#if(DEBUG_LVL == LVL_TRACE)
                ezRpc_PrintPayload(payload, temp_header->payload_size);
#endif /* DEBUG_LVL == LVL_TRACE */

                if (ezRpc_IsCrcActivated(rpc_inst))
                {
                    crc = NULL;
                    crc_elem = ezQueue_ReserveElement(&rpc_inst->rx_msg_queue,
                                                    (void *)&crc,
                                                    rpc_inst->crc_size);

                    if (crc_elem != NULL && crc != NULL)
                    {
                        rpc_inst->byte_count = 0;
                        rpc_inst->deserializer_state = STATE_CRC;
                    }
                    else
                    {
                        (void)ezQueue_ReleaseReservedElement(&rpc_inst->rx_msg_queue, header_elem);
                        (void)ezQueue_ReleaseReservedElement(&rpc_inst->rx_msg_queue, payload_elem);
                        rpc_inst->deserializer_state = STATE_SOF;
                        DEBUG("Queue operation error");
                    }
                }
                else
                {
                    status = ezQueue_PushReservedElement(
                        &rpc_inst->rx_msg_queue,
                        header_elem);

                    status = ezQueue_PushReservedElement(
                        &rpc_inst->rx_msg_queue,
                        payload_elem);

                    rpc_inst->deserializer_state = STATE_SOF;
                }
            }
            break;

        case STATE_CRC:
            TRACE("STATE_CRC");

            *(crc + rpc_inst->byte_count) = rx_byte;
            rpc_inst->byte_count++;

            if (rpc_inst->byte_count >= rpc_inst->crc_size)
            {
#if(DEBUG_LVL == LVL_TRACE)
                ezRpc_PrintCrc(crc, rpc_inst->crc_size);
#endif /* DEBUG_LVL == LVL_TRACE */
                
                if (rpc_inst->IsCrcCorrect)
                {
                    if (rpc_inst->IsCrcCorrect(payload, temp_header->payload_size, crc, rpc_inst->crc_size))
                    {
                        DEBUG("crc correct");
                        status = ezQueue_PushReservedElement(
                            &rpc_inst->rx_msg_queue,
                            header_elem);

                        status = ezQueue_PushReservedElement(
                            &rpc_inst->rx_msg_queue,
                            payload_elem);
                    }
                    else
                    {
                        DEBUG("crc wrong");
                        (void)ezQueue_ReleaseReservedElement(
                            &rpc_inst->rx_msg_queue,
                            header_elem);

                        (void)ezQueue_ReleaseReservedElement(
                            &rpc_inst->rx_msg_queue,
                            payload_elem);
                    }
                }

                (void)ezQueue_ReleaseReservedElement(
                    &rpc_inst->rx_msg_queue,
                    crc_elem);

                rpc_inst->deserializer_state = STATE_SOF;

            }
            break;
        
        default:
            rpc_inst->deserializer_state = STATE_SOF;
            break;
        }
    }
}

#if(DEBUG_LVL == LVL_TRACE)
/******************************************************************************
* Function : ezRpc_PrintHeader
*//**
* @Description: Print the header on the terminal
*
* @param    *header: (IN)pointer to rpc message header
* @return   None
*
*******************************************************************************/
void ezRpc_PrintHeader(struct ezRpcMsgHeader *header)
{
    if (header != NULL)
    {
        dbg_print("\nRPC header\n");
        dbg_print("------------------------\n");
        dbg_print("uuid:\t\t %d\n", header->uuid);

        if (header->type == RPC_MSG_REQ)
        {
            dbg_print("type:\t\t request\n");
        }
        else if (header->type == RPC_MSG_RESP)
        {
            dbg_print("type:\t\t response\n");
        }
        else
        {
            dbg_print("type:\t\t unknown\n");
        }

        dbg_print("tag:\t\t %d\n", header->tag);
        dbg_print("encrypt:\t %d\n", header->is_encrypted);
        dbg_print("size:\t\t %d\n", header->payload_size);
        dbg_print("\n");
    }
}


void ezRpc_PrintPayload(uint8_t *payload, uint32_t size)
{
    if (payload != NULL)
    {
        dbg_print("\nRPC payload\n");
        dbg_print("------------------------\n");
        HEXDUMP(payload, size);
        dbg_print("\n");
    }
}


void ezRpc_PrintCrc(uint8_t *crc, uint32_t size)
{
    if (crc != NULL)
    {
        dbg_print("\nRPC CRC\n");
        dbg_print("------------------------\n");
        HEXDUMP(crc, size);
        dbg_print("\n");
    }
}
#endif /* DEBUG_LVL == LVL_TRACE */

#endif /* CONFIG_RPC == 1U */
/* End of file*/
