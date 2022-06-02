
/*******************************************************************************
* Filename:         unit_test_logging1.h
* Author:           Hai Nguyen
* Original Date:    01.06.2022
* Last Update:      01.06.2022
*
* -----------------------------------------------------------------------------
* Comany:           Easy Embedded
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Easy Embedded
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 01.06.2022
*
*******************************************************************************/

#ifndef _UNIT_TEST_LOGGING1
#define _UNIT_TEST_LOGGING1

#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "utilities/logging/logging.h"

#define DEBUG_LVL   LVL_TRACE
#define MOD_NAME    "logging1"
}

namespace 
{
    TEST(logging1, root_print_function) 
    {
        dbg_print("hello world\n");
        dbg_print("%s\n", "hello another world");
        dbg_print("interger = %d\n", 10);
        dbg_print("float = %f\n", 3.1415);
        dbg_print("char = %c\n", 'a');
        dbg_print("hex = %x\n", 0x05);
    }

    TEST(logging1, debug_module_name)
    {
        DEBUG("hello world");
        DEBUG("%s", "hello another world");
        DEBUG("interger = %d", 10);
        DEBUG("float = %f", 3.1415);
        DEBUG("char = %c", 'a');
        DEBUG("hex = %x", 0x05);
    }

    TEST(logging1, critical_module_name)
    {
        CRITICAL("hello world");
        CRITICAL("%s", "hello another world");
        CRITICAL("interger = %d", 10);
        CRITICAL("float = %f", 3.1415);
        CRITICAL("char = %c", 'a');
        CRITICAL("hex = %x", 0x05);
    }

    TEST(logging1, trace_module_name)
    {
        TRACE("hello world");
        TRACE("%s", "hello another world");
        TRACE("interger = %d", 10);
        TRACE("float = %f", 3.1415);
        TRACE("char = %c", 'a');
        TRACE("hex = %x", 0x05);
    }
}

#endif /* _UNIT_TEST_LOGGING1 */
