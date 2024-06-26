
/*******************************************************************************
* Filename:         ez_hal_uart_runner.c
* Author:           Hai Nguyen
* Original Date:    25.06.2023
* Last Update:      25.06.2023
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
* Written by Hai Nguyen 25.06.2023
*
*******************************************************************************/

/** @file   ez_hal_uart_runner.c
 *  @author Hai Nguyen
 *  @date   25.06.2023
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
TEST_GROUP_RUNNER(ez_hal_uart)
{
    RUN_TEST_CASE(ez_hal_uart, ezHalUart_GetDriver_Correct);
    RUN_TEST_CASE(ez_hal_uart, ezHalUart_GetDriver_Incorrect);
    RUN_TEST_CASE(ez_hal_uart, ezHalUart_Write_Without_Get_Driver);
    RUN_TEST_CASE(ez_hal_uart, ezHalUart_ReleaseDriver);
    RUN_TEST_CASE(ez_hal_uart, UsingUnsupportedAPI);
    RUN_TEST_CASE(ez_hal_uart, ezHalUart_Write); 
    RUN_TEST_CASE(ez_hal_uart, ezHalUart_WriteReadSequencial);
    RUN_TEST_CASE(ez_hal_uart, ezHalUart_WriteReadBlock);
}


/* End of file */

