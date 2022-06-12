
/*******************************************************************************
* Filename:         wifi_controller.h
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
#include "app/app_config.h"

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

/** @brief definition of a new type
 *  
 */
typedef enum
{
    WIFI_CONNECTING,    /**< Connecting */
    WIFI_CONNECTED,     /**< Connecting success */
    WIFI_SCANNING,      /**< Scanning for availalbe networks*/
    WIFI_SCAN_RESULT,   /**< Scan complete, return results */
    WIFI_DISCONNECTED,  /**< Disconnected */
}WIFI_EVENT;

/** @brief definition of api set for Wifi controller
 *
 */
typedef struct
{
    bool (*WiFiCtrl_Connect)(const char * ssid, uint32_t ssid_size,
                             const char * pwd, uint32_t pwd_size);  /**< pointer to the fucntion connecting to a network */
    char *(*WiFiCtrl_GetStoredSsid)(void);                    /**< pointer to the fucntion returning the stored ssid */
    bool (*WifiCtrl_Disconnect)(void);                              /**< pointer to the fucntion disconnecting from a network */
    bool (*WifiCtrl_Scan)(void);                                    /**< pointer to the fucntion scanning for available networks */
    WIFI_EVENT (*WifiCtrl_GetEvent)(void);                          /**< pointer to the fucntion returning event */
}WiFiCtrlDriverApi;

typedef uint32_t (*INTERRUPT_CALLBACK)(uint32_t event_code, void* param1, void* param2);

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void* WifiCtrl_GetWifiControllerDriver(void);

#endif /* WIFI_CONTROLLER */

#endif /* _WIFI_CONTROLLER_H */

/* End of file */

