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
#include "string.h"
#include "../ezmDebug/ezmDebug.h"
#include "../utilities/hexdump/hexdump.h"

#define VERBOSE     0U
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
static void ezmParser_RunCommand(struct BinCmdParser *parser);


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
bool ezmParser_Init(struct BinCmdParser *parser, uint8_t *buffer, uint16_t buffer_size_byte, StatusHandler handler)
{
    bool is_success = false;

    /* sanity check the parser*/
    if (NULL != parser && NULL != buffer && buffer_size_byte > 0 
        && NULL != parser->command_table && parser->command_table_size > 0)
    {
        is_success = true;
        parser->parser_state = START_OF_FRAME;
        parser->curr_frame = NULL;
        parser->StatusHandler = handler;
        parser->buff_index = 0U;
        is_success = is_success && ezmStcMem_InitMemList(&parser->memory_list, buffer, buffer_size_byte);
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
void ezmParser_RunBinParser(struct BinCmdParser* parser, uint8_t data_byte)
{
    switch ((BinaryParserState)parser->parser_state)
    {
    case START_OF_FRAME:
    {
        if (SOF == data_byte)
        {
            VERBOSEPRINT1("[sof = 0x%02x]", data_byte);
            parser->curr_frame = (struct BinaryFrame*)ezmStcMem_Malloc(&parser->memory_list, sizeof(struct BinaryFrame));
            if (parser->curr_frame)
            {
                parser->curr_frame->header.sof = data_byte;
                parser->parser_state = UUID;
                parser->buff_index = sizeof(uint32_t);
            }
            else
            {
                PRINT_ERR("[error = cannot allocate memory]");
                EXECUTE_STATUS_HANDLER(BINPARSER_MEM_ERR);
            }
        }
        else
        {
            PRINT_ERR("[error = wrong sof]");
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
            VERBOSEPRINT1("[uuid = 0x%02x]", parser->curr_frame->header.uuid);
        }
        break;
    }
    case OP_CODE:
    {
        HEADER(parser).opcode = data_byte;
        parser->parser_state = ENCRYPT_INFO;
        VERBOSEPRINT1("[opcode = 0x%02x]", data_byte);
        break;
    }
    case ENCRYPT_INFO:
    {
        HEADER(parser).encrypt_info = data_byte;
        parser->parser_state = DATA_LENGTH;
        parser->buff_index = sizeof(uint16_t);
        VERBOSEPRINT1("[encryption = 0x%02x]", data_byte);
        break;
    }
    case DATA_LENGTH:
    {
        *((uint8_t*)(&(HEADER(parser).payload_size_byte)) + --parser->buff_index) = data_byte;
        if (parser->buff_index == 0)
        {
            parser->curr_frame->payload = (uint8_t*)ezmStcMem_Malloc(&parser->memory_list, HEADER(parser).payload_size_byte);

            if (parser->curr_frame->payload)
            {
                parser->parser_state = DATA;
                VERBOSEPRINT1("[len = 0x%02x]", header->payload_size_byte);
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
                ezmParser_RunCommand(parser);
                CLEANUP();
            }
        }
        break;
    }
    case CHECKSUM:
    {
        *((uint8_t*)(&(parser->curr_frame->checksum)) + --parser->buff_index) = data_byte;
        if (parser->buff_index == 0)
        {
            if (parser->CrcVerify(parser->curr_frame))
            {
                ezmParser_RunCommand(parser);
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
static void ezmParser_RunCommand(struct BinCmdParser* parser)
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

#endif /* CONFIG_BIN_PARSER == 1U */
/* End of file*/