
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
#include "app/app_config.h"

#include <stdint.h>
#include <stdbool.h>

#if (MQTT == 1U)
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief MQTT event enumeration
 *
 */
typedef enum
{
    EVENT_STARTED,          /**< MQTT started */
    EVENT_STOPPED,          /**< MQTT stopped */
    EVENT_CONNECTED,        /**< MQTT connected */
    EVENT_DISCONNECTED,     /**< MQTT disconnected */
    EVENT_SUBSCRIBED,       /**< MQTT subscribed */
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
    const char * broker_adr;            /**< */
    const char * username;              /**< */
    const char * pwd;                   /**< */
    uint16_t port;                      /**< */
    bool is_tls_activate;               /**< */
    MQTT_TRANSPORT_TYPE transport_type; /**< */
    void * tls_config;                  /**< */
}MqttConfig;


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
    void (*Mqtt_Config)     (MqttConfig * config);  /**< */
    void (*Mqtt_Start)      (void);                 /**< */
    void (*Mqtt_Stop)       (void);                 /**< */
    void (*Mqtt_Connect)    (void);                 /**< */
    void (*Mqtt_Disconnect) (void);                 /**< */
    void (*Mqtt_Subscribe)  (const char* topic);    /**< */
    void (*Mqtt_Unsubscribe)(const char* topic);    /**< */
    void (*Mqtt_Publish)    (const char* topic,
                             void* data,
                             uint32_t size);        /**< */
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

