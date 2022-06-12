
/*******************************************************************************
* Filename:         wifi_controller.c
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

/** @file   wifi_controller.c
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
#include "wifi_controller.h"

#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "WIFI_CTRL"     /**< module name */

#include <string.h>

#include "utilities/logging/logging.h"
#include "utilities/event_notifier/event_notifier.h"
#include "utilities/ezmAssert/ezmAssert.h"
#include "ezmDriver/driver.h"

#if(SUPPORTED_CHIP == WIN)
#include "platforms/simulator/wifi_controller/wifi_controller_sim.h"
#endif


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of api set for Wifi controller
 *
 */
struct WiFiController
{
    evnt_pub publisher_handle;  /**< Event publisher handle, publish wifi event to subscribers*/
    Driver  driver;             /**< Store wifi controller api */
    INTERRUPT_CALLBACK callback;/**< Handle interrupt from hardware component */
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct WiFiController controller_instance = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/
static evnt_sub WifiCtrl_ReceiveEventNotification(EVENT_CALLBACK callback);
static uint32_t WifiCtrl_InterruptCallback(uint32_t event_code,
                                            void* param1,
                                            void* param2);

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : WifiCtrl_GetWifiControllerDriver
*//** 
* @Description:
*
* Return the driver structure of the wifi controller. This function is called by
* the driver module to register the driver to the system
* 
* @param    None
* @return   pointer to the driver structure
*
*******************************************************************************/
void* WifiCtrl_GetWifiControllerDriver(void)
{
    bool is_success = true;
    void* ret_driver = NULL;

    TRACE("WifiCtrl_GetWifiControllerDriver()");

    controller_instance.publisher_handle = evntNoti_CreatePublisher(3);

    if (controller_instance.publisher_handle == UINT32_MAX)
    {
        is_success = false;
        ERROR("Cannot create event publisher");
    }

    if (is_success)
    {
        controller_instance.driver.is_busy = false;
        controller_instance.driver.ReceiveEventNotification = WifiCtrl_ReceiveEventNotification;

        /* Binding to low level layer*/
        controller_instance.driver.init_function = wifiSim_Initialization;
        is_success = wifiSim_BindingDriverApi(&controller_instance.driver.driver_api);
    }

    /* check return */
    if (is_success)
    {
        wifiSim_RegisterInterruptCallback(WifiCtrl_InterruptCallback);
        ret_driver = (void*)&(controller_instance.driver);
        TRACE("Init complete");
    }
    else
    {
        memset(&controller_instance, 0, sizeof(struct WiFiController));
        WARNING("Cannot init hardware, instance is reset");
    }

    return ret_driver;
}

/******************************************************************************
* Internal functions
*******************************************************************************/

/******************************************************************************
* Function : WifiCtrl_ReceiveEventNotification
*//**
* @Description:
*
* This function registers the subscriber which want to receive the wifi event
*
* @param    callback: (IN)callback function to thandle the event
* @return   event subscriber handle
*
*******************************************************************************/
static evnt_sub WifiCtrl_ReceiveEventNotification(EVENT_CALLBACK callback)
{
    ASSERT_MSG(callback != NULL, "callback is Null");
    return evntNoti_SubscribeEvent(controller_instance.publisher_handle, callback);
}


/******************************************************************************
* Function : WifiCtrl_InterruptCallback
*//**
* @Description:
*
* Handle the interrupt from hardware component and distribute the event to event
* subscribers
*
* @param    event_code: (IN)event code
* @param    param1: (IN)pointer to data 1
* @param    param2: (IN)pointer to data 2
* 
* @return   always 0
*
*******************************************************************************/
static uint32_t WifiCtrl_InterruptCallback(uint32_t event_code, void * param1, void * param2)
{

    DEBUG("receive [event = %d]", event_code);

    /* send event code and data to subscriber */
    evntNoti_NotifyEvent(controller_instance.publisher_handle, 
                            event_code,
                            param1,
                            param2);
    return 0;
}

/* End of file*/

