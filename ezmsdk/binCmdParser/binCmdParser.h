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
#include "../app/app_config.h"

#if (BIN_PARSER == 1U)
#include "stdint.h"
#include "stdbool.h"

#if(STCMEM == 1U)
#include "../helper/stcmem/stcmem.h"
#else
#error STCMEM must be activated
#endif /* STCMEM */

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/*Work around to prevent compile error, the definition locates in app_conf.h*/
#ifndef PAYLOAD_MAX_SIZE
#define PAYLOAD_MAX_SIZE        0U
#endif /* PAYLOAD_MAX_SIZE */

/*Work around to prevent compile error, the definition locates in app_conf.h*/
#ifndef CRC_SIZE
#define CRC_SIZE                0U
#endif /* CRC_SIZE */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/**
 * brief:   Defines the each state required to receive a complete packet
 */
typedef enum
{
    START_OF_FRAME = 0, /**< This states looks for a sync character */
    OP_CODE,            /**< This state receives the op code */
    ENCRYPT_INFO,       /**< */
    DATA_LENGTH,        /**< This state receives the size of the data section */
    DATA,               /**< This state receives the packet data */
    CHECKSUM,           /**< This state receives the checksum bytes */
}BinaryParserState;


/**
 * brief:
 */
typedef struct
{
    uint8_t sof;                /**< */
    uint8_t opcode;             /**< */
    uint8_t encrypt_info;       /**< */
    uint16_t payload_size_byte; /**< */
}BinaryFrameHeader;

/**
 * brief:
 */
typedef struct
{
    BinaryFrameHeader   header;             /**< */
    uint8_t             *payload;           /**< */
    uint8_t             checksum[CRC_SIZE]; /**< */
}BinaryFrame;

typedef void(*CommandHandler)(void *payload, uint16_t payload_size_byte);
typedef bool(*CrcVerify) (BinaryFrame *bin_frame);
typedef void(*CrcCalculate)(BinaryFrame *bin_frame);

typedef struct
{
   uint8_t opcode;              /**< Stores the command code*/
   CommandHandler pfnHandler;   /**< pointer to function handling that command */
}Command;


typedef struct
{
    uint8_t             command_table_size; /**< Size of the command table, how many commands are there in total */
    Command             *ptr_command_table; /**< Poiter to the command table */
    CrcVerify           pfn_CrcVerify;      /**< Pointer to the CRC verification function */
    CrcCalculate        pfn_CrcCalculate;   /**< Pointer to the CRC calculation function */
    ezmMemList          stMemList;          /**< Memory list, which holds command pending to be executed */
    uint8_t             eBinState;          /**< Store the state of the binary parser statemachine */
}BinCmdParser;



/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/

#if 0
void ezmParser_Init(BinCmdParser * pstParser, uint8_t u8ModuleId);
void ezmParser_RunBinParser(BinCmdParser * pstParser, uint8_t u8Byte);
void ezmParser_RunCmdParser(BinCmdParser * pstParser);
#endif
#endif /* BIN_PARSER */
#endif /* _BIN_PARSER_H */
/* End of file*/
