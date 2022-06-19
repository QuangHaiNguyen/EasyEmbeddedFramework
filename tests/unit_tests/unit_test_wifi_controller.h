
/*******************************************************************************
* Filename:         unit_test_wifi_controller.h
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

#ifndef _UNIT_TEST_WIFI_CONTROLLER
#define _UNIT_TEST_WIFI_CONTROLLER

#include <limits.h>
#include "gtest/gtest.h"

extern "C" {
#include "ezmDriver/driver.h"
#include "hal/wifi_controller/wifi_controller.h"
#include "utilities/event_notifier/event_notifier.h"

#if (WIFI_CONTROLLER == 1U)
    Driver * driver = NULL;
    bool connecting_flag = false;
    bool connected_flag = false;
    bool scanning_flag = false;
    bool scan_result_flag = false;
    bool disconnected_flag = false;
    const char test_ssid[] = "test_ssid";
    const char test_pwd[] = "test_pwd";

    int EventCallback(uint32_t event_code, void* param1, void* param2)
    {
        switch ((HAL_WIFI_EVENT)event_code)
        {
        case WIFI_CONNECTING:
            connecting_flag = true;
            break;
        case WIFI_CONNECTED:
            connected_flag = true;
            break;
        case WIFI_SCANNING:
            scanning_flag = true;
            break;
        case WIFI_SCAN_RESULT:
            scan_result_flag = true;
            break;
        case WIFI_DISCONNECTED:
            disconnected_flag = true;
            break;
        }

        return 0;
    }
#endif
}

namespace 
{
#if (WIFI_CONTROLLER == 1U)
    TEST(Wifi_Controller, General_Test) 
    {
        bool is_success = true;
        evnt_sub sub_handle;
        WiFiCtrlDriverApi * api = NULL;
        HAL_WIFI_EVENT event;
        char * ssid = NULL;

        evntNoti_Initialize();

        driver = (Driver*)WifiCtrl_GetWifiControllerDriver();
        ASSERT_NE(driver, nullptr);
        ASSERT_NE(driver->driver_api, nullptr);

        is_success = driver->init_function();
        sub_handle = driver->SubscribeEventNotification(EventCallback);

        ASSERT_EQ(driver->is_busy, false);
        ASSERT_EQ(is_success, true);

        api = (WiFiCtrlDriverApi*)driver->driver_api;

        api->WiFiCtrl_Connect(test_ssid, sizeof(test_ssid),
                                test_pwd, sizeof(test_pwd));

        event = api->WifiCtrl_GetEvent();
        ASSERT_EQ(event, WIFI_CONNECTED);

        api->WiFiCtrl_GetStoredSsid(&ssid);
        ASSERT_EQ(strcmp(ssid, test_ssid), 0);

        api->WifiCtrl_Scan();
        api->WifiCtrl_Disconnect();

        event = api->WifiCtrl_GetEvent();
        ASSERT_EQ(event, WIFI_DISCONNECTED);

        ASSERT_EQ(connecting_flag, true);
        ASSERT_EQ(connected_flag, true);
        ASSERT_EQ(scanning_flag, true);
        ASSERT_EQ(scan_result_flag, true);
        ASSERT_EQ(disconnected_flag, true);

        is_success = driver->UnsubscribeEventNotification(sub_handle);
        ASSERT_EQ(is_success, true);
    }
#endif
}

#endif /* _UNIT_TEST_WIFI_CONTROLLER */
