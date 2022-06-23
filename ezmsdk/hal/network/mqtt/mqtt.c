
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
 *  @brief  This is the source for a module
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
#define MOD_NAME    "MQTT"       /**< module name */

#include <string.h>

#include "utilities/logging/logging.h"
#include "utilities/event_notifier/event_notifier.h"
#include "utilities/ezmAssert/ezmAssert.h"
#include "ezmDriver/driver.h"
#include "platforms/esp32/network/esp_mqtt/esp32_mqtt.h"
/*the rest of include go here*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
struct MqttClient
{
    bool is_config;                 /**< */
    event_subject subject;          /**< */
    Driver driver;                  /**< */
    MqttConfig * config;            /**< */
    MQTT_EVENT_CALLBACK callback;   /**< */
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct MqttClient client = { 0 };


/******************************************************************************
* Function Definitions
*******************************************************************************/
static void Mqtt_SetCofig(void * config);
static void *Mqtt_GetCofig(void);
static bool Mqtt_ReceiveEventNotification(event_observer* observer);
static bool Mqtt_StopEventNotification(event_observer* observer);
static uint32_t Mqtt_EventCallback(uint32_t event_code,
                                   void *param1,
                                   void *param2);

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : sum
*//** 
* @Description:
*
* This function initializes the ring buffer
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* @Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
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
        client.is_config = false;

        /* Binding to driver module */
        client.driver.is_busy = false;
        client.driver.SubscribeEventNotification = Mqtt_ReceiveEventNotification;
        client.driver.UnsubscribeEventNotification = Mqtt_StopEventNotification;
        client.driver.SetConfig = Mqtt_SetCofig;
        client.driver.GetConfig = Mqtt_GetCofig;

        /* Calling low level binind function */
        client.driver.init_function = espMqtt_Initialization;
        is_success &= espMqtt_BindingDriverApi((void**)&client.driver);
        is_success &= espMqtt_RegisterEventCallback(Mqtt_EventCallback);
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
static void Mqtt_SetCofig(void * config)
{
    if (config)
    {
        client.config = config;
        client.is_config = true;
    }
}

static void* Mqtt_GetCofig(void)
{
    return (void*)client.config;
}

static bool Mqtt_ReceiveEventNotification(event_observer * observer)
{
    return evntNoti_SubscribeEvent(&client.subject, observer);
}

static bool Mqtt_StopEventNotification(event_observer * observer)
{
    return evntNoti_UnsubscribeEvent(&client.subject, observer);
}

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

