/*****************************************************************************
* Filename:         unittest_ez_state_machine.c
* Author:           Hai Nguyen
* Original Date:    02.05.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   unittest_ez_state_machine.c
 *  @author Hai Nguyen
 *  @date   02.05.2024
 *  @brief  One line description of the component
 *
 *  @details Detail description of the component
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(ez_state_machine);

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

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
static void RunAllTests(void);

/******************************************************************************
* External functions
*******************************************************************************/
int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}


TEST_SETUP(ez_state_machine)
{
}


TEST_TEAR_DOWN(ez_state_machine)
{
}


TEST_GROUP_RUNNER(ez_state_machine)
{
    RUN_TEST_CASE(ez_state_machine, TestTempPlate);
}


TEST(ez_state_machine, TestTempPlate)
{
    TEST_ASSERT_EQUAL(false, false);
}


/******************************************************************************
* Internal functions
*******************************************************************************/
static void RunAllTests(void)
{
    RUN_TEST_GROUP(ez_state_machine);
}


/* End of file */
