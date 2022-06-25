
/*******************************************************************************
* Filename:         app_mqtt.c
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

/** @file   app_mqtt.c
 *  @author Hai Nguyen
 *  @date   23.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "app_mqtt.h"

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "APP_MQTT"       /**< module name */

#include "utilities/logging/logging.h"
#include "utilities/event_notifier/event_notifier.h"

#include "hal/network/mqtt/mqtt.h"
#include "hal/wifi_controller/wifi_controller.h"

#include "ezmDriver/driver.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define TOPIC_STATUS    "STATUS/"
#define TOPIC_SET       "SET/"

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief  
 *
 */
typedef struct
{
    const char * topic;     /**< */
    DataPoint data_point;   /**< */
}MqttDataPoint;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static const char * broker = "mqtt.eclipseprojects.io";

static MqttConfig mqtt_config = {0};
static event_observer wifi_observer;
static event_observer mqtt_observer;
static MqttDriverApi * mqtt_drv;


MqttDataPoint database[] = {
    {"system/test", DATA_POINT_INVALID},
};

/******************************************************************************
* Function Definitions
*******************************************************************************/
static bool appMqtt_SubscribeAllTopics(void);

static int appMqtt_WifiEventHandle(uint32_t event_code,
                                    void * param1,
                                    void * param2);

static int appMqtt_MqttEventHandle(uint32_t event_code,
                                    void * param1,
                                    void * param2);

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
bool appMqtt_Initialization(void)
{

    TRACE("appMqtt_Initialization");
    bool is_success = true;

    /* house keeping for the module */
    mqtt_drv = NULL;
    
    mqtt_config.broker_adr  = broker;
    mqtt_config.username = NULL;
    mqtt_config.pwd = NULL;
    mqtt_config.client_id = NULL;
    mqtt_config.port = 1883;
    mqtt_config.is_tls_activate = false;
    mqtt_config.transport_type = TCPIP;
    mqtt_config.tls_config = NULL;

    
    if(is_success)
    {
        is_success &= evntNoti_CreateObserver(&wifi_observer, appMqtt_WifiEventHandle);
        is_success &= ezmDriver_SubscribeDriverEvent(WIFI_CTRL_DRIVER,
                                                     &wifi_observer);
    }
    
    if(is_success)
    {
        ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);

        if(mqtt_drv == NULL)
        {
            ERROR("Cannot get MQTT driver");
            is_success = false;
        }
        else
        {
            is_success &= mqtt_drv->Mqtt_Config(&mqtt_config);
        }

        ezmDriver_ReleaseDriverInstance(WIFI_CTRL_DRIVER);
    }

    if(is_success)
    {
        is_success &= evntNoti_CreateObserver(&mqtt_observer, appMqtt_MqttEventHandle);
        is_success &= ezmDriver_SubscribeDriverEvent(MQTT_DRIVER,
                                                     &mqtt_observer);
    }


    if(!is_success)
    {
        ERROR("cannot get mqtt event");
    }
    return is_success;
}


bool appMqtt_BindDataToDataPoint(DATABASE_INDEX index,
                                      void* data,
                                      uint32_t size)
{
    TRACE("appMqtt_BindDataToDataPoint()");
    bool is_success = false;
    
    if(index < NUM_OF_INDEX && database[index].data_point == DATA_POINT_INVALID)
    {   
        database[index].data_point = DataModel_CreateDataPoint(data, size);;
        is_success = true;
    }
    return is_success;
}

bool appMqtt_WriteData(DATABASE_INDEX index, void* data, uint32_t size)
{
    bool is_success = false;
    char set_topic[128] = {0};

    TRACE("appMqtt_WriteData()");

    if(index < NUM_OF_INDEX && 
       database[index].data_point != DATA_POINT_INVALID)
    {
        is_success = DataModel_WriteDataPoint(database[index].data_point, 
                                              data, 
                                              size);
        
        ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);
        
        if(mqtt_drv != NULL)
        {
            sprintf(set_topic, "%s%s", TOPIC_STATUS, database[index].topic);
            is_success &= mqtt_drv->Mqtt_Publish(set_topic, data, size);
            DEBUG("send data to [topic = %s]", set_topic);
#if DEBUG_LVL > LVL_DEBUG
            TRACE("data:");
            HEXDUMP(data, size);
#endif
        }
        else
        {
            ERROR("cannot publish data");
        }

        ezmDriver_ReleaseDriverInstance(MQTT_DRIVER);
    }

    return is_success;
}

bool appMqtt_SubscribeDataChangeEvent(DATABASE_INDEX index,
                                      event_observer * observer)
{
    bool is_success = true;

    if(index < NUM_OF_INDEX &&
       database[index].data_point != DATA_POINT_INVALID)
    {
        is_success = DataModel_SubscribeDataPointEvent(database[index].data_point,
                                                       observer);
    }

    return is_success;
}
/******************************************************************************
* Internal functions
*******************************************************************************/

static bool appMqtt_SubscribeAllTopics(void)
{
    TRACE("appMqtt_SubscribeAllTopics()");

    char status_topic[128] = {0};
    bool is_success = true;

    ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);

    if(mqtt_drv != NULL)
    {
        for (uint32_t i = 0; i < NUM_OF_INDEX; i++)
        {
            sprintf(status_topic, "%s%s\0", TOPIC_SET, database[i].topic);
            is_success &=  mqtt_drv->Mqtt_Subscribe(status_topic); 
            
        }
    }
    else
    {
        ERROR("cannot connect to the broker");
    }

    ezmDriver_ReleaseDriverInstance(MQTT_DRIVER);

    return is_success;
}

static int appMqtt_WifiEventHandle(uint32_t event_code,
                                    void * param1,
                                    void * param2)
{

    TRACE("WiFiMgmt_WifiEventHandle()");
    DEBUG("[event code = %d]", event_code);
    
    bool is_success = true;

    switch((HAL_WIFI_EVENT)event_code)
    {
    case WIFI_STA_START:
    case WIFI_STA_STOP:
    case WIFI_CONNECTING:
    case WIFI_SCANNING:
    case WIFI_SCAN_RESULT:
        break;

    case WIFI_CONNECTED:
        DEBUG("connected to an AP, so we start MQTT");

        ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);
        if(mqtt_drv != NULL)
        {
            is_success &= mqtt_drv->Mqtt_Connect();
        }
        else
        {
            ERROR("cannot connect to the broker");
        }
        ezmDriver_ReleaseDriverInstance(MQTT_DRIVER);

        if(is_success)
        {
            is_success &= appMqtt_SubscribeAllTopics();
        }
        break;

    case WIFI_DISCONNECTED:
        break;

    default:
        break;
    }
    
    return 0;
}

static int appMqtt_MqttEventHandle(uint32_t event_code,
                                    void * param1,
                                    void * param2)
{
    uint32_t test_data = 0xbeefcafe;
    switch((MQTT_EVENT)event_code)
    {
    case EVENT_CONNECTED:
        TRACE("EVENT_CONNECTED");
        break;
    case EVENT_DISCONNECTED:
        TRACE("EVENT_DISCONNECTED");
        break;
    case EVENT_SUBSCRIBED:
        TRACE("EVENT_SUBSCRIBED");
        break;
    case EVENT_UNSUBSCRIBED:
        TRACE("EVENT_UNSUBSCRIBED");
        break;
    case EVENT_PUBLISHED:
        TRACE("EVENT_PUBLISHED");
        break;
    case EVENT_DATA_RECEIVED:
        TRACE("EVENT_DATA_RECEIVED");
        break;
    case EVENT_ERROR:
        TRACE("EVENT_ERROR");
        break;
    case EVENT_UNKNOWN:
        TRACE("EVENT_UNKNOWN");
        break;
    default:
        break;
    }

    return 0;
}
/* End of file*/

