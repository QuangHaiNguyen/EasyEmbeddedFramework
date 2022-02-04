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
    START_OF_FRAME = 0,     
    /**< This states looks for a sync character           */
    OP_CODE,            
    /**< This state receives the op code                  */
    DATA_LENGTH,        
    /**< This state receives the size of the data section */
    DATA,               
    /**< This state receives the packet data              */
    CHECKSUM,          
    /**< This state receives the checksum bytes           */
}BinaryParserState;


typedef enum
{
    PARSER_OK,
    /**< Status OK */
    PARSER_ERROR,
    /**< Status generic error */
    PARSER_WRONG_CODE,
    /**< Status wrong op code */
    PARSER_WRONG_SOF,
    /**< Status start of frame */
    PARSER_NO_LEN,
    /**< Status data length zero */
    PARSER_DATA_OVERFLOW,
    /**< data length is bigger than config length */
    PARSER_WRONG_CHECKSUM,
    /**< Status wrong check sum */
    PARSER_BUSY,
    /**< Status busy, or still processing the byte */
    PARSER_NO_COMMAND,
    /**< Status no command found */
}BIN_PARSER_STATUS;

typedef void(*CommandHandler)(void* payload, uint16_t payload_size_byte);

typedef struct
{
    uint8_t sof;
    uint8_t opcode;
    uint8_t encrypt_info;
    uint16_t payload_size_byte;
}BinaryFrameHeader;

typedef struct
{
    BinaryFrameHeader header;
    uint8_t *payload;
    uint8_t checksum[CRC_SIZE]
}BinaryFrame;

typedef struct
{
   uint8_t opcode;              /**< Stores the command code*/
   CommandHandler pfnHandler;   /**< pointer to function handling that command */
}Command;

#if 0
typedef struct
{
    bool bUseSof;
    /**< If a start of frame is used */
    bool bUseEncryption;
    /**< If encryption is used */
    bool bUseChecksum;
    /**< If checksum is used */
    uint8_t CommandTableSize;
    /**< Size of the command table, how many commands are there in total */
    Command * pstCommandTable;
    /**< Poiter to the command table */
    bool(*CrcVeryfy)(BinFrame * pstFrame);
    /**< Pointer to the CRC verification function */
    void(*CrcCalculate)(BinFrame * pstFrame);
    /**< Pointer to the CRC calculation function */
    ezmMemList stMemList;
    /**< Memory list, which holds command pending to be executed */
    BinaryParserState eBinState;
    /**< Store the state of the binary parser statemachine */
    BIN_PARSER_STATUS eStatus;
    /**< Stateus of the parser */
}BinCmdParser;
#endif


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
