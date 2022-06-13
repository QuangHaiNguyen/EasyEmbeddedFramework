
/*******************************************************************************
* Filename:         esp32_wifi_controller.c
* Author:           Hai Nguyen
* Original Date:    12.06.2022
* Last Update:      12.06.2022
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
* Written by Hai Nguyen 12.06.2022
*
*******************************************************************************/

/** @file   esp32_wifi_controller.c
 *  @author Hai Nguyen
 *  @date   12.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "esp32_wifi_controller.h"

#if(WIFI_CONTROLLER == 1U && SUPPORTED_CHIP == ESP32)

#define DEBUG_LVL   LVL_TRACE           /**< logging level */
#define MOD_NAME    "ESP_WIFI_CTRL"     /**< module name */

#include <string.h>

/* sdk section */
#include "utilities/logging/logging.h"
#include "utilities/ezmAssert/ezmAssert.h"

/* esp section */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define DEFAULT_SEC_THREHOLD     WIFI_AUTH_OPEN   /**< threshold for supported security mode*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/** @brief structure containing the required data for wifi component
 *
 */
struct WiFiComponent
{
    bool is_sta_mode_started;           /**< flag indicating sta mode has started */
    HAL_WIFI_EVENT event;               /**< Current event of the mnodule */
    INTERRUPT_CALLBACK wifi_callback;   /**< pointer to interrupt handler */
    wifi_config_t wifi_config;          /**< store wifi config */
    WiFiCtrlDriverApi api;              /**< the real api */
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct WiFiComponent wifi_component;

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data);

static bool wifiEsp_Connect(const char * ssid, uint32_t ssid_size,
                            const char * pwd, uint32_t pwd_size);

static void wifiEsp_GetStoredSsid(char ** ssid);
static bool wifiEsp_Disconnect(void);
static bool wifiEsp_Scan(void);
static HAL_WIFI_EVENT wifiEsp_GetEvent(void);
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
bool wifiEsp_Initialization(void)
{
    bool is_success = true;
    esp_err_t err_code = ESP_OK;
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;

    TRACE("wifiEsp_Initialization()");

    /* sdk section */
    wifi_component.event = WIFI_DISCONNECTED;
    wifi_component.is_sta_mode_started = false;

    /* esp section */
    if(esp_netif_init() != ESP_OK)
    {
        ERROR("esp_netif_init() failed");
        is_success = false;
    }

    if(is_success && esp_event_loop_create_default() != ESP_OK)
    {
        ERROR("esp_event_loop_create_default() failed");
        is_success = false;
    }


    if(is_success)
    {
        esp_netif_create_default_wifi_sta();
        if(esp_wifi_init(&cfg) != ESP_OK)
        {
            ERROR("esp_wifi_init() failed");
            is_success = false;
        }
    }

    if(is_success)
    {
        err_code =  esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id);

        err_code = err_code | esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip);

        if(err_code != ESP_OK)
        {
            ERROR("esp_event_handler_instance_register() failed");
            is_success = false;
        }
    }

    if(is_success)
    {
        err_code = esp_wifi_set_mode(WIFI_MODE_STA);
        err_code = err_code | esp_wifi_set_config(WIFI_IF_STA, &wifi_component.wifi_config);
        err_code = err_code | esp_wifi_start();

        if(err_code != ESP_OK)
        {
            ERROR("esp_wifi_set_mode() failed");
            is_success = false;
        }
    }

    if(is_success)
    {
        INFO("esp wifi init ok");
    }
    else
    {
        INFO("esp wifi init fail");
    }

    return is_success;
}


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
bool wifiEsp_BindingDriverApi(void ** api)
{
    bool is_success = true;

    INFO("Binding wifi api ok");

    wifi_component.api.WiFiCtrl_Connect = wifiEsp_Connect;
    wifi_component.api.WifiCtrl_Disconnect = wifiEsp_Disconnect;
    wifi_component.api.WifiCtrl_GetEvent = wifiEsp_GetEvent;
    wifi_component.api.WiFiCtrl_GetStoredSsid = wifiEsp_GetStoredSsid;
    wifi_component.api.WifiCtrl_Scan = wifiEsp_Scan;

    *api = (void*)(&wifi_component.api);

    return is_success;
}

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
bool wifiEsp_RegisterInterruptCallback(INTERRUPT_CALLBACK callback)
{
    bool is_success = true;

    wifi_component.wifi_callback = callback;

    return is_success;
}

/******************************************************************************
* Internal functions
*******************************************************************************/
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        wifi_component.is_sta_mode_started = true;
        INFO("wifi STA mode hase been started");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        char ip[20] = {0};
        uint8_t ip_size = 20;
        
        sprintf(ip, IPSTR, IP2STR(&event->ip_info.ip));

        INFO("wifi connected [ip = %s]", ip);
        wifi_component.event = WIFI_CONNECTED;
        if(wifi_component.wifi_callback)
        {
            wifi_component.wifi_callback(wifi_component.event, (void*)ip, (void*)&ip_size);
        }
    }
}

static bool wifiEsp_Connect(const char * ssid, uint32_t ssid_size,
                            const char * pwd, uint32_t pwd_size)
{
    bool is_success = false;
    esp_err_t err_code = ESP_OK;

    TRACE("wifiEsp_Connect()");
    TRACE("[ssid = %d]", ssid);
    TRACE("[pwd = %d]", pwd);

    if (ssid != NULL && pwd != NULL && wifi_component.is_sta_mode_started)
    {
        if(ssid_size > sizeof(wifi_component.wifi_config.sta.ssid))
        {
           ssid_size =  sizeof(wifi_component.wifi_config.sta.ssid);
        }
        strncpy((char *)wifi_component.wifi_config.sta.ssid, ssid, ssid_size);
        
        if(pwd_size > sizeof(wifi_component.wifi_config.sta.password))
        {
           pwd_size =  sizeof(wifi_component.wifi_config.sta.password);
        }
        strncpy((char *)wifi_component.wifi_config.sta.password, pwd, pwd_size);

        esp_wifi_connect();

        if(err_code == ESP_OK)
        {
            is_success = true;
            wifi_component.event = WIFI_CONNECTING;
            TRACE("trying to connect to...");
            TRACE("[ssid = %s]", wifi_component.wifi_config.sta.ssid);
        }
    }

    if(is_success && wifi_component.wifi_callback)
    {
        wifi_component.wifi_callback(wifi_component.event, NULL, NULL);
    }

    return is_success;
}

static void wifiEsp_GetStoredSsid(char ** ssid)
{
    *ssid = (char *)(&wifi_component.wifi_config.sta.ssid);
}

static bool wifiEsp_Disconnect(void)
{
    bool is_success = true;

    if  (wifi_component.is_sta_mode_started)
    {
        if(esp_wifi_disconnect() != ESP_OK)
        {
            WARNING("cannot disconnect from an AP");
            is_success = false;
        }
    }

    return is_success;
}

static bool wifiEsp_Scan(void)
{
    bool is_success = false;

    return is_success;
}

static HAL_WIFI_EVENT wifiEsp_GetEvent(void)
{
    return wifi_component.event;
}

#endif /* WIFI_CONTROLLER == 1U && SUPPORTED_CHIP == ESP32 */

/* End of file*/

