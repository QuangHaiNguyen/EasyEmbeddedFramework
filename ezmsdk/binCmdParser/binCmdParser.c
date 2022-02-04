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
#else 
    #define VERBOSEPRINT(a)
    #define VERBOSEPRINT1(a,b)
    #define VERBOSEPRINT2(a,b,c)
    #define VERBOSEPRINT3(a,b,c,d)
    #define VERBOSEPRINT4(a,b,c,d,e)
#endif

#define VERBOSE                 0x00U

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
/* None */

/******************************************************************************
* Function : ezmParser_Init
*
* This function initializes a parser
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    pstParser: (IN)pointer to the parser
* @return   None
*
*
*******************************************************************************/


#if 0
/******************************************************************************
* Function : ezmParser_Init
*//** 
*
* This function initializes a parser
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    pstParser: (IN)pointer to the parser
* @param    u8ModuleId: (IN)module id of the owner of the parser
* @return   None
*
* \b Example Example:
* @code
* BinCmdParser stParser;
*
* stParser.bUseSof = true;
* stParser.bUseChecksum = true;
* stParser.bUseEncryption = false;
* stParser.bUseChecksum = true;
* stParser.CommandTableSize = 4U;
* stParser.pstCommandTable = CommandTable;
* stParser.CrcCalculate = &CrCCalculate;
* stParser.CrcVeryfy = &CrCVerify;
*
* ezmParser_Init(&stParser, 0xFF);
* @endcode
*
*******************************************************************************/
void ezmParser_Init(BinCmdParser * pstParser, uint8_t u8ModuleId)
{
    PARSERPRINT1("Init bin parser for module 0x%02x", u8ModuleId);
    ezmSmalloc_InitMemList(&pstParser->stMemList, u8ModuleId);
    pstParser->eBinState = START_OF_FRAME;
    pstParser->eStatus = PARSER_OK;
}

/******************************************************************************
* Function : ezmParser_RunCmdParser
*//** 
*
* This function runs the command parser, which checks the command queue and
* executes any commands pending there 
*
* PRE-CONDITION: ezmParser_Init
*
* POST-CONDITION: None
* 
* @param    pstParser: (IN)pointer to the parser
* @return   None
*
* \b Example Example:
* @code
* BinCmdParser stParser;
*
* stParser.bUseSof = true;
* stParser.bUseChecksum = true;
* stParser.bUseEncryption = false;
* stParser.bUseChecksum = true;
* stParser.CommandTableSize = 4U;
* stParser.pstCommandTable = CommandTable;
* stParser.CrcCalculate = &CrCCalculate;
* stParser.CrcVeryfy = &CrCVerify;
*
* ezmParser_Init(&stParser, 0xFF);
* ezmParser_RunCmdParser(&stParser);
* @endcode
*
* @see ezmParser_Init
*
*******************************************************************************/
void ezmParser_RunCmdParser(BinCmdParser * pstParser)
{
    ezmMemoryBlock * pstBlock;
    BinFrame * pstFrame;
    pstBlock = pstParser->stMemList.pstHead;
    
    PARSERPRINT1("Num of command: %d", pstParser->stMemList.u16Size);

    if(pstBlock != NULL)
    {
        pstFrame = (BinFrame *)pstBlock->pBuffer;
        for(uint8_t i = 0; i < pstParser->CommandTableSize; i++)
        {
            if(pstFrame->u8OpCode == pstParser->pstCommandTable[i].u8CmdCode)
            {
                PARSERPRINT1("Command found, opcode: 0x%02x", pstFrame->u8OpCode);
                pstParser->pstCommandTable[i].CommandHandler(pstFrame->au8Payload, pstFrame->u8PayloadSize);
                pstParser->eStatus = PARSER_OK;
                break;
            }
            else
            {
                if(i == pstParser->CommandTableSize - 1U)
                {
                    /* End of table */
                    PARSERPRINT("No command found");
                    pstParser->eStatus = PARSER_NO_COMMAND;
                }
            }
        }
        ezmSmalloc_ReturnMemBlock(&pstParser->stMemList, pstBlock);
    }
    else
    {
        pstParser->eStatus = PARSER_OK;
    }
}

/******************************************************************************
* Function : ezmParser_RunBinParser
*//** 
*
* This function runs the binary parser, which packs the byte data into a BinFrame, then 
* puts BinFrame in a queue for the command parser to process
*
* PRE-CONDITION: ezmParser_Init
*
* POST-CONDITION: None
* 
* @param    pstParser: (IN)pointer to the parser
* @param    u8Byte: (IN)data byte
* @return   None
*
* \b Example Example:
* @code
* BinCmdParser stParser;
*
* stParser.bUseSof = true;
* stParser.bUseChecksum = true;
* stParser.bUseEncryption = false;
* stParser.bUseChecksum = true;
* stParser.CommandTableSize = 4U;
* stParser.pstCommandTable = CommandTable;
* stParser.CrcCalculate = &CrCCalculate;
* stParser.CrcVeryfy = &CrCVerify;
*
* ezmParser_Init(&stParser, 0xFF);
* ezmParser_RunBinParser(&stParser, 0x02);
* ezmParser_RunBinParser(&stParser, 0x03);
* ezmParser_RunBinParser(&stParser, 0x04);
* ezmParser_RunBinParser(&stParser, 0x05);
* @endcode
*
* @see ezmParser_Init
*
*******************************************************************************/
void ezmParser_RunBinParser(BinCmdParser * pstParser, uint8_t u8Byte)
{
    static BinFrame stFrame;
    static uint8_t u8BuffCount = 0U;

    switch(pstParser->eBinState)
    {
    case START_OF_FRAME:
    {
        VERBOSEPRINT1("START_OF_FRAME");
        VERBOSEPRINT2("byte: 0x%02x", u8Byte);
        if(pstParser->bUseSof)
        {
            if(u8Byte == SOF)
            {
                stFrame.u8Sof = u8Byte;
                pstParser->eBinState = OP_CODE;
                pstParser->eStatus = PARSER_BUSY;
            }
            else
            {
                pstParser->eBinState = START_OF_FRAME;
                pstParser->eStatus = PARSER_WRONG_SOF;
            }
        }
        else
        {
            pstParser->eBinState = OP_CODE;
            pstParser->eStatus = PARSER_BUSY;
        }
        break;
    }
    case OP_CODE:
    {
        VERBOSEPRINT("OP_CODE");
        VERBOSEPRINT1("byte: 0x%02x", u8Byte);
        stFrame.u8OpCode = u8Byte;
        pstParser->eBinState = DATA_LENGTH;
        break;
    }
    case DATA_LENGTH:
    {
        VERBOSEPRINT("DATA_LENGTH");
        VERBOSEPRINT1("byte: 0x%02x", u8Byte);
        if(u8Byte < 1)
        {
            pstParser->eBinState = START_OF_FRAME;
            pstParser->eStatus = PARSER_NO_LEN;
            PARSERPRINT("Data length 0U");
        }
        else if (u8Byte > PAYLOAD_MAX_SIZE)
        {
            pstParser->eBinState = START_OF_FRAME;
            pstParser->eStatus = PARSER_DATA_OVERFLOW;
            PARSERPRINT("Data overflow");
            PARSERPRINT1("Expected: %d", PAYLOAD_MAX_SIZE);
            PARSERPRINT1("Got: %d", u8Byte);
        }
        else
        {
            stFrame.u8PayloadSize = u8Byte;
            u8BuffCount = 0U;
            pstParser->eBinState = DATA;
        }       
        break;
    }
    case DATA:
    {
        VERBOSEPRINT("DATA");
        VERBOSEPRINT1("byte: 0x%02x", u8Byte);
        stFrame.au8Payload[u8BuffCount] = u8Byte;
        u8BuffCount++;
        if(u8BuffCount >= stFrame.u8PayloadSize)
        {
            pstParser->eBinState = CHECKSUM;
            u8BuffCount = 0U;
        }
        break;
    }
    case CHECKSUM:
    {
        VERBOSEPRINT("CHECKSUM");
        VERBOSEPRINT1("byte: 0x%02x", u8Byte);
        if(pstParser->bUseChecksum)
        {
            stFrame.au8Checksum[u8BuffCount] = u8Byte;
            u8BuffCount++;
            if(u8BuffCount >= CRC_SIZE)
            {
                if(pstParser->CrcVeryfy(&stFrame))
                {
                    ezmMemoryBlock * pstBlock;
                    pstBlock = ezmSmalloc_GetFreeBlock(sizeof(BinFrame));
                    memcpy(pstBlock->pBuffer, &stFrame, sizeof(BinFrame));
                    ezmSmalloc_ApendBlockToList(pstBlock, &pstParser->stMemList);
                    pstParser->eStatus = PARSER_OK;
                    pstParser->eBinState = START_OF_FRAME;
                    PARSERPRINT("Checksum OK");
                }
                else
                {
                    pstParser->eStatus = PARSER_WRONG_CHECKSUM;
                    pstParser->eBinState = START_OF_FRAME;
                    PARSERPRINT("Wrong Checksum");
                }
            }
        }
        else
        {
            pstParser->eStatus = PARSER_OK;
            pstParser->eBinState = START_OF_FRAME;
        }
        break;
    }
    default:
    {
        pstParser->eBinState = START_OF_FRAME;
        break;
    }
    }
}
#endif
#endif /* BIN_PARSER == 1U */
/* End of file*/