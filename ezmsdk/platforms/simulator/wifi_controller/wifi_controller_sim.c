
/*******************************************************************************
* Filename:         wifi_controller_sim.c
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

/** @file   wifi_controller_sim.c
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
#include "wifi_controller_sim.h"

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "WIFI_SIM"       /**< module name */

#include <string.h>

#include "utilities/logging/logging.h"
#include "utilities/ezmAssert/ezmAssert.h"

/*the rest of include go here*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/** @brief structure containing the required data for wifi component
 *
 */
struct WiFiComponent
{
    WIFI_EVENT event;                   /**< Current event of the mnodule */
    INTERRUPT_CALLBACK wifi_callback;   /**< pointer to interrupt handler */
    char ssid[256];                     /**< ssid */
    char pwd[256];                      /**< password */
    WiFiCtrlDriverApi api;              /**< the real api */
};

static struct WiFiComponent wifi_component;

/******************************************************************************
* Function Definitions
*******************************************************************************/
static bool         wifiSim_Connect(const char * ssid, uint32_t ssid_size,
                                         const char * pwd, uint32_t pwd_size);
static void         wifiSim_GetStoredSsid(char ** ssid);
static bool         wifiSim_Disconnect(void);
static bool         wifiSim_Scan(void);
static WIFI_EVENT   wifiSim_GetEvent(void);

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : wifiSim_Initialization
*//** 
* @Description:
*
* This function initializes the wifi hardware component
* 
* @param    None
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool wifiSim_Initialization(void)
{
    bool is_success = true;

    wifi_component.event = WIFI_DISCONNECTED;
    memset(wifi_component.ssid, 0, sizeof(wifi_component.ssid));
    memset(wifi_component.pwd, 0, sizeof(wifi_component.pwd));

    INFO("Hardware wifi is initialized");

    return is_success;
}

/******************************************************************************
* Function : wifiSim_BindingDriverApi
*//**
* @Description:
*
* This function binds the hardware wifi component api to the hal api
*
* @param    api: (IN)pointer to the api
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool wifiSim_BindingDriverApi(void ** api)
{
    bool is_success = true;

    INFO("wifiSim_BindingDriverApi()");

    WiFiCtrlDriverApi * wifi_api = (WiFiCtrlDriverApi*)api;

    wifi_component.api.WiFiCtrl_Connect = wifiSim_Connect;
    wifi_component.api.WifiCtrl_Disconnect = wifiSim_Disconnect;
    wifi_component.api.WifiCtrl_GetEvent = wifiSim_GetEvent;
    wifi_component.api.WiFiCtrl_GetStoredSsid = wifiSim_GetStoredSsid;
    wifi_component.api.WifiCtrl_Scan = wifiSim_Scan;

    *api = (void*)(&wifi_component.api);

    return is_success;
}

/******************************************************************************
* Internal functions
*******************************************************************************/

/******************************************************************************
* Function : wifiSim_Connect
*//**
* @Description:
*
* This function simulates connect function 
*
* @param    *ssid: (IN)pointer to ssid string
* @param    ssid_size: (IN)size of ssid string
* @param    *pwd: (IN)pointer to pwd string
* @param    pwd_size: (IN)size of pwd string
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool wifiSim_Connect(const char* ssid, uint32_t ssid_size,
                            const char* pwd, uint32_t pwd_size)
{
    bool is_success = false;

    if (ssid != NULL && pwd != NULL)
    {
        if(ssid_size > sizeof(wifi_component.ssid))
        {
           ssid_size =  sizeof(wifi_component.ssid);
        }
        strncpy(wifi_component.ssid, ssid, ssid_size);
        
        if(pwd_size > sizeof(wifi_component.pwd))
        {
           pwd_size =  sizeof(wifi_component.pwd);
        }
        strncpy(wifi_component.pwd, pwd, pwd_size);

        /* mock callback function */
        INFO("WIFI is connecting");
        if (wifi_component.wifi_callback)
        {
            wifi_component.wifi_callback(WIFI_CONNECTING, NULL, NULL);
            wifi_component.event = WIFI_CONNECTING;
        }

        /* mock callback function */
        INFO("WIFI is connected");
        if (wifi_component.wifi_callback)
        {
            wifi_component.wifi_callback(WIFI_CONNECTED, NULL, NULL);
            wifi_component.event = WIFI_CONNECTED;
        }

        is_success = true;
    }

    return is_success;
}

/******************************************************************************
* Function : wifiSim_GetStoredSsid
*//**
* @Description:
*
* This function returns the ssid stored by the hw component
*
* @param    *ssid: (OUT)pointer to the ring buffer
* @return   None
*
*******************************************************************************/
static void  wifiSim_GetStoredSsid(char ** ssid)
{
    *ssid =  wifi_component.ssid;
}

/******************************************************************************
* Function : wifiSim_Disconnect
*//**
* @Description:
*
* This function disconnects from a connected access point
*
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool wifiSim_Disconnect(void)
{
    bool is_success = true;

    INFO("WIFI is disconnected");
    wifi_component.event = WIFI_DISCONNECTED;
    memset(wifi_component.ssid, 0, sizeof(wifi_component.ssid));
    memset(wifi_component.pwd, 0, sizeof(wifi_component.pwd));

    /* mock callback function */
    if (wifi_component.wifi_callback)
    {
        wifi_component.wifi_callback(WIFI_DISCONNECTED, NULL, NULL);
    }

    return is_success;
}

/******************************************************************************
* Function : wifiSim_Scan
*//**
* @Description:
*
* This function simulates the scan operation
*
* @param    None
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool wifiSim_Scan(void)
{
    bool is_success = true;

    INFO("starts scanning");
    wifi_component.event = WIFI_SCANNING;

    /* mock callback function */
    if (wifi_component.wifi_callback)
    {
        wifi_component.wifi_callback(WIFI_SCANNING, NULL, NULL);
    }

    /* mock callback function */
    if (wifi_component.wifi_callback)
    {
        wifi_component.event = WIFI_SCAN_RESULT;
        wifi_component.wifi_callback(WIFI_SCAN_RESULT, NULL, NULL);
    }

    return is_success;
}

/******************************************************************************
* Function : wifiSim_GetEvent
*//**
* @Description:
*
* This function returns the current event of the hardware component
*
* @param    None
* @return   WIFI_EVENT
*
*******************************************************************************/
static WIFI_EVENT wifiSim_GetEvent(void)
{
    INFO("[wifi event = %d]", wifi_component.event);
    return wifi_component.event;
}

/******************************************************************************
* Function : wifiSim_RegisterInterruptCallback
*//**
* @Description:
*
* This function registers the handler for interrupt
*
* @param    callback: (IN)pointer to the function handler
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool wifiSim_RegisterInterruptCallback(INTERRUPT_CALLBACK callback)
{
    bool is_success = true;

    ASSERT_MSG(callback != NULL, "call back is null");

    wifi_component.wifi_callback = callback;

    return is_success;
}

/* End of file*/

