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
#include "hal/uart/uart.h"
#include "dummy_driver.h"
#include "string.h"
#include "helper/hexdump/hexdump.h"
#include "helper/linked_list/linked_list.h"
#include "ezmDebug/ezmDebug.h"

#define MOD_NAME        "DRIVER"


#if (MODULE_DEBUG == 1U) && (KERNEL_DEBUG == 1U)
#define DRIVERPRINT(a)                     PRINT_DEBUG(MOD_NAME,a)
#define DRIVERPRINT1(a,b)                  PRINT_DEBUG1(MOD_NAME,a,b)
#define DRIVERPRINT2(a,b,c)                PRINT_DEBUG2(MOD_NAME,a,b,c)
#define DRIVERPRINT3(a,b,c,d)              PRINT_DEBUG3(MOD_NAME,a,b,c,d)
#define DRIVERPRINT4(a,b,c,d,e)            PRINT_DEBUG4(MOD_NAME,a,b,c,d,e)
#else 
#define DRIVERPRINT(a)
#define DRIVERPRINT(a,b)
#define DRIVERPRINT2(a,b,c)
#define DRIVERPRINT3(a,b,c,d)
#define DRIVERPRINT4(a,b,c,d,e)
#endif

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
#if(HAL_UART)
    (GetDriverFunction)GetUart0Driver,
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

    for (uint16_t i = 0; i < NUM_OF_DRIVER; i++)
    {
        driver_list[i] = get_driver[i]();

        if (false == driver_list[i]->init_function())
        {
            is_success = false;
            DRIVERPRINT("driver init failed");
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

    if (id < NUM_OF_DRIVER && id >= 0)
    {
        if (!driver_list[(uint8_t)id]->is_busy)
        {
            *driver_api = driver_list[id]->driver_api;
        }
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

#endif /* DRIVERINF */

/* End of file*/
