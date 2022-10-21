/*******************************************************************************
* Title                 :   binCmdParser 
* Filename              :   binCmdParser.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
*******************************************************************************/

/** @file  binCmdParser.c
 *  @brief This is the source template for a binCmdParser
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "binCmdParser.h"

#if (CONFIG_BIN_PARSER == 1U)

#define DEBUG_LVL   LVL_TRACE               /**< logging level */
#define MOD_NAME    "bin_cmd_parser"        /**< module name */

#include "string.h"
#include "utilities/logging/logging.h"
#include "../ezmDebug/ezmDebug.h"
#include "../utilities/hexdump/hexdump.h"


#define SOF         0x80U
#define HEADER(parser)  (parser->curr_frame->header)
#define CLEANUP()   {\
                        (void)ezmStcMem_Free(&parser->memory_list, parser->curr_frame->payload);\
                        (void)ezmStcMem_Free(&parser->memory_list, parser->curr_frame);\
                        parser->curr_frame = NULL;\
                        parser->parser_state = START_OF_FRAME;\
                        parser->buff_index = 0U;\
                    }

#define EXECUTE_STATUS_HANDLER(status)  if (parser->StatusHandler){parser->StatusHandler(status);}
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/**
 * brief:   Defines the each state required to receive a complete packet
 */
typedef enum
{
    START_OF_FRAME = 0, /**< This states looks for a sync character */
    UUID,               /**< This state parses the uuid */
    OP_CODE,            /**< This state parses the opcode */
    ENCRYPT_INFO,       /**< This state parses the encryption info*/
    DATA_LENGTH,        /**< This state receives the size of the data section */
    DATA,               /**< This state receives the packet data */
    CHECKSUM,           /**< This state receives the checksum bytes */
}BinaryParserState;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezParser_RunCommand(struct BinCmdParser *parser);


ezSTATUS ezParser_Init(struct BinCmdParser *parser,
                        uint8_t *buffer,
                        uint16_t buffer_size_byte,
                        StatusHandler handler)
{
    ezSTATUS status = ezFAIL;
    TRACE("ezmParser_Init()");

    /* sanity check the parser*/
    if (NULL != parser && NULL != buffer && buffer_size_byte > 0 
        && NULL != parser->command_table && parser->command_table_size > 0)
    {
        status = ezSUCCESS;
        parser->parser_state = START_OF_FRAME;
        parser->curr_frame = NULL;
        parser->StatusHandler = handler;
        parser->buff_index = 0U;
        if (ezmStcMem_InitMemList(&parser->memory_list, buffer, buffer_size_byte) == true)
        {
            status = ezSUCCESS;
            DEBUG("init success");
        }
        else
        {
            status = ezFAIL;
            DEBUG("init fail");
        }
    }

    return status;
}


void ezParser_RunBinParser(struct BinCmdParser* parser, uint8_t data_byte)
{
    switch ((BinaryParserState)parser->parser_state)
    {
    case START_OF_FRAME:
    {
        if (SOF == data_byte)
        {
            TRACE("[sof = 0x%02x]", data_byte);
            parser->curr_frame = (struct BinaryFrame*)ezmStcMem_Malloc(&parser->memory_list,
                                                                        sizeof(struct BinaryFrame));
            if (parser->curr_frame)
            {
                parser->curr_frame->header.sof = data_byte;
                parser->parser_state = UUID;
                parser->buff_index = sizeof(uint32_t);
            }
            else
            {
                DEBUG("[error = cannot allocate memory]");
                EXECUTE_STATUS_HANDLER(BINPARSER_MEM_ERR);
            }
        }
        else
        {
            DEBUG("[error = wrong sof]");
            EXECUTE_STATUS_HANDLER(BINPARSER_FORMAT_ERR);
        }

        break;
    }
    case UUID:
    {
        *((uint8_t*)(&(HEADER(parser).uuid)) + --parser->buff_index) = data_byte;
        if (parser->buff_index == 0)
        {
            parser->parser_state = OP_CODE;
            TRACE("[uuid = 0x%lx]", parser->curr_frame->header.uuid);
        }
        break;
    }
    case OP_CODE:
    {
        HEADER(parser).opcode = data_byte;
        parser->parser_state = ENCRYPT_INFO;
        TRACE("[opcode = 0x%02x]", data_byte);
        break;
    }
    case ENCRYPT_INFO:
    {
        HEADER(parser).encrypt_info = data_byte;
        parser->parser_state = DATA_LENGTH;
        parser->buff_index = sizeof(uint16_t);
        TRACE("[encryption = 0x%02x]", data_byte);
        break;
    }
    case DATA_LENGTH:
    {
        *((uint8_t*)(&(HEADER(parser).payload_size_byte)) + --parser->buff_index) = data_byte;
        if (parser->buff_index == 0)
        {
            parser->curr_frame->payload = (uint8_t*)ezmStcMem_Malloc(&parser->memory_list,
                                                                    HEADER(parser).payload_size_byte);

            if (parser->curr_frame->payload)
            {
                parser->parser_state = DATA;
                TRACE("[len = 0x%02x]", HEADER(parser).payload_size_byte);
            }
            else
            {
                EXECUTE_STATUS_HANDLER(BINPARSER_MEM_ERR);
                CLEANUP();
            }
        }
        break;
    }
    case DATA:
    {
        parser->curr_frame->payload[parser->buff_index++] = data_byte;
        if (parser->buff_index >= HEADER(parser).payload_size_byte)
        {
            if (HEADER(parser).encrypt_info > 0U)
            {
                /* decrypt frame before moving on */
            }

            HEXDUMP(parser->curr_frame->payload, HEADER(parser).payload_size_byte);

            if (parser->CrcVerify)
            {
                parser->buff_index = CONFIG_CRC_SIZE;
                parser->parser_state = CHECKSUM;
            }
            else
            {
                ezParser_RunCommand(parser);
                CLEANUP();
            }
        }
        break;
    }
    case CHECKSUM:
    {
        TRACE("[checksum = 0x%02x]", data_byte);
        *((uint8_t*)(&(parser->curr_frame->checksum)) + --parser->buff_index) = data_byte;
        if (parser->buff_index == 0)
        {
            if (parser->CrcVerify(parser->curr_frame))
            {
                ezParser_RunCommand(parser);
            }
            else
            {
                EXECUTE_STATUS_HANDLER(BINPARSER_CRC_ERR);
            }
            CLEANUP();
        }
        break;
    }
    default:
        CLEANUP();
        break;
    }
}


uint8_t* ezParser_SerializeHeader(struct BinaryHeader *header, uint8_t* buff, uint32_t buff_size)
{
    uint8_t *ret_ptr = NULL;

    if (header != NULL && buff != NULL && buff_size >= FRAME_HEADER_SIZE)
    {
        ret_ptr = buff;

        /* sof */
        *ret_ptr = header->sof;
        ret_ptr++;

        /* uuid */
        *(uint32_t*)ret_ptr = header->uuid;
        ret_ptr += UUID_SIZE;

        /* opcode */
        *ret_ptr = header->opcode;
        ret_ptr++;

        /* encrypt */
        *ret_ptr = header->encrypt_info;
        ret_ptr++;

        /* payload size */
        *ret_ptr = header->payload_size_byte >> 8;
        ret_ptr++;
        *ret_ptr = header->payload_size_byte & 0xFF;
        ret_ptr++;
    }

    return ret_ptr;
}


/******************************************************************************
* Function : ezParser_RunCommand
*
* This function check the opcode in the lookup table and execute the command
*
* PRE-CONDITION: parser must be initialized first
*
* POST-CONDITION: None
*
* @param    *parser:    (IN)pointer to the parser structure
*
* @return   None
*
*******************************************************************************/
static void ezParser_RunCommand(struct BinCmdParser* parser)
{
    if (parser->curr_frame && parser->curr_frame->payload)
    {
        for (uint8_t i = 0; i < parser->command_table_size; i++)
        {
            if (parser->curr_frame->header.opcode == parser->command_table[i].opcode)
            {
                DEBUG("Command found, [opcode = 0x%02x]", parser->command_table[i].opcode);
                parser->command_table[i].pfnHandler(parser->curr_frame->payload,
                                                    parser->curr_frame->header.payload_size_byte);

                if (parser->StatusHandler)
                {
                    parser->StatusHandler(BINPARSER_OK);
                }
                break;
            }
            else
            {
                if (i == parser->command_table_size - 1U)
                {
                    /* End of table */
                    DEBUG("No command found");

                    if (parser->StatusHandler)
                    {
                        parser->StatusHandler(BINPARSER_CMD_NOT_FOUND);
                    }
                }
            }
        }
    }
    else
    {
        PRINT_ERR("Frame cannot be found");
        if (parser->StatusHandler)
        {
            parser->StatusHandler(BINPARSER_PAYLOAD_ERR);
        }
    }
}

#endif /* CONFIG_BIN_PARSER == 1U */
/* End of file*/
