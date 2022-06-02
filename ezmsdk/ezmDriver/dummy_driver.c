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

#if (DRIVERINF == 1U)
#include "driver.h"
#include "string.h"
#include "utilities/hexdump/hexdump.h"
#include "utilities/linked_list/linked_list.h"
#include "ezmDebug/ezmDebug.h"

#define MOD_NAME        "DUMMY_DRIVER"


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
static Driver dummy_driver = { 0 };
static DummyApi api = { 0 };
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

    return &dummy_driver;
}

static bool dummy_init(void)
{
    DRIVERPRINT("dummy driver is init");

    return true;
}
static void open(void)
{
    DRIVERPRINT("dummy driver is open");
}

static void close(void)
{
    DRIVERPRINT("dummy driver is close");
}

static void write(void)
{
    DRIVERPRINT("dummy driver writes");
}

static void read(void)
{
    DRIVERPRINT("dummy driver reads");
}

#endif /* DRIVERINF */

/* End of file*/