
/*******************************************************************************
* Filename:         ezKernel_runner.c
* Author:           Hai Nguyen
* Original Date:    15.09.2022
* Last Update:      15.09.2022
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
* Written by Hai Nguyen 15.09.2022
*
*******************************************************************************/

/** @file   ezKernel_runner.c
 *  @author Hai Nguyen
 *  @date   15.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "unity.h"
#include "unity_fixture.h"

#if(EZ_KERNEL == 1U)

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
TEST_GROUP_RUNNER(ezKernel)
{
    RUN_TEST_CASE(ezKernel, Test_ezKernel_GetTickMillis_TickIsAdvance);
    RUN_TEST_CASE(ezKernel, Test_ezKernel_Run_OneShortTask);
    RUN_TEST_CASE(ezKernel, Test_ezKernel_AddTask_MoreTaskThanSupported);
    RUN_TEST_CASE(ezKernel, Test_ezKernel_Run_LoopTask);
    RUN_TEST_CASE(ezKernel, Test_ezKernel_Run_RunUntilComplete);
    RUN_TEST_CASE(ezKernel, Test_ezKernel_Run_RunFailTask);
    RUN_TEST_CASE(ezKernel, Test_ezKernel_Run_AddAnotherTaskInsideTaskFunction);
    RUN_TEST_CASE(ezKernel, Test_ezKernel_GetTickMillis_TickValueCorrect);
}

#endif /* EZ_KERNEL == 1U */

/* End of file */

