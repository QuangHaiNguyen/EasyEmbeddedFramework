
/*******************************************************************************
* Filename:         app_mqtt.h
* Author:           Hai Nguyen
* Original Date:    23.06.2022
* Last Update:      23.06.2022
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
* Written by Hai Nguyen 23.06.2022
*
*******************************************************************************/

/** @file   app_mqtt.h
 *  @author Hai Nguyen
 *  @date   23.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _APP_MQTT_H
#define _APP_MQTT_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#include <stdbool.h>
#include <stdint.h>

#include "data_model/data_model.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef enum
{
    WATER_SYS_ONOFF,
    WATER_SYS_WATER_LVL,
    WATER_SYS_LAST_WATER,
    WATER_SYS_INTERNAL_TEMP,
    WATER_SYS_HEARTBEAT,
    NUM_OF_INDEX,
}DATABASE_INDEX;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool appMqtt_Initialization(void);
bool appMqtt_BindDataToDataPoint(DATABASE_INDEX index,
                                 void* data,
                                 uint32_t size);

bool appMqtt_WriteData(DATABASE_INDEX index, void* data, uint32_t size);
bool appMqtt_SubscribeDataChangeEvent(DATABASE_INDEX index,
                                      event_observer * observer);

bool appMqtt_SetDataByTopic(const char * topic, void* data, uint32_t size);
#endif /* _APP_MQTT_H */

/* End of file */

