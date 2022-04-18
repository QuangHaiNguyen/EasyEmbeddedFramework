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

void ezmDriver_GetDriverInstance(DriverId id, void** driver_api)
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
