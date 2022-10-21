
/*******************************************************************************
* Filename:         esp32_mqtt.h
* Author:           Hai Nguyen
* Original Date:    20.06.2022
* Last Update:      20.06.2022
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
* Written by Hai Nguyen 20.06.2022
*
*******************************************************************************/

/** @file   esp32_mqtt.h
 *  @author Hai Nguyen
 *  @date   20.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _ESP32_MQTT_H
#define _ESP32_MQTT_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (MQTT == 1U && SUPPORTED_CHIP == ESP32)

#include <stdint.h>
#include <stdbool.h>
#include "hal/network/mqtt/mqtt.h"

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
bool espMqtt_Initialization(void);
bool espMqtt_BindingDriverApi(void ** api);
bool espMqtt_RegisterEventCallback(MQTT_EVENT_CALLBACK callback);

#endif /* (MQTT == 1U && SUPPORTED_CHIP == ESP32) */
#endif /* _ESP32_MQTT_H */

/* End of file */

