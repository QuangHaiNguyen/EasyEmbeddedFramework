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

#if (BIN_PARSER == 1U)
#include "string.h"
#include "../ezmDebug/ezmDebug.h"
#include "../helper/hexdump/hexdump.h"

#define VERBOSE     1U
#define MOD_ID      "BIN PARSER"
#if (MODULE_DEBUG == 1U) && (PARSER_DEBUG == 1U)
    #define PARSERPRINT(a)              PRINT_DEBUG(MOD_ID,a)
    #define PARSERPRINT1(a,b)           PRINT_DEBUG1(MOD_ID,a,b)
    #define PARSERPRINT2(a,b,c)         PRINT_DEBUG2(MOD_ID,a,b,c)
    #define PARSERPRINT3(a,b,c,d)       PRINT_DEBUG3(MOD_ID,a,b,c,d)
    #define PARSERPRINT4(a,b,c,d,e)     PRINT_DEBUG4(MOD_ID,a,b,c,d,e)
#else 
    #define PARSERPRINT(a)
    #define PARSERPRINT1(a,b)
    #define PARSERPRINT2(a,b,c)
    #define PARSERPRINT3(a,b,c,d)
    #define PARSERPRINT4(a,b,c,d,e)
#endif

#if (MODULE_DEBUG == 1U) && (VERBOSE == 1U)
    #define VERBOSEPRINT(a)            PARSERPRINT(a)
    #define VERBOSEPRINT1(a,b)         PARSERPRINT1(a,b)
    #define VERBOSEPRINT2(a,b,c)       PARSERPRINT2(a,b,c)
    #define VERBOSEPRINT3(a,b,c,d)     PARSERPRINT3(a,b,c,d)
    #define VERBOSEPRINT4(a,b,c,d,e)   PARSERPRINT4(a,b,c,d,e)
    #define HEXDUMP(a,b)                ezmHexdump(a,b)
#else 
    #define VERBOSEPRINT(a)
    #define VERBOSEPRINT1(a,b)
    #define VERBOSEPRINT2(a,b,c)
    #define VERBOSEPRINT3(a,b,c,d)
    #define VERBOSEPRINT4(a,b,c,d,e)
    #define HEXDUMP(a,b)
#endif

#define SOF         0x80U


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
    COMPLETE,           /**< Complete state, execute the command */
}BinaryParserState;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezmParser_RunCommand(BinCmdParser *parser);


/******************************************************************************
* Function : ezmParser_Init
*
* This function initializes a parser
*
* PRE-CONDITION: Static memory module must be initialized first
*
* POST-CONDITION: None
*
* @param    *parser:            (IN)pointer to the parser structure
* @param    *buffer:            (IN)pointer to the buffer, which parser will use to run
* @param    buffer_size_byte:   (IN)size of the buffer, in byte
* @param    handler:            (IN)handler to handle the status of the parser
*
* @return   true if initialization success
*
*******************************************************************************/
bool ezmParser_Init(BinCmdParser *parser, uint8_t *buffer, uint16_t buffer_size_byte, StatusHandler handler)
{
    bool is_success = true;

    /* sanity check the parser*/
    if (NULL == parser || NULL == buffer || 0U == buffer_size_byte)
    {
        is_success = false;
    }

    if (is_success)
    {
        if (NULL == parser->command_table || 0U == parser->command_table_size)
        {
            is_success = false;
        }
    }

    if (is_success)
    {
        parser->parser_state = START_OF_FRAME;
        parser->curr_frame = NULL;
        parser->StatusHandler = handler;
        is_success = is_success & ezmStcMem_InitMemList(&parser->memory_list, buffer, buffer_size_byte);
    }

    return is_success;
}

/******************************************************************************
* Function : ezmParser_RunBinParser
*
* This function initializes a parser
*
* PRE-CONDITION: parser must be initialized first
*
* POST-CONDITION: None
*
* @param    *parser:    (IN)pointer to the parser structure
* @param    data_byte:  (IN)data byte feed to the parser
*
* @return   None
*
*******************************************************************************/
void ezmParser_RunBinParser(BinCmdParser* parser, uint8_t data_byte)
{
    static uint8_t buff_index = 0U;
    BinaryFrameHeader *header;

    switch ((BinaryParserState)parser->parser_state)
    {
    case START_OF_FRAME:
    {
        if (SOF == data_byte)
        {
            VERBOSEPRINT1("[sof = 0x%02x]", data_byte);
            parser->curr_frame = (BinaryFrame*)ezmStcMem_Malloc(&parser->memory_list, sizeof(BinaryFrame));
            memset(parser->curr_frame, 0, sizeof(BinaryFrame));

            if (parser->curr_frame)
            {
                parser->curr_frame->header.sof = data_byte;
                parser->parser_state = UUID;
            }
            else
            {
                if (parser->StatusHandler)
                {
                    parser->StatusHandler(BINPARSER_MEM_ERR);
                }

                parser->parser_state = START_OF_FRAME;
                ezmStcMem_Free(&parser->memory_list, parser->curr_frame);
                parser->curr_frame = NULL;
                PRINT_ERR("[error = cannot allocate memory]");
            }
        }
        else
        {
            if (parser->StatusHandler)
            {
                parser->StatusHandler(BINPARSER_FORMAT_ERR);
            }

            parser->parser_state = START_OF_FRAME;
            ezmStcMem_Free(&parser->memory_list, parser->curr_frame);
            parser->curr_frame = NULL;
            PRINT_ERR("[error = Wwrong opcode]");
        }
        break;
    }
    case UUID:
    {
        header = &parser->curr_frame->header;
        if (0 == buff_index)
        {
            header->uuid = data_byte << 24;
            buff_index++;
        }
        else if (1 == buff_index)
        {
            header->uuid = header->uuid | (data_byte << 16);
            buff_index++;
        }
        else if (2 == buff_index)
        {
            header->uuid = header->uuid | (data_byte << 8);
            buff_index++;
        }
        else
        {
            header->uuid = header->uuid | data_byte;
            buff_index = 0U;

            VERBOSEPRINT1("[uuid = 0x%02x]", parser->curr_frame->header.uuid);
            parser->parser_state = OP_CODE;
        }
        break;
    }
    case OP_CODE:
    {
        header = &parser->curr_frame->header;
        header->opcode = data_byte;
        parser->parser_state = ENCRYPT_INFO;
        VERBOSEPRINT1("[opcode = 0x%02x]", data_byte);
        break;
    }
    case ENCRYPT_INFO:
    {
        header = &parser->curr_frame->header;
        header->encrypt_info = data_byte;
        parser->parser_state = DATA_LENGTH;
        VERBOSEPRINT1("[encryption = 0x%02x]", data_byte);
        break;
    }
    case DATA_LENGTH:
    {
        header = &parser->curr_frame->header;
        if (0 == buff_index)
        {
            header->payload_size_byte = data_byte << 8;
            buff_index++;
        }
        else
        {
            header->payload_size_byte |= data_byte ;
            parser->curr_frame->payload = (uint8_t*)ezmStcMem_Malloc(&parser->memory_list, header->payload_size_byte);
            parser->parser_state = DATA;
            buff_index = 0;
            VERBOSEPRINT1("[len = 0x%02x]", header->payload_size_byte);
        }
        break;
    }
    case DATA:
    {
        header = &parser->curr_frame->header;

        parser->curr_frame->payload[buff_index] = data_byte;
        buff_index++;
        if (buff_index >= header->payload_size_byte)
        {
            parser->parser_state = CHECKSUM;
            buff_index = 0U;

            if (header->encrypt_info > 0U)
            {
                /* decrypt frame before moving on */
            }

            HEXDUMP(parser->curr_frame->payload, header->payload_size_byte);
        }
        break;
    }
    case CHECKSUM:
    {
        if (parser->CrcVerify)
        {
            parser->curr_frame->checksum[buff_index] = buff_index;
            buff_index++;
            if (buff_index >= CRC_SIZE)
            {
                if (parser->CrcVerify(parser->curr_frame))
                {
                    parser->parser_state = COMPLETE;
                    buff_index = 0U;
                    VERBOSEPRINT("[CRC = OK]");
                }
                else
                {
                    if (parser->StatusHandler)
                    {
                        parser->StatusHandler(BINPARSER_CRC_ERR);
                    }

                    ezmStcMem_Free(&parser->memory_list, parser->curr_frame->payload);
                    ezmStcMem_Free(&parser->memory_list, parser->curr_frame);
                    parser->curr_frame = NULL;
                    parser->parser_state = START_OF_FRAME;
                    buff_index = 0U;
                    PRINT_ERR("[CRC fail]");
                }
            }
        }
        else
        {
            parser->parser_state = COMPLETE;
        }
        break;
    }
    case COMPLETE:
    {
        ezmParser_RunCommand(parser);
        ezmStcMem_Free(&parser->memory_list, parser->curr_frame->payload);
        ezmStcMem_Free(&parser->memory_list, parser->curr_frame);
        parser->curr_frame = NULL;
        parser->parser_state = START_OF_FRAME;
        break;
    }
    default:
    {
        break;
    }
    }
}

/******************************************************************************
* Function : ezmParser_RunCommand
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
static void ezmParser_RunCommand(BinCmdParser* parser)
{
    if (parser->curr_frame && parser->curr_frame->payload)
    {
        for (uint8_t i = 0; i < parser->command_table_size; i++)
        {
            if (parser->curr_frame->header.opcode == parser->command_table[i].opcode)
            {
                PARSERPRINT1("Command found, [opcode = 0x%02x]", parser->command_table[i].opcode);
                parser->command_table[i].pfnHandler(parser->curr_frame->payload, parser->curr_frame->header.payload_size_byte);

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
                    PRINT_ERR("No command found");

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

#endif /* BIN_PARSER == 1U */
/* End of file*/