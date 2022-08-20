
/*******************************************************************************
* Filename:         unity_test_setup.c
* Author:           Hai Nguyen
* Original Date:    21.08.2022
* Last Update:      21.08.2022
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
* Written by Hai Nguyen 21.08.2022
*
*******************************************************************************/

/** @file   unity_test_setup.c
 *  @author Hai Nguyen
 *  @date   21.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "unity_test_setup.h"
#include "unity_test_platform/unity.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef void (*setup_callback)(void);
typedef void (*teardown_callback)(void);

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
static setup_callback setup_callback_ptr = NULL;
static teardown_callback teardown_callback_ptr = NULL;

/******************************************************************************
* External functions
*******************************************************************************/
void setUp(void)
{
    if (setup_callback_ptr)
    {
        setup_callback_ptr();
    }
}


void tearDown(void)
{
    if (teardown_callback_ptr)
    {
        teardown_callback_ptr();
    }
}


void unitySetup_RegisterSetup(void (*setUp)(void))
{
    if (setUp)
    {
        setup_callback_ptr = setUp;
    }
}


void unitySetup_RegisterTearDown(void (*tearDown)(void))
{
    if (tearDown)
    {
        teardown_callback_ptr = tearDown;
    }
}


/******************************************************************************
* Internal functions
*******************************************************************************/
/* None */


/* End of file*/

