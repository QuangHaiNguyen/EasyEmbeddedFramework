
/*******************************************************************************
* Filename:         TestezRpc_Runner.c
* Author:           Hai Nguyen
* Original Date:    10.09.2022
* Last Update:      10.09.2022
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
* Written by Hai Nguyen 10.09.2022
*
*******************************************************************************/

/** @file   TestezRpc_Runner.c
 *  @author Hai Nguyen
 *  @date   10.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (CONFIG_EZ_RPC_TEST == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "TestezRpc_Runner"       /**< module name */
#include "utilities/logging/logging.h"

#include "unity_test_platform/unity.h"
#include "unity_test_platform/unity_fixture.h"

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
TEST_GROUP_RUNNER(ezRpc)
{
    RUN_TEST_CASE(ezRpc, Test_ezRpc_Initialization_InvalidParams);
    RUN_TEST_CASE(ezRpc, Test_ezRpc_Initialization_ValidParams);
    RUN_TEST_CASE(ezRpc, Test_ezRpc_SetCrcFunctions_InvalidParams);
    RUN_TEST_CASE(ezRpc, Test_ezRpc_SetCrcFunctions_ValidParams);
    RUN_TEST_CASE(ezRpc, Test_ezRPC_CreateRpcMessage_InvalidParam);
    RUN_TEST_CASE(ezRpc, Test_ezRPC_CreateRpcMessage_WithNoPayload);
    RUN_TEST_CASE(ezRpc, Test_ezRPC_CreateRpcMessage_OverflowRecords);
    RUN_TEST_CASE(ezRpc, Test_ezRPC_Run_NumOfValidMessageReceived);
    //RUN_TEST_CASE(ezRpc, Test_ezRPC_Run_RequestTimeOut) //TODO need tick to run
}

#endif /* CONFIG_EZ_RPC_TEST == 1U */

/* End of file*/

