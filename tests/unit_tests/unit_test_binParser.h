#include <limits.h>
#include "gtest/gtest.h"

#ifndef UNIT_TEST_BIN_PARSER
#define UNIT_TEST_BIN_PARSER

extern "C" {
#include "../../ezmsdk/app/app_config.h"

#if (BIN_PARSER == 1U)
#include "../../ezmsdk/binCmdParser/binCmdParser.h"
#include "../../ezmsdk/ezmDebug/ezmDebug.h"

#define UNUSED(x) (void)(x)

void Sum(uint8_t * pu8Data, uint8_t size);
void Minus(uint8_t * pu8Data, uint8_t size);
void Multiply(uint8_t * pu8Data, uint8_t size);
void Devide(uint8_t * pu8Data, uint8_t size);
bool CrCVerify(BinFrame * pstFrame);
void CrCCalculate(BinFrame * pstFrame);

Command CommandTable[4] = 
{
    {0x01, Sum},
    {0x02, Minus},
    {0x03, Multiply},
    {0x04, Devide},
};

void Sum(uint8_t * pu8Data, uint8_t size)
{
    ASSERT_EQ(size, 2);
    PRINT2("Result: %d", pu8Data[0] + pu8Data[1]);
}

void Minus(uint8_t * pu8Data, uint8_t size)
{
    ASSERT_EQ(size, 2);
    PRINT2("Result: %d", pu8Data[0] - pu8Data[1]);
}

void Multiply(uint8_t * pu8Data, uint8_t size)
{
    ASSERT_EQ(size, 2);
    PRINT2("Result: %d", pu8Data[0] * pu8Data[1]);
}

void Devide(uint8_t * pu8Data, uint8_t size)
{
    ASSERT_EQ(size, 2);
    PRINT2("Result: %d", pu8Data[0] / pu8Data[1]);
}

bool CrCVerify(BinFrame * pstFrame)
{
    UNUSED(pstFrame);
    return true;
}

void CrCCalculate(BinFrame * pstFrame)
{
    UNUSED(pstFrame);
    return;
}

#endif /* BIN_PARSER */
}

namespace 
{
#if (BIN_PARSER == 1U)
    TEST(Parser, Basic) 
    {
        BinCmdParser stParser;

        stParser.bUseSof = true;
        stParser.bUseChecksum = true;
        stParser.bUseEncryption = false;
        stParser.bUseChecksum = true;
        stParser.CommandTableSize = 4U;
        stParser.pstCommandTable = CommandTable;
        stParser.CrcCalculate = &CrCCalculate;
        stParser.CrcVeryfy = &CrCVerify;

        ezmParser_Init(&stParser, 0xFF);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x00U);
        ASSERT_EQ(stParser.stMemList.u8ModuleId, 0xFFU);

        ezmParser_RunBinParser(&stParser, 0x00);
        ezmParser_RunBinParser(&stParser, 0x01);
        ezmParser_RunBinParser(&stParser, 0x02);
        ezmParser_RunBinParser(&stParser, 0x02);
        ezmParser_RunBinParser(&stParser, 0x03);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x01U);

        ezmParser_RunBinParser(&stParser, 0x00);
        ezmParser_RunBinParser(&stParser, 0x02);
        ezmParser_RunBinParser(&stParser, 0x02);
        ezmParser_RunBinParser(&stParser, 0x03);
        ezmParser_RunBinParser(&stParser, 0x02);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x02U);

        ezmParser_RunBinParser(&stParser, 0x00);
        ezmParser_RunBinParser(&stParser, 0x03);
        ezmParser_RunBinParser(&stParser, 0x02);
        ezmParser_RunBinParser(&stParser, 0x02);
        ezmParser_RunBinParser(&stParser, 0x03);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x03U);

        ezmParser_RunBinParser(&stParser, 0x00);
        ezmParser_RunBinParser(&stParser, 0x04);
        ezmParser_RunBinParser(&stParser, 0x02);
        ezmParser_RunBinParser(&stParser, 0x03);
        ezmParser_RunBinParser(&stParser, 0x03);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x04U);

        ezmParser_RunCmdParser(&stParser);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x03U);

        ezmParser_RunCmdParser(&stParser);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x02U);

        ezmParser_RunCmdParser(&stParser);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x01U);

        ezmParser_RunCmdParser(&stParser);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x00U);

        ezmParser_RunBinParser(&stParser, 0x00);
        ezmParser_RunBinParser(&stParser, 0x05);
        ezmParser_RunBinParser(&stParser, 0x02);
        ezmParser_RunBinParser(&stParser, 0x03);
        ezmParser_RunBinParser(&stParser, 0x03);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ezmParser_RunBinParser(&stParser, 0xFF);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x01U);

        ezmParser_RunCmdParser(&stParser);
        ASSERT_EQ(stParser.stMemList.u16Size, 0x00U);
    }
#endif /* BIN_PARSER */
}

#endif /* UNIT_TEST_BIN_PARSER */