
/*******************************************************************************
* Filename:         esp32_wifi_controller.h
* Author:           Hai Nguyen
* Original Date:    12.06.2022
* Last Update:      12.06.2022
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
* Written by Hai Nguyen 12.06.2022
*
*******************************************************************************/

/** @file   esp32_wifi_controller.h
 *  @author Hai Nguyen
 *  @date   12.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _ESP32_WIFI_CONTROLLER_H
#define _ESP32_WIFI_CONTROLLER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if(WIFI_CONTROLLER == 1U && SUPPORTED_CHIP == ESP32)
#include "hal/wifi_controller/wifi_controller.h"

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
bool wifiEsp_Initialization(void);
bool wifiEsp_BindingDriverApi(void ** api);
bool wifiEsp_RegisterInterruptCallback(INTERRUPT_CALLBACK callback);

#endif /* WIFI_CONTROLLER == 1U && SUPPORTED_CHIP == ESP32 */

#endif /* _ESP32_WIFI_CONTROLLER_H */

/* End of file */

