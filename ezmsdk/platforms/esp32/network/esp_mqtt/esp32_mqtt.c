
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
* Function : espMqtt_Initialization
*//** 
* @Description:
*
* This function initializes the low-level mqtt driver. At the moment, it does
* nothing
* 
* @param    None
* @return   None
*
*******************************************************************************/
bool espMqtt_Initialization(void)
{
    bool is_success = true;  

    TRACE("espMqtt_Initialization()");

#if (DEBUG_LVL >= LVL_TRACE)
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
#endif /*(DEBUG_LVL >= LVL_TRACE)*/

    return is_success;
}


/******************************************************************************
* Function : espMqtt_Initialization
*//** 
* @Description:
*
* This function initializes the low-level mqtt driver. At the moment, it does
* nothing
* 
* @param    None
* @return   None
*
*******************************************************************************/
bool espMqtt_BindingDriverApi(void ** api)
{
    
    bool is_success = true;
    esp_mqtt_client_config_t mqtt_cfg = {0};

    TRACE("espMqtt_BindingDriverApi()");

    component.api.Mqtt_Config = espMqtt_Config;
    component.api.Mqtt_Connect = espMqtt_Connect;
    component.api.Mqtt_Disconnect = espMqtt_Disconnect;
    component.api.Mqtt_Subscribe = espMqtt_Subscribe;
    component.api.Mqtt_Unsubscribe = espMqtt_Unsubscribe;
    component.api.Mqtt_Publish = espMqtt_Publish;
    component.client = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(component.client, 
                                   ESP_EVENT_ANY_ID, 
                                   espMqtt_EventHandler,
                                   NULL);
    

    if(is_success)
    {
        *api =  (void**)&component.api;
    }

    return is_success;
}


/******************************************************************************
* Function : espMqtt_Initialization
*//** 
* @Description:
*
* This function initializes the low-level mqtt driver. At the moment, it does
* nothing
* 
* @param    None
* @return   None
*
*******************************************************************************/
bool espMqtt_RegisterEventCallback(MQTT_EVENT_CALLBACK callback)
{
    bool is_success = true;

    component.callback = callback;

    return is_success;
}


/******************************************************************************
* Internal functions
*******************************************************************************/


/******************************************************************************
* Function : espMqtt_Config
*//** 
* @Description:
*
* This function configures the esp32 mqtt driver
* 
* @param    *config: (IN) pointer to the MQTT configuration
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool espMqtt_Config(MqttConfig * config)
{
    bool is_success = true;
    esp_err_t err = ESP_OK;
    esp_mqtt_client_config_t mqtt_cfg = {0};

    TRACE("espMqtt_Config()");

    mqtt_cfg.host = config->broker_adr;
    mqtt_cfg.port = config->port;
    mqtt_cfg.username = config->username;
    mqtt_cfg.password = config->pwd;
    mqtt_cfg.client_id = config->client_id;
    mqtt_cfg.transport = MQTT_TRANSPORT_OVER_TCP;
    mqtt_cfg.protocol_ver =  MQTT_PROTOCOL_V_3_1;

    err = esp_mqtt_set_config(component.client, &mqtt_cfg);

    if(err != ESP_OK)
    {
        ERROR("cannot config mqtt");
        is_success = false;
    }

    return is_success;
}


/******************************************************************************
* Function : espMqtt_Connect
*//** 
* @Description:
*
* This function connects the esp32 to the broker according to the configuration
* 
* @param    None
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool espMqtt_Connect(void)
{
    bool is_success = true;
    
    TRACE("espMqtt_Connect()");
    esp_mqtt_client_start(component.client);

    return is_success;
}


/******************************************************************************
* Function : espMqtt_Disconnect
*//** 
* @Description:
*
* This function disconnects the esp32 from the broker according to the
* configuration
* 
* @param    None
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool espMqtt_Disconnect(void)
{
    bool is_success = true;

    esp_mqtt_client_stop(component.client);

    return is_success;
}


/******************************************************************************
* Function : espMqtt_Subscribe
*//** 
* @Description:
*
* This function subcribes the esp32 to a topic
* @param    *topic: (IN)pointer to the topic
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool espMqtt_Subscribe(const char* topic)
{
    bool is_success = true;

    if(esp_mqtt_client_subscribe(component.client, topic, 0) == -1)
    {
        is_success = false;
    }

    return is_success;
}


/******************************************************************************
* Function : espMqtt_Unsubscribe
*//** 
* @Description:
*
* This function unsubcribes the esp32 from a topic
* 
* @param    None
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool espMqtt_Unsubscribe(const char* topic)
{
    bool is_success = true;

    if(esp_mqtt_client_unsubscribe(component.client, topic) == -1)
    {
        is_success = false;
    }

    return is_success;
}


/******************************************************************************
* Function : espMqtt_Publish
*//** 
* @Description:
*
* This function publish the data to a corresponding topic
* nothing
* 
* @param    *topic: (IN)pointer to the topic
* @param    *data:  (IN)pointer to the data 
* @param    size:   (IN)size of the data
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool espMqtt_Publish(const char* topic, void* data, uint32_t size)
{
    bool is_success = true;
    
    if(esp_mqtt_client_publish(component.client, topic, data, size, 1, 0) == -1)
    {
        is_success = false;
    }

    return is_success;
}


/******************************************************************************
* Function : espMqtt_EventHandler
*//** 
* @Description:
*
* This function handles the mqtt events. It converts the esp32 event to sdk+
* event and notifies the upper layer
* 
* @param    *handler_args: (IN) extra arguments
* @param    base: (IN)base event
* @param    event_id: (IN) id of the event
* @param    *event_data (IN)pointer to the event data, if if is applicable
* @return   None
*
*******************************************************************************/
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
        TRACE("MQTT_EVENT_PUBLISHED");

        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, (void*)event->topic, NULL);
        }

        break;

    case MQTT_EVENT_DATA:
    {
        DataEvent sdk_data_event; 

        TRACE("MQTT_EVENT_DATA");
        TRACE("[topic = %s]", event->topic);
        TRACE("Data:");
        HEXDUMP(event->data, event->data_len);
        
        sdk_data_event.data = event->data;
        sdk_data_event.size = event->data_len;

        ret_event = espMqtt_ConvertMqttEvent(event_id);
        if(component.callback)
        {
            component.callback(ret_event, 
                               (void*)event->topic,
                               (void*)&sdk_data_event);
        }
        break;
    }  

    case MQTT_EVENT_ERROR:
        TRACE("MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
        {
            ERROR("reported from esp-tls: %s",
                  strerror(event->error_handle->esp_tls_last_esp_err));
            
            ERROR("reported from tls stack: %s",
                  strerror(event->error_handle->esp_tls_stack_err));
            
            ERROR("captured as transport's socket errno: %s",
                  strerror(event->error_handle->esp_transport_sock_errno));
            
            ERROR("Last errno string (%s)",
                  strerror(event->error_handle->esp_transport_sock_errno));

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


/******************************************************************************
* Function : espMqtt_ConvertMqttEvent
*//** 
* @Description:
*
* This function converts esp32 mqtt event to sdk mqtt event
* 
* @param    event_id: (IN)esp32 event
* @return   sdk event
*
*******************************************************************************/
static MQTT_EVENT espMqtt_ConvertMqttEvent(esp_mqtt_event_id_t event_id)
{
    MQTT_EVENT ret_event = EVENT_UNKNOWN;

    switch (event_id)
    {
    case MQTT_EVENT_CONNECTED:      ret_event = EVENT_CONNECTED;    break;
    case MQTT_EVENT_DISCONNECTED:   ret_event = EVENT_DISCONNECTED; break;
    case MQTT_EVENT_SUBSCRIBED:     ret_event = EVENT_SUBSCRIBED;   break;
    case MQTT_EVENT_UNSUBSCRIBED:   ret_event = EVENT_UNSUBSCRIBED; break;
    case MQTT_EVENT_PUBLISHED:      ret_event = EVENT_PUBLISHED;    break;
    case MQTT_EVENT_DATA:   ret_event = EVENT_DATA_RECEIVED;        break;
    case MQTT_EVENT_ERROR:  ret_event = EVENT_ERROR;                break;
    default:                ret_event = EVENT_UNKNOWN;              break;
    }

    return ret_event;
}

/* End of file*/

