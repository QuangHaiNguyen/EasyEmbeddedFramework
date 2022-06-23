
/*******************************************************************************
* Filename:         esp32_mqtt.c
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

/** @file   esp32_mqtt.c
 *  @author Hai Nguyen
 *  @date   20.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "esp32_mqtt.h"

#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "ESP32_MQTT"    /**< module name */


#include <stddef.h>
#include <string.h>

/* esp32 */
#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "utilities/logging/logging.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief
 *
 */
struct MqttComponent
{
    MqttDriverApi               api;        /**< */
    MQTT_EVENT_CALLBACK         callback;   /**< */
    esp_mqtt_client_handle_t    client;     /**< */
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct MqttComponent component;

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/
static MQTT_EVENT espMqtt_ConvertMqttEvent(esp_mqtt_event_id_t event_id);

static bool espMqtt_Config(MqttConfig * config);
static bool espMqtt_Connect(void);
static bool espMqtt_Disconnect(void);
static bool espMqtt_Subscribe(const char* topic);
static bool espMqtt_Unsubscribe(const char* topic);
static bool espMqtt_Publish(const char* topic, void* data, uint32_t size);
static void espMqtt_EventHandler(void *handler_args,
                                 esp_event_base_t base,
                                 int32_t event_id,
                                 void *event_data);
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
bool espMqtt_Initialization(void)
{
    bool is_success = true;

    return is_success;
}


bool espMqtt_BindingDriverApi(void ** api)
{
    bool is_success = true;
    esp_mqtt_client_config_t mqtt_cfg = {0};

    component.api.Mqtt_Config = espMqtt_Config;
    component.api.Mqtt_Connect = espMqtt_Connect;
    component.api.Mqtt_Disconnect = espMqtt_Disconnect;
    component.api.Mqtt_Subscribe = espMqtt_Subscribe;
    component.api.Mqtt_Unsubscribe = espMqtt_Unsubscribe;
    component.api.Mqtt_Publish = espMqtt_Publish;

    component.client = esp_mqtt_client_init(&mqtt_cfg);

    component.client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(component.client, 
                                   ESP_EVENT_ANY_ID, 
                                   espMqtt_EventHandler,
                                   NULL);


    return is_success;
}


bool espMqtt_RegisterEventCallback(MQTT_EVENT_CALLBACK callback)
{
    bool is_success = true;

    component.callback = callback;

    return is_success;
}


/******************************************************************************
* Internal functions
*******************************************************************************/


static bool espMqtt_Config(MqttConfig * config)
{
    bool is_success = false;
    esp_mqtt_client_config_t mqtt_cfg = {0};

    mqtt_cfg.host = config->broker_adr;
    mqtt_cfg.port = config->port;
    mqtt_cfg.username = config->username;
    mqtt_cfg.password = config->pwd;
    mqtt_cfg.client_id = config->client_id;
    mqtt_cfg.transport = MQTT_TRANSPORT_OVER_TCP;
    mqtt_cfg.protocol_ver =  MQTT_PROTOCOL_V_3_1;

    esp_mqtt_set_config(component.client, &mqtt_cfg);

    return is_success;
}


static bool espMqtt_Connect(void)
{
    bool is_success = true;

    esp_mqtt_client_start(component.client);

    return is_success;
}


static bool espMqtt_Disconnect(void)
{
    bool is_success = true;

    esp_mqtt_client_stop(component.client);

    return is_success;
}


static bool espMqtt_Subscribe(const char* topic)
{
    bool is_success = true;

    if(esp_mqtt_client_subscribe(component.client, topic, 0) == -1)
    {
        is_success = false;
    }

    return is_success;
}


static bool espMqtt_Unsubscribe(const char* topic)
{
    bool is_success = true;

    if(esp_mqtt_client_unsubscribe(component.client, topic) == -1)
    {
        is_success = false;
    }

    return is_success;
}


static bool espMqtt_Publish(const char* topic, void* data, uint32_t size)
{
    bool is_success = true;
    
    if(esp_mqtt_client_publish(component.client, topic, data, size, 1, 0) == -1)
    {
        is_success = false;
    }

    return is_success;
}


static void espMqtt_EventHandler(void *handler_args,
                                 esp_event_base_t base,
                                 int32_t event_id,
                                 void *event_data)
{
    esp_mqtt_event_handle_t event = event_data;
    MQTT_EVENT ret_event = EVENT_UNKNOWN;

    TRACE("espMqtt_EventHandler()");

    switch ((esp_mqtt_event_id_t)event_id)
    {
    case MQTT_EVENT_CONNECTED:
        TRACE("MQTT_EVENT_CONNECTED");
        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, NULL, NULL);
        }
        break;

    case MQTT_EVENT_DISCONNECTED:
        TRACE("MQTT_EVENT_DISCONNECTED");
        
        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, NULL, NULL);
        }
        break;

    case MQTT_EVENT_SUBSCRIBED:
        TRACE("MQTT_EVENT_SUBSCRIBED");

        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, (void*)event->topic, NULL);
        }

        break;

    case MQTT_EVENT_UNSUBSCRIBED:
        TRACE("MQTT_EVENT_UNSUBSCRIBED");
        
        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, (void*)event->topic, NULL);
        }
        break;

    case MQTT_EVENT_PUBLISHED:
        TRACE("MQTT_EVENT_UNSUBSCRIBED");

        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, (void*)event->topic, NULL);
        }

        break;

    case MQTT_EVENT_DATA:
        TRACE("MQTT_EVENT_DATA");
        TRACE("[topic = %s]", event->topic);
        TRACE("Data:");
        HEXDUMP(event->data, event->data_len);

        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, event->data, &event->data_len);
        }
        break;

    case MQTT_EVENT_ERROR:
        TRACE("MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            ERROR("reported from esp-tls: %s", strerror(event->error_handle->esp_tls_last_esp_err));
            ERROR("reported from tls stack: %s", strerror(event->error_handle->esp_tls_stack_err));
            ERROR("captured as transport's socket errno: %s", strerror(event->error_handle->esp_transport_sock_errno));
            ERROR("Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }

        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, NULL, NULL);
        }
        break;

    default:
        TRACE("Other event [id = %d]", event->event_id);
        
        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, NULL, NULL);
        }
        break;
    }
}

static MQTT_EVENT espMqtt_ConvertMqttEvent(esp_mqtt_event_id_t event_id)
{
    MQTT_EVENT ret_event = EVENT_UNKNOWN;

    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:
        ret_event = EVENT_CONNECTED;
        break;

    case MQTT_EVENT_DISCONNECTED:
        ret_event = EVENT_DISCONNECTED;
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ret_event = EVENT_SUBSCRIBED;
        break;

    case MQTT_EVENT_UNSUBSCRIBED:
        ret_event = EVENT_UNSUBSCRIBED;
        break;

    case MQTT_EVENT_PUBLISHED:
        ret_event = EVENT_PUBLISHED;
        break;

    case MQTT_EVENT_DATA:
        ret_event = EVENT_DATA_RECEIVED;
        break;

    case MQTT_EVENT_ERROR:
        ret_event = EVENT_ERROR;
        break;

    default:
        ret_event = EVENT_UNKNOWN;
        break;
    }

    return ret_event;
}

/* End of file*/

