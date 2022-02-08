/*******************************************************************************
* Title                 :   ezmDriver 
* Filename              :   ezmDriver.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   08.02.2022
* Version               :   1.0.0
*
*******************************************************************************/

/** @file  ezmDriver.c
 *  @brief This is the source code for the ezmDriver module
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ezmDriver.h"

#if (DRIVERINF == 1U)
#include "string.h"
#include "../helper/hexdump/hexdump.h"
#include "../helper/linked_list/linked_list.h"
#include "../ezmDebug/ezmDebug.h"

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


/******************************************************************************
* Module Typedefs
*******************************************************************************/


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static Driver   driver_list[NUM_OF_DRIVER] = { 0 };

/******************************************************************************
* Function Prototypes
*******************************************************************************/

bool ezmDriver_AddDriver(DriverId id, DriverInitFunction init_function, void* driver_api)
{
    bool is_success = true;
    Driver *driver;

    if (NULL == init_function || NULL == driver_api || id >= NUM_OF_DRIVER)
    {
        is_success = false;
    }

    if (is_success)
    {
        driver = &driver_list[id];

        driver->driver_api = driver_api;
        driver->init_function = init_function;
        driver->is_busy = false;

        DRIVERPRINT1("add driver [id = %x] successfully", id);

        if (false == driver->init_function())
        {
            is_success = false;
        }
        else
        {
            DRIVERPRINT("driver init successfully");
        }
    }

    return is_success;
}

bool ezmDriver_GetDriverInstance(DriverId id, void* driver_api)
{
    bool is_success = true;
    Driver* driver = NULL;

    if (id >= NUM_OF_DRIVER || driver_api == NULL)
    {
        is_success = false;
    }
    else
    {
        driver = &driver_list[id];
    }

    if (is_success && false == driver->is_busy)
    {
        driver_api = driver->driver_api;
    }
    else
    {
        is_success = false;
    }

    return is_success;
}

bool ezmDriver_ReleaseDriverInstance(DriverId id)
{
    bool is_success = false;

    if (id < NUM_OF_DRIVER)
    {
        driver_list[id].is_busy = false;
        is_success = true;
    }

    return is_success;
}

bool ezmDriver_IsDriverBusy(DriverId id)
{
    bool is_busy = false;

    if (id < NUM_OF_DRIVER)
    {
        is_busy = driver_list[id].is_busy;
    }

    return is_busy;
}

#endif /* DRIVERINF */

/* End of file*/