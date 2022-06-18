
/*******************************************************************************
* Filename:         app_wifi_management.h
* Author:           Hai Nguyen
* Original Date:    13.06.2022
* Last Update:      13.06.2022
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
* Written by Hai Nguyen 13.06.2022
*
*******************************************************************************/

/** @file   app_wifi_management.h
 *  @author Hai Nguyen
 *  @date   13.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _APP_WIFI_MANAGEMENT_H
#define _APP_WIFI_MANAGEMENT_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"
#include <stdint.h>
#include <stdbool.h>

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
bool wifiMgmt_Initialize(void);

#endif /* _APP_WIFI_MANAGEMENT_H */

/* End of file */

