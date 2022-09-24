/*******************************************************************************
* Title                 :   dummy_driver
* Filename              :   dummy_driver.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.12.2021
* Version               :   1.0.0
*
*******************************************************************************/

/** @file  dummy_driver.c
 *  @brief This is the source code for a dummy_driver
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "dummy_driver.h"

#define DEBUG_LVL       LVL_DEBUG       /**< logging level */
#define MOD_NAME        "DUMMY_DRIVER"

#if (CONFIG_DRIVERINF == 1U)
#include "ezDriver/ezDriver.h"
#include "string.h"
#include "utilities/hexdump/hexdump.h"
#include "utilities/logging/logging.h"
#include "utilities/linked_list/linked_list.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define ECHO_BUFF           512U


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct ezDriver dummy_drv = { 0 };
struct ezStdInterface dummy_std_inf = { 0 };
static uint8_t echo_buff[ECHO_BUFF];


/******************************************************************************
* Function Prototypes
*******************************************************************************/
ezSTATUS DummyDrv_Initialize(void);
static KERNEL_TASK_STATUS Open(void *task_data, uint32_t task_data_size);
static KERNEL_TASK_STATUS Close(void *task_data, uint32_t task_data_size);
static KERNEL_TASK_STATUS Write(void *task_data, uint32_t task_data_size);
static KERNEL_TASK_STATUS Read(void *task_data, uint32_t task_data_size);


void *LinkDummyDriver(void)
{
    dummy_drv.is_busy = false;
    dummy_drv.config = NULL;
    dummy_drv.Initialize = DummyDrv_Initialize;
    dummy_drv.ll_interface = NULL;
    dummy_drv.std_interface = NULL;

    return (void *)&dummy_drv;
}


ezSTATUS DummyDrv_Initialize(void)
{
    INFO("dummy driver is init");

    dummy_std_inf.Open = Open;
    dummy_std_inf.Close = Close;
    dummy_std_inf.Write = Write;
    dummy_std_inf.Read = Read;

    dummy_drv.std_interface = &dummy_std_inf;

    return ezSUCCESS;
}


static KERNEL_TASK_STATUS Open(void *task_data, uint32_t task_data_size)
{
    KERNEL_TASK_STATUS status = TASK_STATUS_OK;

    if (dummy_drv.is_busy)
    {
        status = TASK_STATUS_EXEC_AGAIN;
    }
    else
    {
        DEBUG("Dummy Open is called");
    }

    return status;
}


static KERNEL_TASK_STATUS Close(void *task_data, uint32_t task_data_size)
{
    KERNEL_TASK_STATUS status = TASK_STATUS_OK;

    if (dummy_drv.is_busy)
    {
        status = TASK_STATUS_EXEC_AGAIN;
    }
    else
    {
        DEBUG("Dummy Close is called");
    }

    return status;
}


static KERNEL_TASK_STATUS Write(void *task_data, uint32_t task_data_size)
{
    KERNEL_TASK_STATUS status = TASK_STATUS_ERROR;
    struct ezStdInterfaceData *inf_data = (struct ezStdInterfaceData *)task_data;

    if (dummy_drv.is_busy)
    {
        status = TASK_STATUS_EXEC_AGAIN;
    }
    else
    {
        DEBUG("Dummy Write is called");

        if (inf_data != NULL && task_data_size == sizeof(struct ezStdInterfaceData))
        {
            if (inf_data->data)
            {
                memcpy(echo_buff, inf_data->data, inf_data->data_size);
                TRACE("write [size = %d bytes] to echo buff:", inf_data->data_size);
                HEXDUMP(echo_buff, inf_data->data_size);
            }

            if (inf_data->callback)
            {
                TRACE("execute tx complete callback");
                inf_data->callback(CODE_TX_CMPLT, NULL, NULL);
            }

            status = TASK_STATUS_OK;
        }
    }

    return status;
}


static KERNEL_TASK_STATUS Read(void *task_data, uint32_t task_data_size)
{
    KERNEL_TASK_STATUS status = TASK_STATUS_ERROR;
    struct ezStdInterfaceData *inf_data = (struct ezStdInterfaceData *)task_data;

    if (dummy_drv.is_busy)
    {
        status = TASK_STATUS_EXEC_AGAIN;
    }
    else
    {
        DEBUG("Dummy Read is called");

        if (inf_data != NULL && task_data_size == sizeof(struct ezStdInterfaceData))
        {
            if (inf_data->data)
            {
                memcpy(inf_data->data, echo_buff, inf_data->data_size);
                TRACE("Read [size = %d bytes] from echo buff:", task_data_size);
                HEXDUMP(inf_data->data, inf_data->data_size);
            }

            if (inf_data->callback)
            {
                TRACE("execute rx complete callback");
                inf_data->callback(CODE_RX_CMPLT, &inf_data->data_size, NULL);
            }
        }
    }

    return status;
}


#endif /* CONFIG_DRIVERINF */

/* End of file*/