
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
 *  @brief  This is the source for a module
 *  
 *  @details
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
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *  
 */
typedef enum
{
    WIFI_CONNECTING,    /**< */
    WIFI_CONNECTED,     /**< */
    WIFI_SCANNING,      /**< */
    WIFI_SCAN_RESULT,   /**< */
    WIFI_DISCONNECTED,  /**< */
}WIFI_EVENT;

/** @brief definition of api set for UART
 *
 */
typedef struct
{
    bool (*WiFiCtrl_Connect)(const char * ssid, const char * pwd);  /**< */
    void (*WiFiCtrl_GetStoredSsid)(char * ssid);                    /**< */
    bool (*WifiCtrl_Disconnect)(void);                              /**< */
    bool (*WifiCtrl_Scan)(void);                                    /**< */
    WIFI_EVENT (*WifiCtrl_GetEvent)(void);                          /**< */
}WiFiCtrlDriverApi;

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

