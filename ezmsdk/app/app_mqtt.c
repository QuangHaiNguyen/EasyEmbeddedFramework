
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

#include <string.h>

#include "utilities/logging/logging.h"
#include "utilities/event_notifier/event_notifier.h"

#include "hal/network/mqtt/mqtt.h"
#include "hal/wifi_controller/wifi_controller.h"

#include "ezmDriver/driver.h"
#include "ezmKernel/ezmKernel.h"

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
    bool(*SetData)(char *in);
}MqttDataPoint;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static const char * broker = "pi-app-server";

static MqttConfig mqtt_config = {0};
static event_observer wifi_observer;
static event_observer mqtt_observer;
static MqttDriverApi * mqtt_drv;
static EzmProcess mqtt_start_proc;
static EzmProcess mqtt_stop_proc;

/******************************************************************************
* Function Definitions
*******************************************************************************/
static bool appMqtt_SubscribeAllTopics(void);
static DATABASE_INDEX appMqtt_GetIndexFromTopic(const char * topic);

static int appMqtt_WifiEventHandle(uint32_t event_code,
                                    void * param1,
                                    void * param2);

static int appMqtt_MqttEventHandle(uint32_t event_code,
                                    void * param1,
                                    void * param2);

static bool appMqtt_SetDataOnOff(char *in);
static bool appMqtt_SetDataWaterLevel(char *in);
static bool appMqtt_SetDataLastWater(char *in);

static uint8_t appMqtt_MqttStopProcess(void);
static uint8_t appMqtt_MqttStartProcess(void);
/******************************************************************************
* Lookup table
*******************************************************************************/
MqttDataPoint database[] = {
    {"smarthome/balcony/water_sys/onoff/", DATA_POINT_INVALID, appMqtt_SetDataOnOff},
    {"smarthome/balcony/water_sys/water_level/", DATA_POINT_INVALID, appMqtt_SetDataWaterLevel},
    {"smarthome/balcony/water_sys/last_water/", DATA_POINT_INVALID, appMqtt_SetDataLastWater},
    {"smarthome/balcony/water_sys/sys_temperature/", DATA_POINT_INVALID, NULL},
    {"smarthome/balcony/water_sys/heartbeat/", DATA_POINT_INVALID, NULL},
};
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

        ezmDriver_ReleaseDriverInstance(MQTT_DRIVER);
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
    TRACE("[index = %d]", index);

    if(index < NUM_OF_INDEX && 
       database[index].data_point != DATA_POINT_INVALID)
    {
        is_success = DataModel_WriteDataPoint(database[index].data_point, 
                                              data, 
                                              size);
        
        if(is_success)
        {
            ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);
        
            if(mqtt_drv != NULL)
            {
                TRACE("got driver, try to publish data...");
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
        else
        {
            ERROR("cannot write data");
        }
    }
    else
    {
        ERROR("Sanity check failed");
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


bool appMqtt_SetDataByTopic(const char * topic, void* data, uint32_t size)
{
    bool is_success = false;

    TRACE("appMqtt_SetDataByTopic()");

    if(topic != NULL && data != NULL && size > 0)
    {
        for(uint32_t i = 0; i < NUM_OF_INDEX; i++)
        {
            if(strcmp(topic, database[i].topic) == 0 && 
                database[i].data_point != DATA_POINT_INVALID)
            {
                is_success = DataModel_WriteDataPoint(database[i].data_point, 
                                                        data, 
                                                        size);

                DEBUG("write data at [topic = %s], data:", topic);
                HEXDUMP(data, size);
                break;
            }
        }
    }
    return is_success;
}


/******************************************************************************
* Internal functions
*******************************************************************************/

static bool appMqtt_SubscribeAllTopics(void)
{
    TRACE("appMqtt_SubscribeAllTopics()");

    char set_topic[128] = {0};
    bool is_success = true;

    ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);

    if(mqtt_drv != NULL)
    {
        for (uint32_t i = 0; i < NUM_OF_INDEX; i++)
        {
            sprintf(set_topic, "%s%s\0", TOPIC_SET, database[i].topic);
            is_success &=  mqtt_drv->Mqtt_Subscribe(set_topic);
 
            if(is_success)
            {
                TRACE("success subscribe [topic = %s]", set_topic);
            }
            else
            {
                ERROR("subscribe fail");
            }
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
            if(mqtt_drv->Mqtt_Connect() == false)
            {
                ERROR("MQTT connect fail");
            }
        }
        else
        {
            ERROR("cannot connect to the broker");
        }
        ezmDriver_ReleaseDriverInstance(MQTT_DRIVER);

        break;

    case WIFI_DISCONNECTED:
        ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);
        if(mqtt_drv != NULL)
        {
            if(mqtt_drv->Mqtt_Disconnect() == false)
            {
                ERROR("MQTT disconnect fail");
            }
        }
        else
        {
            ERROR("cannot connect to the broker");
        }
        ezmDriver_ReleaseDriverInstance(MQTT_DRIVER);
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
    switch((MQTT_EVENT)event_code)
    {
    case EVENT_CONNECTED:
    {
        TRACE("EVENT_CONNECTED");
        if(appMqtt_SubscribeAllTopics() == false)
        {
            ERROR("subscribe failed");
        }
        break;
    }

        
    case EVENT_DISCONNECTED:
        TRACE("EVENT_DISCONNECTED");
        ezmKernel_AddProcess(&mqtt_stop_proc, PROC_ONCE, 200, appMqtt_MqttStopProcess);
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
    {
        TRACE("EVENT_DATA_RECEIVED");
        char topic[128] = {0};
        char value[128] = {0};
        uint32_t string_len = 0;
        DATABASE_INDEX index = NUM_OF_INDEX;

        DataEvent * event = (DataEvent *)param1;

        /* Get topic string */
        string_len =  event->topic_size;
        if(string_len > sizeof(topic))
        {
            string_len =  sizeof(topic) - 1;
        }
        memcpy(topic, event->topic, string_len);
        TRACE("[topic = %s]", topic);

        /* Get value string */
        string_len =  event->data_size;
        if(string_len > sizeof(value))
        {
            string_len =  sizeof(topic) - 1;
        }
        memcpy(value, event->data, string_len);
        TRACE("[value = %s]", value);

        /* omit the characters SET/*/
        index = appMqtt_GetIndexFromTopic(&topic[4]);
        if(index < NUM_OF_INDEX && database[index].SetData != NULL)
        {
            TRACE("OK, [topic = %s] found", topic);
            if(database[index].SetData(value) == false)
            {
                WARNING("Cannot set data, are u sure data point is avail");
            }
        }

        break;
    }

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

static DATABASE_INDEX appMqtt_GetIndexFromTopic(const char * topic)
{
    DATABASE_INDEX index = NUM_OF_INDEX;

    TRACE("appMqtt_GetIndexFromTopic()");

    if(topic != NULL)
    {
        for(uint32_t i = 0; i < NUM_OF_INDEX; i++)
        {
            if(strcmp(topic, database[i].topic) == 0)
            {
                index = i;
                break;
            }
        }
    }

    return index;
}

static bool appMqtt_SetDataOnOff(char * in)
{
    bool is_success = false;
    uint32_t result = 0;
    char set_topic[128] = {0};

    TRACE("appMqtt_SetDataOnOff()");

    if(in != NULL)
    {
        result = atoi(in);
        is_success = DataModel_WriteDataPoint(database[WATER_SYS_ONOFF].data_point, 
                                                (void*)&result, 
                                                sizeof(result));
    }

    if(is_success)
    {
        ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);
        if(mqtt_drv != NULL)
        {
            sprintf(set_topic, "%s%s", TOPIC_STATUS, database[WATER_SYS_ONOFF].topic);
            is_success &= mqtt_drv->Mqtt_Publish(set_topic, &result, sizeof(result));
        }
        else
        {
            ERROR("cannot publish data");
        }
        ezmDriver_ReleaseDriverInstance(MQTT_DRIVER);
    }

    return is_success;
}

static bool appMqtt_SetDataWaterLevel(char *in)
{
    bool is_success = false;


    TRACE("appMqtt_SetDataWaterLevel()");

    if(in != NULL)
    {
        WARNING("someone want to change the water level, reject anyway");
    }

    return is_success;
}


static bool appMqtt_SetDataLastWater(char *in)
{
    bool is_success = false;

    TRACE("appMqtt_SetDataWaterLevel()");

    if(in != NULL)
    {
        WARNING("someone want to change the last time water, reject anyway");
    }

    return is_success;
}

static uint8_t appMqtt_MqttStopProcess(void)
{
    DEBUG("Stop mqtt, try to reconnect in 10s");
    ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);
    if(mqtt_drv != NULL)
    {
        if(mqtt_drv->Mqtt_Disconnect() == false)
        {
            ERROR("MQTT disconnect fail");
        }
    }
    else
    {
        ERROR("cannot get driver");
    }
    ezmDriver_ReleaseDriverInstance(MQTT_DRIVER);

    ezmKernel_AddProcess(&mqtt_start_proc, PROC_ONCE, 10000, appMqtt_MqttStartProcess);

    return 0;
}

static uint8_t appMqtt_MqttStartProcess(void)
{
    DEBUG("restart mqtt");
    ezmDriver_GetDriverInstance(MQTT_DRIVER, (void**)&mqtt_drv);
    if(mqtt_drv != NULL)
    {
        if(mqtt_drv->Mqtt_Connect() == false)
        {
            ERROR("MQTT connect fail");
        }
    }
    else
    {
        ERROR("cannot get driver");
    }
    ezmDriver_ReleaseDriverInstance(MQTT_DRIVER);
    return 0;
}
/* End of file*/

