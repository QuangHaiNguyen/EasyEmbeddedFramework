/*******************************************************************************
* Title                 :   cli 
* Filename              :   cli.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.12.2021
* Version               :   1.0.0
*
*******************************************************************************/

/** @file  cli.c
 *  @brief This is the source code for a cli
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "dummy_driver.h"

#define DEBUG_LVL       LVL_INFO       /**< logging level */
#define MOD_NAME        "DUMMY_DRIVER"

#if (DRIVERINF == 1U)
#include "driver.h"
#include "string.h"
#include "utilities/hexdump/hexdump.h"
#include "utilities/logging/logging.h"
#include "utilities/linked_list/linked_list.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/


/******************************************************************************
* Module Typedefs
*******************************************************************************/
static Driver dummy_driver = { 0 };
static DummyApi api = { 0 };
static const char* driver_name = "dummy driver";

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
static bool dummy_init(void);
static void open(void);
static void close(void);
static void write(void);
static void read(void);

void* DummyDriver_GetDriver(void)
{
    api.dummy_open = open;
    api.dummy_close = close;
    api.dummy_read = read;
    api.dummy_write = write;

    dummy_driver.is_busy = false;
    dummy_driver.driver_api = (void*)&api;
    dummy_driver.init_function = dummy_init;
    dummy_driver.driver_name = driver_name;

    return &dummy_driver;
}

static bool dummy_init(void)
{
    INFO("dummy driver is init");

    return true;
}
static void open(void)
{
    INFO("dummy driver is open");
}

static void close(void)
{
    INFO("dummy driver is close");
}

static void write(void)
{
    INFO("dummy driver writes");
}

static void read(void)
{
    INFO("dummy driver reads");
}

#endif /* DRIVERINF */

/* End of file*/