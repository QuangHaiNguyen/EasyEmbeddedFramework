
/*******************************************************************************
* Filename:         wifi_controller.h
* Author:           Hai Nguyen
* Original Date:    12.06.2022
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
* Written by Hai Nguyen 12.06.2022
*
*******************************************************************************/

/** @file   wifi_controller.h
 *  @author Hai Nguyen
 *  @date   12.06.2022
 *  @brief  Contain the public api of wifi controller module.
 *  
 *  @details These api are not used direct by user but they are registered to
 *           the driver module, which will be used by the user
 * 
 */

#ifndef _WIFI_CONTROLLER_H
#define _WIFI_CONTROLLER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezApp/ezSdk_config.h"

#if(WIFI_CONTROLLER == 1U)
#include <stdbool.h>
#include <stdint.h>


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief WIFI status event
 *  
 */
typedef enum
{
    WIFI_STA_START,     /**< STA started */
    WIFI_STA_STOP,      /**< STA stopped */
    WIFI_CONNECTING,    /**< Connecting */
    WIFI_CONNECTED,     /**< Connecting success */
    WIFI_SCANNING,      /**< Scanning for availalbe networks*/
    WIFI_SCAN_RESULT,   /**< Scan complete, return results */
    WIFI_DISCONNECTED,  /**< Disconnected */
}HAL_WIFI_EVENT;


/** @brief Security mode enumeration
 *
 */
typedef enum
{
    WIFI_SEC_OPEN,              /**< No security */
    WIFI_SEC_WEP,               /**< WEP */
    WIFI_SEC_WPA,               /**< WPA */
    WIFI_SEC_WPA2,              /**< WPA2 */
    WIFI_SEC_WPA2_MIX,          /**< WPA2 mix */
    WIFI_SEC_WPA2_ENTERPRISE,   /**< WPA2 enterprise */
    WIFI_SEC_WPA3,              /**< WPA3 */
    WIFI_SEC_WPA3_MIX,          /**< WPA3 mix */
    WIFI_SEC_WAPI,              /**< WAPI */
    WIFI_SEC_UNKNOWN,           /**< Security mode unknown */
}WIFI_SEC_MODE;


/** @brief API set for Wifi controller
 *
 */
typedef struct
{
    bool (*WiFiCtrl_Connect)(const char * ssid, uint32_t ssid_size,
                             const char * pwd, uint32_t pwd_size);  /**< pointer to the fucntion connecting to a network */
    void (*WiFiCtrl_GetStoredSsid)(char ** ssid);                   /**< pointer to the fucntion returning the stored ssid */
    bool (*WifiCtrl_Disconnect)(void);                              /**< pointer to the fucntion disconnecting from a network */
    bool (*WifiCtrl_Scan)(void);                                    /**< pointer to the fucntion scanning for available networks */
    HAL_WIFI_EVENT (*WifiCtrl_GetEvent)(void);                      /**< pointer to the fucntion returning event */
}WiFiCtrlDriverApi;


/** @brief information of an access point
 *
 */
typedef struct
{
    uint8_t channel;        /**< Channel */
    int8_t rssi;            /**< RSSI */
    char bssid[6];          /**< BSSID */
    char ssid[33];          /**< SSID */
    WIFI_SEC_MODE sec_mode; /**< Wifi security mode */
}WifiAccesPointInfo;


/** @brief Interrupt callback function pointer
 *
 */
typedef uint32_t (*INTERRUPT_CALLBACK)(uint32_t event_code,
                                       void* param1,
                                       void* param2);

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void* WifiCtrl_GetWifiControllerDriver(void);

#endif /* WIFI_CONTROLLER == 1U */

#endif /* _WIFI_CONTROLLER_H */

/* End of file */

