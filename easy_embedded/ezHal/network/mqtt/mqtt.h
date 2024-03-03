
/*******************************************************************************
* Filename:         mqtt.h
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

/** @file   mqtt.h
 *  @author Hai Nguyen
 *  @date   19.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _MQTT_H
#define _MQTT_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezApp/ezSdk_config.h"

#include <stdint.h>
#include <stdbool.h>

#if (MQTT == 1U)
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief MQTT event enumeration
 *
 */
typedef enum
{
    EVENT_CONNECTED,        /**< MQTT connected */
    EVENT_DISCONNECTED,     /**< MQTT disconnected */
    EVENT_SUBSCRIBED,       /**< MQTT subscribed */
    EVENT_UNSUBSCRIBED,     /**< MQTT unsubscribed */
    EVENT_PUBLISHED,        /**< MQTT published */
    EVENT_DATA_RECEIVED,    /**< MQTT data received */
    EVENT_ERROR,            /**< MQTT error */
    EVENT_UNKNOWN           /**< Unknown event */
}MQTT_EVENT;


/** @brief enumeration for transport protocol, they do not include the security
 *  option
 *
 */
typedef enum
{
    TCPIP,      /* using tcp/ip */
    WEBSOCK,    /* using websocket */
    HTTP,       /* using http */
}MQTT_TRANSPORT_TYPE;


/** @brief MQTT configuration data structure
 *
 */
typedef struct
{
    const char  *broker_adr;            /**< Address of the mqtt broker */
    const char  *username;              /**< username for login if applied */
    const char  *pwd;                   /**< password for login if applied */
    const char  *client_id;             /**< id of the mqtt client */
    uint16_t    port;                   /**< port */
    bool        is_tls_activate;        /**< flag if tls is used */
    MQTT_TRANSPORT_TYPE transport_type; /**< transport protocol: ws, http, tcp */
    void        *tls_config;            /**< store tls config, future feafute */
}MqttConfig;

/** @brief Hold data to notify the upper layer in EVENT_DATA_RECEIVED event 
 *
 */
typedef struct
{
    void * data;    /**< pointer to the data */
    uint32_t size;  /**< size of the data */
}DataEvent;


/** @brief event callback function pointer
 *
 */
typedef uint32_t(*MQTT_EVENT_CALLBACK)(uint32_t event_code,
                                       void* param1,
                                       void* param2);

/** @brief API set for Wifi controller
 *
 */
typedef struct
{
    bool (*Mqtt_Config)     (MqttConfig * config);  /**< config function */
    bool (*Mqtt_Connect)    (void);                 /**< connect to broker */
    bool (*Mqtt_Disconnect) (void);                 /**< disconnect from the broker */
    bool (*Mqtt_Subscribe)  (const char* topic);    /**< subscribe to a topic */
    bool (*Mqtt_Unsubscribe)(const char* topic);    /**< unsubscribe from a topic */
    bool (*Mqtt_Publish)    (const char* topic,
                             void* data,
                             uint32_t size);        /**< publish data to a topic */
}MqttDriverApi;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void * Mqtt_GetDriver(void);

#endif /* (MQTT == 1U) */
#endif /* _MQTT_H */

/* End of file */

