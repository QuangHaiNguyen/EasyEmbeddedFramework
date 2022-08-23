
/*******************************************************************************
* Filename:         app_embedded_emulator.c
* Author:           Hai Nguyen
* Original Date:    21.08.2022
* Last Update:      21.08.2022
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
* Written by Hai Nguyen 21.08.2022
*
*******************************************************************************/

/** @file   app_embedded_emulator.c
 *  @author Hai Nguyen
 *  @date   21.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "app_embedded_emulator.h"

#if (CONFIG_EMBEDDED_EMULATOR == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "mbed_emulator"       /**< module name */

#include <string.h>
#include "utilities/logging/logging.h"
#include "utilities/ez_queue/ez_queue.h"
#include "binCmdParser/binCmdParser.h"
#include "ezmDriver/driver.h"
#include "hal/uart/uart.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define PARSER_BUFF_SIZE    256    /**< size of the parser buffer */
#define QUEUE_BUFF_SIZE     256    /**< size of the queue buffer */


/******************************************************************************
* Module Typedefs
*******************************************************************************/


/** @brief state of the emulator state machine
 *
 */
typedef enum
{
    STATE_TX,       /**< State transmiting data */
    STATE_RX,       /**< State receiving and handle receive data */
    STATE_ERROR,    /**< State handling error */
}EMULATOR_STATE;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint8_t              parser_buff[PARSER_BUFF_SIZE] = {0};
static uint8_t              queue_buff[QUEUE_BUFF_SIZE] = {0};
static struct BinCmdParser  parser;
static ezQueue              tx_queue;
static bool                 emulator_started = false;
static UartDrvApi           *virtual_com_drv;
static EMULATOR_STATE       emulator_state = STATE_TX;


/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezMbedEmulator_ParserStatusHandlerCallback(BinParserStatus status);
static ezSTATUS ezMbedEmulator_AddDataFrameToQueue(uint8_t opcode,
                                                    void* data,
                                                    uint32_t data_size);

static void ezMbedEmulator_HandleEchoOpcode(void* payload, uint16_t payload_size);


/** @brief lookup table for opcode and command handler
 *
 */
static struct Command CommandTable[1] = {
    {OPCODE_ECHO, ezMbedEmulator_HandleEchoOpcode},
};


/*******************************************************************************
* External functions
*******************************************************************************/
ezSTATUS ezMbedEmulator_Initialization(void)
{
    bool success = true;
    ezSTATUS status = ezSUCCESS;
    TRACE("ezMbedEmulator_Initialization()");

    parser.command_table = CommandTable;
    parser.command_table_size = sizeof(CommandTable) / sizeof(struct Command);

    if (ezParser_Init(&parser, parser_buff, PARSER_BUFF_SIZE,
                        ezMbedEmulator_ParserStatusHandlerCallback))
    {
        status = ezQueue_CreateQueue(&tx_queue, queue_buff, QUEUE_BUFF_SIZE);

        if (status == ezFAIL)
        {
            DEBUG("cannot init queue");
        }
    }
    else
    {
        DEBUG("Cannot init parser");
        status = ezFAIL;
    }

    if (status == ezSUCCESS)
    {
        if (ezmDriver_IsDriverBusy(VIRTUAL_COM_DRIVER) == false)
        {
            ezmDriver_GetDriverInstance(VIRTUAL_COM_DRIVER, &virtual_com_drv);
            if (virtual_com_drv == NULL)
            {
                status = ezFAIL;
                DEBUG("virtual comm is blocked by other module, fail");
            }
            else
            {
                UartConfiguration virtual_com_config;

                virtual_com_config.baudrate = 1500000;
                virtual_com_config.byte_size = 8;
                virtual_com_config.parity = NONE;
                virtual_com_config.port_name = "COM15";
                virtual_com_config.stop_bit = ONE_BIT;

                virtual_com_drv->ezmUart_Configure(&virtual_com_config);
            }
        }
        else
        {
            status = ezFAIL;
            DEBUG("virtual comm is busy, fail");
        }
    }

    if (status == ezSUCCESS)
    {
        emulator_started = true;
        DEBUG("emulator initialized successfully");
    }

    return ezSUCCESS;
}


void ezMbedEmulator_Run(void)
{
    ezSTATUS status = ezSUCCESS;

    if (emulator_started)
    {
        switch (emulator_state)
        {
        case STATE_TX:
            if (ezQueue_GetNumOfElement(&tx_queue) > 0)
            {
                uint8_t* data = NULL;
                uint32_t data_size = 0;

                status = ezQueue_GetFront(&tx_queue, &data, &data_size);
                if (status == ezSUCCESS)
                {
                    virtual_com_drv->ezmUart_SendBlocking(data, data_size);
                    status = ezQueue_Pop(&tx_queue);
                }
            }
            emulator_state = STATE_RX;
            break;

        case STATE_RX:
        {
            uint8_t one_byte;
            if (virtual_com_drv->ezmUart_ReceiveBlocking(&one_byte, 1) == 1)
            {
                TRACE("byte = 0x%x", one_byte);
                ezParser_RunBinParser(&parser, one_byte);
            }
            emulator_state = STATE_TX;
            break;
        }

        case STATE_ERROR:
            emulator_state = STATE_RX;
            break;

        default:
            emulator_state = STATE_RX;
            break;
        }
    }
}


ezSTATUS ezMbedEmulator_SendData(uint8_t opcode, void* data, uint32_t data_size)
{
    TRACE("ezMbedEmulator_AddDataFrameToQueue(opcode = 0x%x, @ = %p, size = %lu);",
                     opcode, data, data_size);
    return ezMbedEmulator_AddDataFrameToQueue(opcode, data, data_size);
}


ezSTATUS ezMbedEmulator_GetQueueElement(void **queue_buffer, uint32_t data_size)
{
    return ezQueue_ReserveElement(&tx_queue, queue_buffer, data_size);
}


/******************************************************************************
* Internal functions
*******************************************************************************/


/******************************************************************************
* Function : ezMbedEmulator_ParserStatusHandlerCallback
*//**
* @Description:
*
* A callback to handle the status of the parser
*
* @param    status: parser status
* @return   None
*
*******************************************************************************/
static void ezMbedEmulator_ParserStatusHandlerCallback(BinParserStatus status)
{
}


/******************************************************************************
* Function : ezMbedEmulator_AddDataFrameToQueue
*//**
* @Description:
*
* Serialize and add the data to the transmit queue
*
* @param    opcode: opcode of the data
* @param    *data: pointer to the data
* @param    data_size: size of the data
* @return   ezSUCCESS: success
*           ezFAIL: fail
*
*******************************************************************************/
static ezSTATUS ezMbedEmulator_AddDataFrameToQueue(uint8_t opcode,
                                                    void *data,
                                                    uint32_t data_size)
{
    ezSTATUS status = ezSUCCESS;
    uint32_t frame_size = data_size + FRAME_HEADER_SIZE + CONFIG_CRC_SIZE;
    uint8_t *reserve_data = NULL;

    TRACE("ezMbedEmulator_AddDataFrameToQueue()");

    status = ezQueue_ReserveElement(&tx_queue, &(void*)reserve_data, frame_size);

    if (status == ezSUCCESS && reserve_data != NULL)
    {
        /* refactor to a separated function */

        /* sof */
        reserve_data[0] = 0x80;
        reserve_data++;
        
        /* uuid */
        *(uint32_t*)reserve_data = 0x00U;
        reserve_data += UUID_SIZE;
        
        /* opcode */
        *reserve_data = opcode;
        reserve_data++;
        
        /* encrypt */
        *reserve_data = 0x00U;
        reserve_data++;

        /* payload size */
        *reserve_data = (uint16_t)data_size >> 8;
        reserve_data++;
        *reserve_data = (uint16_t)data_size & 0xFF;
        reserve_data++;

        /* the real payload */
        memcpy((void*)reserve_data, data, data_size);

        /* calculate crc if applied */

        DEBUG("Add data to frame success");

#if (DEBUG_LVL == LVL_TRACE)
        uint32_t read_data_size = 0U;
        ezQueue_GetFront(&tx_queue, &(void*)reserve_data, &read_data_size);

        TRACE("TX frame:");
        HEXDUMP(reserve_data, read_data_size);
#endif /* DEBUG_LVL == LVL_TRACE */
    }

    return status;
}


/******************************************************************************
* Function : ezMbedEmulator_HandleEchoOpcode
*//**
* @Description:
*
* Function to handle the echo command, do nothing but print the payload
*
* @param    *payload: pointer to the payload 
* @param    payload_size: size of the payload
* @return   None
*
*******************************************************************************/
static void ezMbedEmulator_HandleEchoOpcode(void* payload, uint16_t payload_size)
{
    INFO("echo data:");
    HEXDUMP(payload, payload_size);
}

#endif /* CONFIG_EMBEDDED_EMULATOR */
/* End of file*/

