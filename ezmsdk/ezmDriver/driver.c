/*******************************************************************************
* Filename:         driver.c
* Author:           Hai Nguyen
* Original Date:    29.05.2022
* Last Update:      29.05.2022
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
* Written by Hai Nguyen 29.05.2022
*
*******************************************************************************/

/** @file   driver.c
 *  @author Hai Nguyen
 *  @date   29.05.2022
 *  @brief  This is the source for the driver module
 *
 *  @details This module provides and manages a list of available hardware
 *  drivers to interface with the peripherals or sensors. The drivers call the
 *  HAL api which abstract the hardware dependency part. When running on Windows
 *  or Linux environment, the drivers use the simulated api
 *
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "driver.h"

#if (DRIVERINF == 1U)

#define DEBUG_LVL   LVL_TRACE       /**< logging level */
#define MOD_NAME    "DRIVER"     /**< module name */

#include <string.h>
#include "utilities/logging/logging.h"
#include "utilities/linked_list/linked_list.h"

#include "hal/uart/uart.h"
#include "hal/wifi_controller/wifi_controller.h"
#include "hal/network/mqtt/mqtt.h"
#include "dummy_driver.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

GetDriverFunction get_driver[NUM_OF_DRIVER] =
{
    (GetDriverFunction)DummyDriver_GetDriver,
#if(HAL_UART == 1U)
    (GetDriverFunction)GetUart0Driver,
#endif
#if(WIFI_CONTROLLER == 1U)
    (GetDriverFunction)WifiCtrl_GetWifiControllerDriver,
#endif
#if(MQTT == 1U)
    (GetDriverFunction)Mqtt_GetDriver,
#endif
};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static Driver   *driver_list[NUM_OF_DRIVER] = { 0 };

/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : ezmDriver_Init
*//**
* @Description:
*
* This function initializes the driver module
*
* @param    None
* @return   None
*
* @Example Example:
* @code
* if(ezmDriver_Init() == false)
* {
*     printf("init failed");
* }
* @endcode
*
*******************************************************************************/
bool ezmDriver_Init(void)
{
    bool is_success = true;

    TRACE("ezmDriver_Init()");

    for (uint16_t i = 0; i < NUM_OF_DRIVER; i++)
    {
        INFO("Init [driver id = %d]", i);
        if (get_driver[i] != NULL)
        {
            driver_list[i] = get_driver[i]();

            if (driver_list[i] != NULL && driver_list[i]->init_function != NULL)
            {
                if (false == driver_list[i]->init_function())
                {
                    is_success = false;
                    ERROR("[driver id = %d] init failed", i);
                }
            }
            else
            {
                WARNING("Cannot get hw driver [index = %d]", i);
            }
        }
        else
        {
            WARNING("[driver index = %d] is empty, and will be skipped", i);
        }
    }

    return is_success;
}

/******************************************************************************
* Function : ezmDriver_GetDriverInstance
*//**
* @Description:
*
* This function returns the set of api for a specific driver (e.g uart)
*
* @param    **driver_api: (OUT)pointer to the structure of api
* @param    id : (IN)id of the api, it can be found in driver.h
* @return   None
*
* @Example Example:
* @code
* UartDrvApi* uart_driver;
* ezmDriver_GetDriverInstance(UART0_DRIVER, (void*)(&uart_driver));
* if (uart_driver == NULL)
* {
*     printf("ERROR");
* }
* @endcode
*
*******************************************************************************/
void ezmDriver_GetDriverInstance(DriverId id, void **driver_api)
{
    *driver_api = NULL;
    DEBUG("ezmDriver_GetDriverInstance()");
    if (id < NUM_OF_DRIVER && id >= 0)
    {
        if (!driver_list[(uint8_t)id]->is_busy)
        {
            *driver_api = driver_list[id]->driver_api;
        }
        else
        {
            WARNING("busy");
        }
    }
    else
    {
        ERROR("invalid index [index = %d]", id);
    }
}

/******************************************************************************
* Function : ezmDriver_ReleaseDriverInstance
*//**
* @Description:
*
* This function releases a specified driver, which allows other modules to use it
*
* @param    id : (IN)id of the api, it can be found in driver.h
* @return   True if success, False if fail
*
* @Example Example:
* @code
* if (ezmDriver_ReleaseDriverInstance(UART0_DRIVER) == false)
* {
*     printf("ERROR");
* }
* @endcode
*
*******************************************************************************/
bool ezmDriver_ReleaseDriverInstance(DriverId id)
{
    bool is_success = false;

    if (id < NUM_OF_DRIVER && id >= 0)
    {
        driver_list[id]->is_busy = false;
        is_success = true;
    }

    return is_success;
}

/******************************************************************************
* Function : ezmDriver_IsDriverBusy
*//**
* @Description:
*
* This function returns the busy status of a driver.
*
* @param    id : (IN)id of the api, it can be found in driver.h
* @return   True if busy, False if available
*
* @Example Example:
* @code
* if (ezmDriver_IsDriverBusy(UART0_DRIVER) == false)
* {
*     printf("driver is used by another module");
* }
* @endcode
*
*******************************************************************************/
bool ezmDriver_IsDriverBusy(DriverId id)
{
    bool is_busy = false;

    if (id < NUM_OF_DRIVER && id >= 0)
    {
        is_busy = driver_list[id]->is_busy;
    }

    return is_busy;
}

/******************************************************************************
* Function : ezmDriver_SubscribeDriverEvent
*//**
* @Description:
*
* This function subscribed the driver user to event notification system
*
* @param    id : (IN)id of the api, it can be found in driver.h
* @param    callback: (IN)callback to be registered to handle received event
* @return   true: success
*           false: fail
*
*******************************************************************************/
bool ezmDriver_SubscribeDriverEvent(DriverId id,
                                    event_observer * observer)
{
    bool is_success = false;
    if (id < NUM_OF_DRIVER && 
        id >= 0 && 
        driver_list[id]->SubscribeEventNotification)
    {
            driver_list[id]->SubscribeEventNotification(observer);
            is_success = true;
    }
    else
    {
        WARNING("cannot subscribe to event");
    }
    return is_success;
}

/******************************************************************************
* Function : ezmDriver_UnsubscribeDriverEvent
*//**
* @Description:
*
* This function unsubscribed the driver user to event notification system
*
* @param    id : (IN)id of the api, it can be found in driver.h
* @param    sub_handle: (IN)subscriber handle
* @return   true: success 
*           false: fail
*
*******************************************************************************/
bool ezmDriver_UnsubscribeDriverEvent(DriverId id, event_observer * observer)
{
    bool is_success = false;
    if (id < NUM_OF_DRIVER
        && id >= 0 &&
        driver_list[id]->UnsubscribeEventNotification)
    {
        is_success = driver_list[id]->UnsubscribeEventNotification(observer);
    }
    return is_success;
}
#endif /* DRIVERINF */

/* End of file*/
