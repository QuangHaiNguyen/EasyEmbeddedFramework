
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
 *  @brief  This is the source for the esp32 wifi driver
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
#define DEFAULT_SCAN_LIST_SIZE   10U


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
static wifi_ap_record_t esp_ap_info[DEFAULT_SCAN_LIST_SIZE];
static WifiAccesPointInfo sdk_ap_info[DEFAULT_SCAN_LIST_SIZE];


/******************************************************************************
* Function Definitions
*******************************************************************************/
static void wifiEsp_WifiEventHandler(void* arg, esp_event_base_t event_base,
                                     int32_t event_id, void* event_data);

static void wifiEsp_IPEventHandler(void* arg, esp_event_base_t event_base,
                                   int32_t event_id, void* event_data);

static bool wifiEsp_Connect(const char * ssid, uint32_t ssid_size,
                            const char * pwd, uint32_t pwd_size);

static void wifiEsp_GetStoredSsid(char ** ssid);
static bool wifiEsp_Disconnect(void);
static bool wifiEsp_Scan(void);

static void  WifiCtrl_ConvertFromEspApInfo(WifiAccesPointInfo * sdk_info, 
                                           wifi_ap_record_t * esp_info);

static HAL_WIFI_EVENT   wifiEsp_GetEvent(void);
static WIFI_SEC_MODE    WifiCtrl_ConvertFromEspSec( wifi_auth_mode_t sec_mode);

/******************************************************************************
* External functions
*******************************************************************************/

/*******************************************************************************
* Function : wifiEsp_Initialization
*//** 
* @Description:
*
* This function initializes the wifi low layer driver
* 
* @param    None
*
* @return   true: success
*           false: fail
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
                                                        &wifiEsp_WifiEventHandler,
                                                        NULL,
                                                        &instance_any_id);

        err_code |= esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifiEsp_IPEventHandler,
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
        err_code |= esp_wifi_set_config(WIFI_IF_STA, &wifi_component.wifi_config);
        err_code |= esp_wifi_start();

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


/*******************************************************************************
* Function : wifiEsp_BindingDriverApi
*//** 
* @Description:
*
* This function binds the low level driver to the upper layer driver
* 
* @param    **api: (OUT)pointer to the driver pointer
*
* @return   true: success
*           false: fail
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
* Function : wifiEsp_RegisterInterruptCallback
*//** 
* @Description:
*
* This function allows the upper layer to register a callback function to handle
* the interrupt
* 
* @param    callback: (IN)pointer to the callback function
*
* @return   true: success
*           false: fail
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

/******************************************************************************
* Function : wifiEsp_WifiEventHandler
*//** 
* @Description:
*
* This function handle the wifi event from the esp32
* 
* @param    arg: (IN)pointer to the arguments
* @param    event_base: (IN)base of the event
* @param    event_id: (IN)id of the event
* @param    event_data: (IN)data of the event
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
static void wifiEsp_WifiEventHandler(   void* arg, 
                                        esp_event_base_t event_base,
                                        int32_t event_id,
                                        void* event_data)
{
    TRACE("something is triggered [event base % s], [id = %d]",
            event_base, event_id);

    esp_err_t err_code = ESP_OK;

    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        wifi_component.is_sta_mode_started = true;
        DEBUG("wifi STA mode hase been started");
        
        wifi_component.event = WIFI_STA_START;
        if(wifi_component.wifi_callback)
        {
            wifi_component.wifi_callback(wifi_component.event, NULL, NULL);
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_STOP) 
    {
        DEBUG("wifi STA mode hase been started");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) 
    {
        DEBUG("wifi STA mode hase been started");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        DEBUG("wifi STA mode hase been started");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) 
    {
        uint16_t ap_count = 0;
        uint16_t number = DEFAULT_SCAN_LIST_SIZE;

        DEBUG("wifi scan completed");
        memset(esp_ap_info, 0, sizeof(esp_ap_info));

        err_code = esp_wifi_scan_get_ap_records(&number, esp_ap_info);
        err_code |= esp_wifi_scan_get_ap_num(&ap_count);

        if(err_code == ESP_OK)
        {
            ap_count = MIN(ap_count, number);
            for(uint8_t i = 0; i < ap_count; i++)
            {
                DEBUG("[%d] - [ssid = %s]", i, esp_ap_info[i].ssid);
                WifiCtrl_ConvertFromEspApInfo(&sdk_ap_info[i], &esp_ap_info[i]);
            }

            wifi_component.event = WIFI_SCAN_RESULT;
            if(wifi_component.wifi_callback)
            {
                wifi_component.wifi_callback(wifi_component.event, &ap_count, &sdk_ap_info[0]);
            }

            wifi_component.event = WIFI_CONNECTED;
        }

    }
    else
    {
    }
}


/******************************************************************************
* Function : wifiEsp_IPEventHandler
*//** 
* @Description:
*
* This function handle the ID event from the esp32
* 
* @param    arg: (IN)pointer to the arguments
* @param    event_base: (IN)base of the event
* @param    event_id: (IN)id of the event
* @param    event_data: (IN)data of the event
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
static void wifiEsp_IPEventHandler( void* arg,
                                    esp_event_base_t event_base,
                                    int32_t event_id,
                                    void* event_data)
{
    TRACE("something is triggered [event base % s], [id = %d]",
            event_base, event_id);

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        char ip[20] = {0};
        
        sprintf(ip, IPSTR, IP2STR(&event->ip_info.ip));

        INFO("wifi connected [ip = %s]", ip);
        wifi_component.event = WIFI_CONNECTED;
        if(wifi_component.wifi_callback)
        {
            wifi_component.wifi_callback(wifi_component.event, 
                                            (void*)ip, 
                                            (void*)wifi_component.wifi_config.sta.ssid);
        }
    }
}


/******************************************************************************
* Function : wifiEsp_Connect
*//** 
* @Description:
*
* This function connects the esp to an AP. Info of the AP is specified in the
* arguments
* 
* @param    ssid: (IN)pointer to AP's ssid
* @param    ssid_size: (IN)size of the ssid
* @param    pwd: (IN)pointer to the password
* @param    pwd_size: (IN)size of the password
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool wifiEsp_Connect(const char * ssid, uint32_t ssid_size,
                            const char * pwd, uint32_t pwd_size)
{
    bool is_success = false;
    esp_err_t err_code = ESP_OK;

    TRACE("wifiEsp_Connect()");
    TRACE("[ssid = %s]", ssid);
    TRACE("[pwd = %s]", pwd);

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

        err_code = err_code | esp_wifi_set_config(WIFI_IF_STA, &wifi_component.wifi_config);
        err_code = esp_wifi_connect();

        if(err_code == ESP_OK)
        {
            is_success = true;
            wifi_component.event = WIFI_CONNECTING;
            TRACE("trying to connect to...");
            TRACE("[ssid = %s]", wifi_component.wifi_config.sta.ssid);
        }
        else
        {
            ERROR("connection error [code = %d]", err_code);
        }
    }

    if(is_success && wifi_component.wifi_callback)
    {
        wifi_component.wifi_callback(wifi_component.event, 
                                    (void*)wifi_component.wifi_config.sta.ssid, 
                                    NULL);
    }

    return is_success;
}


/******************************************************************************
* Function : wifiEsp_GetStoredSsid
*//** 
* @Description:
*
* This function returns the current ID stored in cache
* 
* @param    ssid: (IN)pointer to ssid
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
static void wifiEsp_GetStoredSsid(char ** ssid)
{
    *ssid = (char *)(&wifi_component.wifi_config.sta.ssid);
}


/******************************************************************************
* Function : wifiEsp_Disconnect
*//** 
* @Description:
*
* This function disconnects the esp wifi from the connected AP
* 
* @param    None
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
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
        else
        {
            wifi_component.event = WIFI_DISCONNECTED;
            wifi_component.wifi_callback(wifi_component.event, 
                                        (void*)wifi_component.wifi_config.sta.ssid, 
                                        NULL);
        }
    }

    return is_success;
}


/******************************************************************************
* Function : wifiEsp_Scan
*//** 
* @Description:
*
* This function scans the available networks
* 
* @param    None
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool wifiEsp_Scan(void)
{
    bool is_success = false;

    if(esp_wifi_scan_start(NULL, false) != ESP_OK)
    {
        WARNING("cannot scan");
        is_success = false;
    }
    else
    {
        wifi_component.event = WIFI_SCANNING;
        wifi_component.wifi_callback(wifi_component.event, 
                                    NULL, 
                                    NULL);
    }
    

    return is_success;
}


/******************************************************************************
* Function : wifiEsp_GetEvent
*//** 
* @Description:
*
* This function return thr current event/status of the wifi driver
* 
* @param    None
*
* @return   wifi event
*
*******************************************************************************/
static HAL_WIFI_EVENT wifiEsp_GetEvent(void)
{
    return wifi_component.event;
}


/******************************************************************************
* Function : WifiCtrl_ConvertFromEspApInfo
*//** 
* @Description:
*
* This function converts esp ap info structure to embedded sdk wifi structure
* 
* @param    sdk_info: (OUT)pointer to sdk struture
* @param    esp_info: (IN)pointer to esp struture
*
* @return   None
*
*******************************************************************************/
static void  WifiCtrl_ConvertFromEspApInfo( WifiAccesPointInfo * sdk_info, 
                                            wifi_ap_record_t * esp_info)
{
    sdk_info->rssi = esp_info->rssi;
    sdk_info->channel = esp_info->primary;
    sdk_info->sec_mode = WifiCtrl_ConvertFromEspSec(esp_info->authmode);
    strncpy(sdk_info->bssid, (char*)esp_info->bssid, 6);
    strncpy(sdk_info->ssid, (char*)esp_info->ssid, 33);
}


/******************************************************************************
* Function : WifiCtrl_ConvertFromEspSec
*//** 
* @Description:
*
* This function converts esp security to embedded sdk security mode
* 
* @param    sec_mode: (IN)esp security mode
*
* @return   sdk wifi security mode
*
*******************************************************************************/
static WIFI_SEC_MODE  WifiCtrl_ConvertFromEspSec( wifi_auth_mode_t sec_mode)
{
    WIFI_SEC_MODE ret_sec_mode = WIFI_SEC_UNKNOWN;

    switch(sec_mode)
    {
    case WIFI_AUTH_OPEN:            ret_sec_mode = WIFI_SEC_OPEN;       break;
    case WIFI_AUTH_WEP:             ret_sec_mode = WIFI_SEC_WEP;        break;
    case WIFI_AUTH_WPA_PSK:         ret_sec_mode = WIFI_SEC_WPA;        break;
    case WIFI_AUTH_WPA2_PSK:        ret_sec_mode = WIFI_SEC_WPA2;       break;
    case WIFI_AUTH_WPA_WPA2_PSK:    ret_sec_mode = WIFI_SEC_WPA2_MIX;   break;
    case WIFI_AUTH_WPA2_ENTERPRISE: ret_sec_mode = WIFI_SEC_WPA2_ENTERPRISE;    break;
    case WIFI_AUTH_WPA3_PSK:        ret_sec_mode = WIFI_SEC_WPA3;       break;
    case WIFI_AUTH_WPA2_WPA3_PSK:   ret_sec_mode = WIFI_SEC_WPA3_MIX;   break;
    case WIFI_AUTH_WAPI_PSK:        ret_sec_mode = WIFI_SEC_WAPI;       break;
    default:                        ret_sec_mode = WIFI_SEC_UNKNOWN;    break;
    }

    return ret_sec_mode;
}
#endif /* WIFI_CONTROLLER == 1U && SUPPORTED_CHIP == ESP32 */

/* End of file*/

