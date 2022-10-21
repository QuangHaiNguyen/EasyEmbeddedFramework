
/*******************************************************************************
* Filename:         mqtt.c
* Author:           Hai Nguyen
* Original Date:    19.06.2022
* Last Update:      19.06.2022
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
* Written by Hai Nguyen 19.06.2022
*
*******************************************************************************/

/** @file   mqtt.c
 *  @author Hai Nguyen
 *  @date   19.06.2022
 *  @brief  This is the source code for module HAL mqtt. It provides the user a
 *  common api to interact with mqtt 
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "mqtt.h"

#if (MQTT == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "HAL_MQTT"  /**< module name */

#include <string.h>

#include "ezUtilities/logging/logging.h"
#include "utilities/event_notifier/event_notifier.h"
#include "utilities/ezmAssert/ezmAssert.h"

#include "ezmDriver/driver.h"
#include "platforms/esp32/network/esp_mqtt/esp32_mqtt.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief data structure hold driver info and config
 *
 */
struct MqttClient
{
    event_subject subject;          /**< subject to manage the event */
    Driver driver;                  /**< hold driver */
    MqttConfig * config;            /**< point to mqtt config */
    MQTT_EVENT_CALLBACK callback;   /**< callback to handle event from low level driver */
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct MqttClient client = { 0 };


/******************************************************************************
* Function Definitions
*******************************************************************************/
static bool Mqtt_ReceiveEventNotification(event_observer* observer);
static bool Mqtt_StopEventNotification(event_observer* observer);
static uint32_t Mqtt_EventCallback(uint32_t event_code,
                                   void *param1,
                                   void *param2);


/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : Mqtt_GetDriver
*//** 
* @Description:
*
* This function binds the hal driver to the low-level, hardware driver
* 
* @param    None
* @return   Driver structure or NULL
*
*******************************************************************************/
void * Mqtt_GetDriver(void)
{
    TRACE("Mqtt_GetDriver()");

    bool    is_success = true;
    void    *ret_driver = NULL;

    is_success = evntNoti_CreateSubject(&client.subject, 10);

    if (!is_success)
    {
        ERROR("cannot create an event subject");
    }

    if (is_success)
    {
        /* Binding to driver module */
        client.driver.is_busy = false;
        client.driver.SubscribeEventNotification = Mqtt_ReceiveEventNotification;
        client.driver.UnsubscribeEventNotification = Mqtt_StopEventNotification;
#if CONFIG_WIN == 1U
        WARNING("No supported mqtt for window platform");
        is_success = false;
#elif
        /* Calling low level binind function */
        client.driver.init_function = espMqtt_Initialization;
        is_success &= espMqtt_BindingDriverApi((void**)&client.driver.driver_api);
        is_success &= espMqtt_RegisterEventCallback(Mqtt_EventCallback);
#endif
    }

    if (is_success)
    {
        ret_driver = (void*)&client.driver;
        DEBUG("Mqtt client init OK");
    }
    else
    {
        memset(&client, 0, sizeof(struct MqttClient));
        ERROR("Mqtt client init fail");
    }

    return ret_driver;
}

/******************************************************************************
* Internal functions
*******************************************************************************/

/******************************************************************************
* Function : Mqtt_ReceiveEventNotification
*//** 
* @Description:
*
* This function let other modules receive event from this driver module
* 
* @param    *observer: (IN)observer to receive the event 
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool Mqtt_ReceiveEventNotification(event_observer * observer)
{
    return evntNoti_SubscribeEvent(&client.subject, observer);
}


/******************************************************************************
* Function : Mqtt_StopEventNotification
*//** 
* @Description:
*
* This function stop a module from receive event from this driver module
* 
* @param    *observer: (IN)observer to receive the event 
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool Mqtt_StopEventNotification(event_observer * observer)
{
    return evntNoti_UnsubscribeEvent(&client.subject, observer);
}


/******************************************************************************
* Function : Mqtt_EventCallback
*//** 
* @Description:
*
* This function handles the event from low-layer. It does nothing than notifies
* the other modules which subsribed to receive events about this event
* 
* @param    event_code: (IN)type of event
* @param    *param1: (IN)pointer to parameter 1, depend on event_code
* @param    *param2: (IN)pointer to parameter 2, depend on event_code
* @return   always return 0
*
*******************************************************************************/
static uint32_t Mqtt_EventCallback(uint32_t event_code,
                                   void* param1,
                                   void* param2)
{
    DEBUG("receive [event = %d]", event_code);

    /* send event code and data to subscriber */
    evntNoti_NotifyEvent(&client.subject,
                         event_code,
                         param1,
                         param2);
    return 0;
}

#endif /* (MQTT == 1U) */
/* End of file*/

