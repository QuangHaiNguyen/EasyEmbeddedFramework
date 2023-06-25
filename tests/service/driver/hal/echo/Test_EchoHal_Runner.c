
/*******************************************************************************
* Filename:         EchoHal_runner.c
* Author:           Hai Nguyen
* Original Date:    18.06.2023
* Last Update:      18.06.2023
*
* -----------------------------------------------------------------------------
* Company:          Embedded Easy
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Embedded Easy
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 18.06.2023
*
*******************************************************************************/

/** @file   EchoHal_runner.c
 *  @author Hai Nguyen
 *  @date   18.06.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "unity.h"
#include "unity_fixture.h"


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
/* None */

/******************************************************************************
* External functions
*******************************************************************************/
/* None */


/******************************************************************************
* Internal functions
*******************************************************************************/
TEST_GROUP_RUNNER(EchoHal)
{
    RUN_TEST_CASE(EchoHal, ezHalEcho_GetDriver_Correct);
    RUN_TEST_CASE(EchoHal, ezHalEcho_GetDriver_Incorrect);
    RUN_TEST_CASE(EchoHal, ezHalEcho_ReleaseDriver);
    RUN_TEST_CASE(EchoHal, ezHalEcho_Write_Incorrect);
    RUN_TEST_CASE(EchoHal, ezHalEcho_Write_Correct);
    RUN_TEST_CASE(EchoHal, ezHalEcho_Read_Incorrect);
    RUN_TEST_CASE(EchoHal, ezHalEcho_Read_Correct);
}


/* End of file */

