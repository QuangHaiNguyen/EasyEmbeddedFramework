
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

#include "utilities/logging/logging.h"
#include "utilities/event_notifier/event_notifier.h"
#include "utilities/ezmAssert/ezmAssert.h"
#include "ezmDriver/driver.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
struct WiFiController
{
    evnt_pub publisher_handle;
    Driver  driver;
    uint32_t(*InterruptCalbback)(uint32_t event_code, void* param1, void * param2);
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct WiFiController controller_instance = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/
static evnt_sub WifiCtrl_ReceiveEventNotification(EVENT_CALLBACK callback);

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : WifiCtrl_GetWifiControllerDriver
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
        controller_instance.driver.WifiCtrl_ReceiveEventNotification = WifiCtrl_ReceiveEventNotification;
    }

    /* Binding to low level layer*/

    /* check return */
    if (is_success)
    {
        ret_driver = (void*)&(controller_instance.driver);
        TRACE("Init complete");
    }

    return ret_driver;
}

/******************************************************************************
* Internal functions
*******************************************************************************/
static evnt_sub WifiCtrl_ReceiveEventNotification(EVENT_CALLBACK callback)
{
    ASSERT_MSG(callback != NULL, "callback is Null");
    return evntNoti_SubscribeEvent(controller_instance.publisher_handle, callback);
}

/* End of file*/

