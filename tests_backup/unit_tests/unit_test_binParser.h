#include <limits.h>
#include "gtest/gtest.h"

#ifndef UNIT_TEST_BIN_PARSER
#define UNIT_TEST_BIN_PARSER

extern "C" {
#include "../../ezmsdk/app/app_config.h"

#if (CONFIG_BIN_PARSER == 1U)
#include "../../ezmsdk/binCmdParser/binCmdParser.h"
#include "../../ezmsdk/ezmDebug/ezmDebug.h"
#include "../../ezmsdk/helper/hexdump/hexdump.h"
#include "../../ezmsdk/helper/stcmem/stcmem.h"

#define UNUSED(x) (void)(x)

#define SUM     5
#define MINUS   1
#define MULTI   6
#define DIVIDE  3

void Sum(void * pu8Data, uint16_t size);
void Minus(void * pu8Data, uint16_t size);
void Multiply(void * pu8Data, uint16_t size);
void Devide(void * pu8Data, uint16_t size);
bool CrCVerify(BinaryFrame* frame);
void CrCCalculate(BinaryFrame* frame);

Command CommandTable[4] = 
{
    {0x01, Sum},
    {0x02, Minus},
    {0x03, Multiply},
    {0x04, Devide},
};

void Sum(void* pu8Data, uint16_t size)
{
    uint8_t* payload = (uint8_t*)pu8Data;
    ASSERT_EQ(size, 2);
    ASSERT_EQ(SUM, payload[0] + payload[1]);
    PRINT2("Result: %d", payload[0] + payload[1]);
}

void Minus(void* pu8Data, uint16_t size)
{
    uint8_t* payload = (uint8_t*)pu8Data;
    ASSERT_EQ(size, 2);
    ASSERT_EQ(MINUS, payload[0] - payload[1]);
    PRINT2("Result: %d", payload[0] - payload[1]);
}

void Multiply(void* pu8Data, uint16_t size)
{
    uint8_t* payload = (uint8_t*)pu8Data;
    ASSERT_EQ(size, 2);
    ASSERT_EQ(MULTI, payload[0] * payload[1]);
    PRINT2("Result: %d", payload[0] * payload[1]);
}

void Devide(void* pu8Data, uint16_t size)
{
    uint8_t* payload = (uint8_t*)pu8Data;
    ASSERT_EQ(size, 2);
    ASSERT_EQ(DIVIDE, payload[0] / payload[1]);
    PRINT2("Result: %d", payload[0] / payload[1]);
}

bool CrCVerify(BinaryFrame* frame)
{
    UNUSED(frame);
    return true;
}

void CrCCalculate(BinaryFrame* frame)
{
    UNUSED(frame);
    return;
}

#endif /* CONFIG_BIN_PARSER */
}

namespace 
{
#if (CONFIG_BIN_PARSER == 1U)
    TEST(Parser, Basic) 
    {
        BinCmdParser test_parser = {0};
        uint8_t buffer[64] = {0};
        bool is_success;
        uint8_t compare;

        ezmStcMem_Initialization();

        test_parser.command_table_size = 4U;
        test_parser.command_table = CommandTable;

        is_success = ezmParser_Init(&test_parser, buffer, 64, NULL);
        ASSERT_EQ(true, is_success);

        /* sum command */
        ezmParser_RunBinParser(&test_parser, 0x80);
        ASSERT_EQ(0x80, test_parser.curr_frame->header.sof);

        ezmParser_RunBinParser(&test_parser, 0xbe);
        ezmParser_RunBinParser(&test_parser, 0xef);
        ezmParser_RunBinParser(&test_parser, 0xca);
        ezmParser_RunBinParser(&test_parser, 0xfe);
        ASSERT_EQ(0xbeefcafe, test_parser.curr_frame->header.uuid);

        ezmParser_RunBinParser(&test_parser, 0x01);
        ASSERT_EQ(0x01, test_parser.curr_frame->header.opcode);

        ezmParser_RunBinParser(&test_parser, 0x00);
        ASSERT_EQ(0x00, test_parser.curr_frame->header.encrypt_info);
        
        ezmParser_RunBinParser(&test_parser, 0x00);
        ezmParser_RunBinParser(&test_parser, 0x02);
        ASSERT_EQ(0x0002, test_parser.curr_frame->header.payload_size_byte);

        ezmParser_RunBinParser(&test_parser, 0x02);
        ezmParser_RunBinParser(&test_parser, 0x03);


        /* minus command */
        ezmParser_RunBinParser(&test_parser, 0x80);
        ASSERT_EQ(0x80, test_parser.curr_frame->header.sof);

        ezmParser_RunBinParser(&test_parser, 0xbe);
        ezmParser_RunBinParser(&test_parser, 0xef);
        ezmParser_RunBinParser(&test_parser, 0xca);
        ezmParser_RunBinParser(&test_parser, 0xfe);
        ASSERT_EQ(0xbeefcafe, test_parser.curr_frame->header.uuid);

        ezmParser_RunBinParser(&test_parser, 0x02);
        ASSERT_EQ(0x02, test_parser.curr_frame->header.opcode);

        ezmParser_RunBinParser(&test_parser, 0x00);
        ASSERT_EQ(0x00, test_parser.curr_frame->header.encrypt_info);

        ezmParser_RunBinParser(&test_parser, 0x00);
        ezmParser_RunBinParser(&test_parser, 0x02);
        ASSERT_EQ(0x0002, test_parser.curr_frame->header.payload_size_byte);

        ezmParser_RunBinParser(&test_parser, 0x03);
        ezmParser_RunBinParser(&test_parser, 0x02);


        /* multiply command */
        ezmParser_RunBinParser(&test_parser, 0x80);
        ASSERT_EQ(0x80, test_parser.curr_frame->header.sof);

        ezmParser_RunBinParser(&test_parser, 0xbe);
        ezmParser_RunBinParser(&test_parser, 0xef);
        ezmParser_RunBinParser(&test_parser, 0xca);
        ezmParser_RunBinParser(&test_parser, 0xfe);
        ASSERT_EQ(0xbeefcafe, test_parser.curr_frame->header.uuid);

        ezmParser_RunBinParser(&test_parser, 0x03);
        ASSERT_EQ(0x03, test_parser.curr_frame->header.opcode);

        ezmParser_RunBinParser(&test_parser, 0x00);
        ASSERT_EQ(0x00, test_parser.curr_frame->header.encrypt_info);

        ezmParser_RunBinParser(&test_parser, 0x00);
        ezmParser_RunBinParser(&test_parser, 0x02);
        ASSERT_EQ(0x0002, test_parser.curr_frame->header.payload_size_byte);

        ezmParser_RunBinParser(&test_parser, 0x03);
        ezmParser_RunBinParser(&test_parser, 0x02);

        /* divide command */
        ezmParser_RunBinParser(&test_parser, 0x80);
        ASSERT_EQ(0x80, test_parser.curr_frame->header.sof);

        ezmParser_RunBinParser(&test_parser, 0xbe);
        ezmParser_RunBinParser(&test_parser, 0xef);
        ezmParser_RunBinParser(&test_parser, 0xca);
        ezmParser_RunBinParser(&test_parser, 0xfe);
        ASSERT_EQ(0xbeefcafe, test_parser.curr_frame->header.uuid);

        ezmParser_RunBinParser(&test_parser, 0x04);
        ASSERT_EQ(0x04, test_parser.curr_frame->header.opcode);

        ezmParser_RunBinParser(&test_parser, 0x00);
        ASSERT_EQ(0x00, test_parser.curr_frame->header.encrypt_info);

        ezmParser_RunBinParser(&test_parser, 0x00);
        ezmParser_RunBinParser(&test_parser, 0x02);
        ASSERT_EQ(0x0002, test_parser.curr_frame->header.payload_size_byte);

        ezmParser_RunBinParser(&test_parser, 0x06);
        ezmParser_RunBinParser(&test_parser, 0x02);

        /* wrong command */
        ezmParser_RunBinParser(&test_parser, 0x80);
        ASSERT_EQ(0x80, test_parser.curr_frame->header.sof);

        ezmParser_RunBinParser(&test_parser, 0xbe);
        ezmParser_RunBinParser(&test_parser, 0xef);
        ezmParser_RunBinParser(&test_parser, 0xca);
        ezmParser_RunBinParser(&test_parser, 0xfe);
        ASSERT_EQ(0xbeefcafe, test_parser.curr_frame->header.uuid);

        ezmParser_RunBinParser(&test_parser, 0x05);
        ASSERT_EQ(0x05, test_parser.curr_frame->header.opcode);

        ezmParser_RunBinParser(&test_parser, 0x00);
        ASSERT_EQ(0x00, test_parser.curr_frame->header.encrypt_info);

        ezmParser_RunBinParser(&test_parser, 0x00);
        ezmParser_RunBinParser(&test_parser, 0x02);
        ASSERT_EQ(0x0002, test_parser.curr_frame->header.payload_size_byte);

        ezmParser_RunBinParser(&test_parser, 0x06);
        ezmParser_RunBinParser(&test_parser, 0x02);

    }
#endif /* CONFIG_BIN_PARSER */
}

#endif /* UNIT_TEST_BIN_PARSER */