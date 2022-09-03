
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

#include <string.h>


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define RPC_HEADER_SIZE     12      /**< size of the RPC message header in bytes*/
#define SOF                 0x80    /**< start of frame, for syncronisation /

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */


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

        status = ezQueue_CreateQueue(&rpc_inst->msg_queue, buff, buff_size);

        if (status == ezFAIL)
        {
            ERROR("cannot create queue instance");
        }
        else
        {
            rpc_inst->service_table = service_table;
            rpc_inst->service_table_size = service_table_size;
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
        rpc_inst->CrcVerify = verify_func;
        rpc_inst->CrcCalculate = cal_func;
    }
    else
    {
        status = ezFAIL;
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
            status = ezQueue_ReserveElement(&rpc_inst->msg_queue, 
                                            &buff, alloc_size);

            if (status == ezSUCCESS)
            {
                temp_header.tag = tag;
                temp_header.type = type;
                temp_header.payload_size = payload_size;
                temp_header.uuid = rpc_inst->next_uuid++;
                temp_header.is_encrypted = rpc_inst->is_encrypted;

                status = ezRpc_SerializeRpcHeader(buff,
                                                  alloc_size,
                                                  &temp_header);

                DEBUG("header value:");
                HEXDUMP(buff, RPC_HEADER_SIZE);
            }
            else
            {
                record->is_available = false;
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
    }

    return status;
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
            memset(&records[i], 0, sizeof(struct ezRpcRequestRecord));
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
        && rpc_inst->CrcVerify != NULL);
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


#endif /* CONFIG_RPC == 1U */
/* End of file*/
