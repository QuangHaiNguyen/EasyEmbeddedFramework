
/*******************************************************************************
* Filename:         app_wifi_management.c
* Author:           Hai Nguyen
* Original Date:    13.06.2022
* Last Update:      13.06.2022
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
* Written by Hai Nguyen 13.06.2022
*
*******************************************************************************/

/** @file   app_wifi_management.c
 *  @author Hai Nguyen
 *  @date   13.06.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "app_wifi_management.h"

#if (WIFI_CONTROLLER == 1U && CLI == 1U)

#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "WIFI_MGMT"     /**< module name */

#include "string.h"

#include "utilities/logging/logging.h"
#include "utilities/ezmAssert/ezmAssert.h"

#include "ezmDriver/driver.h"
#include "hal/wifi_controller/wifi_controller.h"
#include "cli/cli.h"

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
WiFiCtrlDriverApi * wifi_drv = NULL;

static const char default_ssid[] = "BabySharkDooDoo";
static const char default_pwd[] = "HauffStr.24-2EtageLinks!";

/* CLI command section */
/* connect to an AP command */
static const char conn_cmd[] = "wifi_connect";
static const char conn_desc[] = "connect wifi to a AP";
static const char conn_long_ssid[] = "--ssid";
static const char conn_short_ssid[] = "-ssid";
static const char conn_ssid_desc[] = "name of access point";
static const char conn_long_pwd[] = "--password";
static const char conn_short_pwd[] = "-pwd";
static const char conn_pwd_desc[] = "passord of the access point";

/* connect default AP command */
static const char conn_cmd_default[] = "wifi_connect_default";
static const char conn_desc_default[] = "connect wifi to a default AP";

/* disconnect from an AP*/
static const char disconn_cmd[] = "wifi_disconnect";
static const char disconn_cmd_desc[] = "disconnect from the connected AP in cache";

/* disconnect from an AP*/
static const char scan_cmd[] = "wifi_scan";
static const char scan_cmd_desc[] = "scan for available APs";

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* wifi section */
static bool WiFiMgmt_GetWifiDriver(void);
static int WiFiMgmt_WifiEventHandle(uint32_t event_code,
                                    void * param1,
                                    void * param2);

/* CLI command section */
static bool WiFiMgmt_AddWifiCmd(void);
static CLI_NOTIFY_CODE WiFiMgmt_ConnCmdHandle(const char * pu8Command,
                                                void * pValueList);

static CLI_NOTIFY_CODE WiFiMgmt_ConnDefaultCmdHandle(const char * pu8Command,
                                                        void * pValueList);

static CLI_NOTIFY_CODE WiFiMgmt_DisconnCmdHandle(const char * pu8Command,
                                                        void * pValueList);

static CLI_NOTIFY_CODE WiFiMgmt_ScanCmdHandle(const char * pu8Command,
                                                        void * pValueList);
/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : wifiMgmt_Initialize
*//** 
* @Description:
*
* This function initializes the wifi management application module
* 
* @param    None
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool wifiMgmt_Initialize(void)
{
    TRACE("wifiMgmt_Initialize()");

    if(WiFiMgmt_GetWifiDriver() == false)
    {
        ERROR("cannot get wifi driver");
    }

    if(WiFiMgmt_AddWifiCmd() == false)
    {
        WARNING("cannot add commands to cli. wifi commands wont be available!");        
    }

    return true;
}

/******************************************************************************
* Internal functions
*******************************************************************************/

/******************************************************************************
* Function : WiFiMgmt_GetWifiDriver
*//** 
* @Description:
*
* This function initializes the wifi management application module
* 
* @param    None
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool WiFiMgmt_GetWifiDriver(void)
{
    bool is_success = true;

    TRACE("WiFiMgmt_GetWifiDriver()");

    ezmDriver_GetDriverInstance(WIFI_CTRL_DRIVER, &wifi_drv);

    if(wifi_drv == NULL)
    {
        is_success = false;
        WARNING("Wifi controller is busy, cannot get any instance");
    }

    if(is_success)
    {
        is_success &= ezmDriver_SubscribeDriverEvent(WIFI_CTRL_DRIVER, 
                                                    WiFiMgmt_WifiEventHandle);
    }

    return is_success;
}

/******************************************************************************
* Function : wifiMgmt_Initialize
*//** 
* @Description:
*
* This function handles the event from wifi module
* 
* @param    None
* @return   true: success
*           false: fail
*
*******************************************************************************/
static int WiFiMgmt_WifiEventHandle(uint32_t event_code,
                                    void * param1,
                                    void * param2)
{
    TRACE("WiFiMgmt_WifiEventHandle()");
    DEBUG("[event code = %d]", event_code);
    
    char * ssid = NULL;
    char * ip = NULL;
    switch((HAL_WIFI_EVENT)event_code)
    {
    case WIFI_STA_START:
        break;

    case WIFI_STA_STOP:
        break;

    case WIFI_CONNECTING:
        DEBUG("[event = WIFI_CONNECTING]");
        ssid = (char*)param1;
        ezmCli_Printf("Connecting to %s...\n", ssid);
        break;

    case WIFI_CONNECTED:
        DEBUG("[event = WIFI_CONNECTED]");
        ip = (char*)param1;
        ssid = (char*)param2;
        ezmCli_Printf("Got ip: %s\n", ip);
        ezmCli_Printf("Connected to %s\n", ssid);
        break;

    case WIFI_SCANNING:
        DEBUG("[event = WIFI_SCANNING]");
        break;

    case WIFI_SCAN_RESULT:
        DEBUG("[event = WIFI_SCAN_RESULT]");

        WifiAccesPointInfo * ap_info = (WifiAccesPointInfo *)param2;
        uint16_t ap_count = *(uint16_t*)param1;
        
        ezmCli_Printf("Num of avail AP %d\n", ap_count);
        ezmCli_Printf("*********************************************\n");
        for(uint16_t i = 0; i < ap_count; i++)
        {
            ezmCli_Printf("%s\n", ap_info->ssid);
            ap_info++;
        }
        ezmCli_Printf("*********************************************\n");
        break;

    case WIFI_DISCONNECTED:
        DEBUG("[event = WIFI_DISCONNECTED]");
        ssid = (char*)param1;
        ezmCli_Printf("Disconnected from %s\n", ssid);
        break;

    default:
        DEBUG("[event = unknown]");
        break;
    }
    
    return 0;
}

/******************************************************************************
* Function : WiFiMgmt_AddWifiCmd
*//** 
* @Description:
*
* This function regiters the wifi related commands to the command line interface
* module
* 
* @param    None
* @return   true: success
*           false: fail
*
*******************************************************************************/
static bool WiFiMgmt_AddWifiCmd(void)
{
    bool is_success = true;

    CommandHandle cmd_handle = CLI_HANDLE_INVALID;
    cmd_handle = ezmCli_RegisterCommand(conn_cmd, 
                                        conn_desc,
                                        WiFiMgmt_ConnCmdHandle);
    if(cmd_handle == CLI_HANDLE_INVALID)
    {
        WARNING("cannot add [cmd = %s]", conn_cmd);
        is_success = false;
    }
    else
    {
        if(ezmCli_AddArgument(cmd_handle, 
                                conn_long_ssid,
                                conn_short_ssid,
                                conn_ssid_desc) == false)
        {
            WARNING("cannot add [ard = %s]", conn_short_ssid);
            is_success = false;
        }

        if(ezmCli_AddArgument(cmd_handle, 
                                conn_long_pwd,
                                conn_short_pwd,
                                conn_pwd_desc) == false)
        {
            WARNING("cannot add [ard = %s]", conn_long_pwd);
            is_success = false;
        }
    }

    cmd_handle = ezmCli_RegisterCommand(conn_cmd_default, 
                                        conn_desc_default,
                                        WiFiMgmt_ConnDefaultCmdHandle);
    if(cmd_handle == CLI_HANDLE_INVALID)
    {
        WARNING("cannot add [cmd = %s]", conn_cmd_default);
        is_success = false;
    }

    cmd_handle = ezmCli_RegisterCommand(disconn_cmd, 
                                        disconn_cmd_desc,
                                        WiFiMgmt_DisconnCmdHandle);
    if(cmd_handle == CLI_HANDLE_INVALID)
    {
        WARNING("cannot add [cmd = %s]", disconn_cmd);
        is_success = false;
    }

    cmd_handle = ezmCli_RegisterCommand(scan_cmd, 
                                        scan_cmd_desc,
                                        WiFiMgmt_ScanCmdHandle);
    if(cmd_handle == CLI_HANDLE_INVALID)
    {
        WARNING("cannot add [cmd = %s]", disconn_cmd);
        is_success = false;
    }

    return is_success;
}

/******************************************************************************
* Function : WiFiMgmt_ConnCmdHandle
*//** 
* @Description:
*
* This function handles the connect command
* 
* @param    *pu8Command: (IN) pointer to the command
* @param    *pValueList: (IN) pointer to the list of value
* @return   CLI notification code
*
*******************************************************************************/
static CLI_NOTIFY_CODE WiFiMgmt_ConnCmdHandle(const char * pu8Command,
                                                void * pValueList)
{
    CLI_NOTIFY_CODE ret_code = CLI_NC_OK;
    bool is_success = true;
    uint32_t* pu32Params = NULL;
    char * ssid = NULL;
    char * pwd = NULL;

    TRACE("receive [command = %s]", pu8Command);

    if(strcmp(pu8Command, conn_cmd) != 0U)
    {
        is_success = false;
    }

    if(is_success)
    {
        pu32Params = (uint32_t*)pValueList;
        
        if(pu32Params)
        {
            ssid = (char*)(*pu32Params);
            TRACE("[ssid = %s]", ssid);

            pu32Params++;
            pwd = (char*)(*pu32Params);
            TRACE("[pwd = %s]", pwd);

            if(wifi_drv)
            {
                wifi_drv->WiFiCtrl_Connect(ssid, strlen(ssid), pwd, strlen(pwd));
            }
        }
    }

    return ret_code;
}

/******************************************************************************
* Function : WiFiMgmt_ConnDefaultCmdHandle
*//** 
* @Description:
*
* This function handles the connect to default AP command
* 
* @param    *pu8Command: (IN) pointer to the command
* @param    *pValueList: (IN) pointer to the list of value
* @return   CLI notification code
*
*******************************************************************************/
static CLI_NOTIFY_CODE WiFiMgmt_ConnDefaultCmdHandle(const char * pu8Command,
                                                        void * pValueList)
{
    CLI_NOTIFY_CODE ret_code = CLI_NC_OK;
    bool is_success = true;

    TRACE("receive [command = %s]", pu8Command);

    if(strcmp(pu8Command, conn_cmd_default) != 0U)
    {
        is_success = false;
    }

    if(is_success)
    {
        if(wifi_drv)
        {
            wifi_drv->WiFiCtrl_Connect( default_ssid, 
                                        strlen(default_ssid), 
                                        default_pwd,
                                        strlen(default_pwd));
        }
    }

    return ret_code;
}


/******************************************************************************
* Function : WiFiMgmt_DisconnCmdHandle
*//** 
* @Description:
*
* This function handles the disconnect command
* 
* @param    *pu8Command: (IN) pointer to the command
* @param    *pValueList: (IN) pointer to the list of value
* @return   CLI notification code
*
*******************************************************************************/
static CLI_NOTIFY_CODE WiFiMgmt_DisconnCmdHandle(const char * pu8Command,
                                                        void * pValueList)
{
    CLI_NOTIFY_CODE ret_code = CLI_NC_OK;
    bool is_success = true;

    TRACE("receive [command = %s]", pu8Command);

    if(strcmp(pu8Command, disconn_cmd) != 0U)
    {
        is_success = false;
    }

    if(is_success)
    {
        if(wifi_drv)
        {
            wifi_drv->WifiCtrl_Disconnect();
        }
    }

    return ret_code;
}


/******************************************************************************
* Function : WiFiMgmt_ScanCmdHandle
*//** 
* @Description:
*
* This function handles the scan command
* 
* @param    *pu8Command: (IN) pointer to the command
* @param    *pValueList: (IN) pointer to the list of value
* @return   CLI notification code
*
*******************************************************************************/
static CLI_NOTIFY_CODE WiFiMgmt_ScanCmdHandle(const char * pu8Command,
                                              void * pValueList)
{
    CLI_NOTIFY_CODE ret_code = CLI_NC_OK;
    bool is_success = true;

    TRACE("receive [command = %s]", pu8Command);

    if(strcmp(pu8Command, scan_cmd) != 0U)
    {
        is_success = false;
    }

    if(is_success)
    {
        if(wifi_drv)
        {
            wifi_drv->WifiCtrl_Scan();
        }
    }

    return ret_code;
}

#endif

/* End of file*/

