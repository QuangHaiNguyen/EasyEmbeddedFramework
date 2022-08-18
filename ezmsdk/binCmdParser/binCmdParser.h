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
#ifndef _BIN_PARSER_H
#define _BIN_PARSER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (CONFIG_BIN_PARSER == 1U)
#include "stdint.h"
#include "stdbool.h"

#if(CONFIG_STCMEM == 1U)
#include "utilities/stcmem/stcmem.h"
#else
#error STCMEM must be activated
#endif /* STCMEM */

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#ifndef CONFIG_CRC_SIZE
#define CONFIG_CRC_SIZE     2U
#endif /* CONFIG_CRC_SIZE */

#ifndef CONFIG_PAYLOAD_MAX_SIZE
define CONFIG_PAYLOAD_MAX_SIZE  32U
#endif /* CONFIG_PAYLOAD_MAX_SIZE */

/******************************************************************************
* Module Typedefs
*******************************************************************************/


/**
 * brief:
 */
struct BinaryHeader
{
    uint8_t  sof;               /**< Start of frame, for syncronization */
    uint32_t uuid;              /**< Unique ID */
    uint8_t  opcode;            /**< Opcode */
    uint8_t  encrypt_info;      /**< Store encryption info, future feature*/
    uint16_t payload_size_byte; /**< size of payload, in byte*/
};

/**
 * brief:
 */
struct BinaryFrame
{
    struct BinaryHeader header;             /**< header of the frame */
    uint8_t             *payload;           /**< pointer to the payload */
    uint8_t             checksum[CONFIG_CRC_SIZE]; /**< buffer to store checksum */
};

/**
 * brief:
 */
typedef enum
{
    BINPARSER_OK,               /**< Command is executed successfully */
    BINPARSER_FORMAT_ERR,       /**< Frame has wrong format */
    BINPARSER_CMD_NOT_FOUND,    /**< Commadn not found */
    BINPARSER_CRC_ERR,          /**< CRC check error */
    BINPARSER_PAYLOAD_ERR,      /**< Payload error, NULL payload */
    BINPARSER_MEM_ERR,          /**< Memory error, cannot allocate memory for the new packet */
}BinParserStatus;

typedef void(*CommandHandler)   (void *payload, uint16_t payload_size_byte);
typedef bool(*CrcVerify)        (struct BinaryFrame *frame);
typedef void(*CrcCalculate)     (struct BinaryFrame *frame);
typedef void(*StatusHandler)    (BinParserStatus status);

struct Command
{
   uint8_t          opcode;       /**< Stores the command code*/
   CommandHandler   pfnHandler;   /**< pointer to function handling that command */
};


struct BinCmdParser
{
    uint8_t             command_table_size; /**< Size of the command table, how many commands are there in total */
    struct Command      *command_table;     /**< Poiter to the command table */
    CrcVerify           CrcVerify;          /**< Pointer to the CRC verification function */
    CrcCalculate        CrcCalculate;       /**< Pointer to the CRC calculation function */
    StatusHandler       StatusHandler;      /**< Callback to handle status (BinParserStatus), not required to use */
    ezmMemList          memory_list;        /**< Memory list, which holds command pending to be executed */
    uint8_t             parser_state;       /**< Store the state of the binary parser statemachine */
    struct BinaryFrame  *curr_frame;        /**< pointer to the current frame that the parser is working*/
    uint16_t            buff_index;         /**< index of the payload of current frame */
};



/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/

bool ezmParser_Init         (struct BinCmdParser* parser, uint8_t *buffer, uint16_t buffer_size_byte, StatusHandler handler);
void ezmParser_RunBinParser (struct BinCmdParser* parser, uint8_t data_byte);

#endif /* CONFIG_BIN_PARSER */
#endif /* _BIN_PARSER_H */
/* End of file*/
