
/*******************************************************************************
* Filename:         unity_test_setup.h
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

/** @file   unity_test_setup.h
 *  @author Hai Nguyen
 *  @date   21.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _UNITY_TEST_SETUP_H
#define _UNITY_TEST_SETUP_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

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
* Function Prototypes
*******************************************************************************/
void unitySetup_RegisterSetup(void (*setUp)(void));
void unitySetup_RegisterTearDown(void (*tearDown)(void));

#endif /* _UNITY_TEST_SETUP_H */

/* End of file */

