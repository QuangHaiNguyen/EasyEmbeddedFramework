
/*******************************************************************************
* Filename:         ez_hal_uart.c
* Author:           Hai Nguyen
* Original Date:    25.06.2023
* Last Update:      25.06.2023
*
* -----------------------------------------------------------------------------
* Company:          Embedded Easy
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Embedded Easy
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 25.06.2023
*
*******************************************************************************/

/** @file   ez_hal_uart.c
 *  @author Hai Nguyen
 *  @date   25.06.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ez_hal_uart.h"

#define DEBUG_LVL   LVL_TRACE           /**< logging level */
#define MOD_NAME    "ez_hal_uart"       /**< module name */

#include "utilities/logging/ez_logging.h"
#include "utilities/assert/ez_assert.h"
#include "utilities/linked_list/ez_linked_list.h"

#if(WINDOWS_TARGET)
#include "targets/windows/uart_driver/ez_windows_uart.h"
#endif

#define IS_DRIVER_INIT(driver)  (((struct ezTargetUartDriver *)(driver))->initialized)
#define GET_TARGET_DRIVER(x)    ((const struct ezTargetUartDriver*)x->ptr_target_driver)
/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct ezDriver uart_drivers[NUM_OF_UART_INTERFACE] =
{
    /* first interface */
    {
        .name = UART0_NAME,
        .version = 10000,
        .ptr_configuration = NULL,
        .ptr_target_driver = NULL,
        .current_handle = NULL,
        /*.parent_list is handled in the ezHalUart_Initialize */
    },
    /* second interface */
    {
        .name = UART1_NAME,
        .version = 10000,
        .ptr_configuration = NULL,
        .ptr_target_driver = NULL,
        .current_handle = NULL,
        /*.parent_list is handled in the ezHalUart_Initialize */
    },
};

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezHalUart_Callback1(uint8_t cb_code,
                                void *param1,
                                void *param2);

static void ezHalUart_Callback2(uint8_t cb_code,
                                void *param1,
                                void *param2);

/******************************************************************************
* External functions
*******************************************************************************/

ezDriverStatus_t ezHalUart_Initialize(void)
{
    EZDEBUG("ezHalUart_Initialize()");
    ezDriverStatus_t status = EZ_DRIVER_ERR_INIT;
    const struct ezTargetUartDriver *target_driver = NULL;

    for (uint8_t i = 0; i < NUM_OF_UART_INTERFACE; i++)
    {
        EZDEBUG("  Init driver %d", i);
        status = EZ_DRIVER_ERR_INIT;
        target_driver = ezWinUart_GetDriver(i);

        if (target_driver
            && target_driver->api.Initialize
            && target_driver->api.Initialize(i) == true)
        {
            /* Link target driver to the HAL driver */
            uart_drivers[i].ptr_target_driver = (ezTargetDriver_t)target_driver;

            /* Get the configuration of the target */
            uart_drivers[i].ptr_configuration = (ezDriverConfiguration_t)ezWinUart_GetConfiguration(i);

            if (uart_drivers[i].ptr_target_driver && uart_drivers[i].ptr_configuration)
            {
                /* Driver and configuration exist */
                ezmLL_InitNode(&uart_drivers[i].parent_list);
                ezWinUart_SetCallback(i, ezHalUart_Callback1);
                status = EZ_DRIVER_OK;
            }
            else
            {
                /* error, stop init proccess */
                break;
            }
        }
        else
        {
            /* error, stop init proccess */
            break;
        }
    }

    return status;
}


ezDriverStatus_t ezHalUart_Deinitialize(void)
{
    EZDEBUG("ezHalUart_Deinitialize()");

    ezDriverStatus_t status = EZ_DRIVER_ERR_GENERIC;
    const struct ezTargetUartDriver *target_driver = NULL;

    for (uint8_t i = 0; i < NUM_OF_UART_INTERFACE; i++)
    {
        target_driver = ezWinUart_GetDriver(i);
        status = EZ_DRIVER_ERR_GENERIC;

        if (target_driver
            && target_driver->api.Deinitialize
            && target_driver->api.Deinitialize(i))
        {
            uart_drivers[i].ptr_target_driver = NULL;
            uart_drivers[i].ptr_configuration = NULL;
            uart_drivers[i].current_handle = NULL;

            status = EZ_DRIVER_OK;
        }
        else
        {
            /* error, stop deinit proccess */
            break;
        }
    }

    return status;
}


ezDriverStatus_t ezHalUart_GetDriver(char *driver_name,
                                     ezDriverHandle_t *handle,
                                     ezDriverCallback callback)
{
    ASSERT(driver_name != NULL);
    ASSERT(handle != NULL);
    ASSERT(callback != NULL);

    ezDriverStatus_t status = EZ_DRIVER_ERR_NO_DRIVER;
    struct ezDriver *driver = NULL;

    EZDEBUG("ezHalUart_GetDriver(name = %s)", driver_name);

    if (driver_name && handle && callback)
    {
        for (uint8_t i = 0; i < NUM_OF_UART_INTERFACE; i++)
        {
            if (strcmp(driver_name, uart_drivers[i].name) == 0)
            {
                driver = &uart_drivers[i];
                EZDEBUG("  driver found");
                break;
            } /* else string does not match */
        }

        if (driver)
        {
            if ((!driver->ptr_configuration) || (!driver->ptr_target_driver))
            {
                EZERROR("Driver is not linked properly");
            }
            else if (!IS_DRIVER_INIT(driver->ptr_target_driver))
            {
                EZERROR("Driver is not initialized");
            }
            else
            {
                EZMLL_ADD_TAIL(&driver->parent_list, &handle->node);
                handle->driver = driver;
                handle->callback = callback;

                status = EZ_DRIVER_OK;
            }
        }/* else no drier found */
    }

    return status;
}


ezDriverStatus_t ezHalUart_ReleaseDriver(ezDriverHandle_t *handle)
{
    ezDriverStatus_t status = EZ_DRIVER_OK;
    ASSERT(handle != NULL);

    if (handle)
    {
        EZMLL_UNLINK_NODE(&handle->node);
    }

    return status;
}


uint32_t ezHalUart_WriteBlocking(ezDriverHandle_t *handle,
                                 uint8_t *buff,
                                 uint32_t buff_size)
{
    ASSERT(handle != NULL);
    ASSERT(buff != NULL);

    uint32_t num_written_bytes = 0;
    const struct ezTargetUartDriver *driver = NULL;

    if (handle
        && handle->driver
        && buff
        && buff_size > 0
        && ezmLL_IsNodeInList(&handle->driver->parent_list, &handle->node))
    {
        driver = GET_TARGET_DRIVER(handle->driver);

        if (handle->driver->current_handle == NULL ||
            handle->driver->current_handle == handle)
        {
            handle->driver->current_handle = handle;

            if (driver->api.WriteBlocking)
            {
                num_written_bytes = driver->api.WriteBlocking(driver->index,
                                                              buff,
                                                              buff_size);
            }

            /* Non blocking operation so current driver is unblock in the callback */
            handle->driver->current_handle = NULL;
        }

    }

    return num_written_bytes;

}


uint32_t ezHalUart_ReadBlocking(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size)
{
    ASSERT(handle != NULL);
    ASSERT(buff != NULL);

    uint32_t num_read_bytes = 0;
    const struct ezTargetUartDriver *driver = NULL;

    if (handle
        && handle->driver
        && buff
        && buff_size > 0
        && ezmLL_IsNodeInList(&handle->driver->parent_list, &handle->node))
    {
        driver = GET_TARGET_DRIVER(handle->driver);

        if (handle->driver->current_handle == NULL ||
            handle->driver->current_handle == handle)
        {
            handle->driver->current_handle = handle;

            if (driver->api.ReadBlocking)
            {
                num_read_bytes = driver->api.ReadBlocking(driver->index,
                                                          buff,
                                                          buff_size);
            }

            /* Blocking operation so unblock the driver */
            handle->driver->current_handle = NULL;
        }

    }

    return num_read_bytes;
}


uint32_t ezHalUart_Write(ezDriverHandle_t *handle,
                         uint8_t *buff,
                         uint32_t buff_size)
{
    ASSERT(handle != NULL);
    ASSERT(buff != NULL);

    uint32_t num_written_bytes = 0;
    const struct ezTargetUartDriver *driver = NULL;

    if (handle
        && handle->driver
        && buff
        && buff_size > 0
        && ezmLL_IsNodeInList(&handle->driver->parent_list, &handle->node))
    {
        driver = GET_TARGET_DRIVER(handle->driver);

        if (handle->driver->current_handle == NULL ||
            handle->driver->current_handle == handle)
        {
            handle->driver->current_handle = handle;
            if (driver->api.Write)
            {
                num_written_bytes = driver->api.Write(driver->index,
                                                      buff,
                                                      buff_size);
            }
            else
            {
                EZWARNING("  ezHalUart_Write() is not supported by target driver");
            }
        }
    }

    return num_written_bytes;
}


uint32_t ezHalUart_Read(ezDriverHandle_t *handle,
                        uint8_t *buff,
                        uint32_t buff_size)
{
    ASSERT(handle != NULL);
    ASSERT(buff != NULL);

    uint32_t num_read_bytes = 0;
    const struct ezTargetUartDriver *driver = NULL;

    if (handle
        && handle->driver
        && buff
        && buff_size > 0
        && ezmLL_IsNodeInList(&handle->driver->parent_list, &handle->node))
    {
        driver = GET_TARGET_DRIVER(handle->driver);

        if (handle->driver->current_handle == NULL ||
            handle->driver->current_handle == handle)
        {
            handle->driver->current_handle = handle;

            if (driver->api.Read)
            {
                num_read_bytes = driver->api.Read(driver->index,
                                                  buff,
                                                  buff_size);
            }
            else
            {
                EZWARNING("  ezHalUart_Read() is not supported by target driver");
            }
        }
    }

    return num_read_bytes;
}


/******************************************************************************
* Internal functions
*******************************************************************************/
static void ezHalUart_Callback1(uint8_t cb_code,
                                void *param1,
                                void *param2)
{
    ezDriverCallback callback = NULL;

    if (uart_drivers[0].current_handle != NULL)
    {
        callback = uart_drivers[0].current_handle->callback;
        EZDEBUG("Handle Exists");
    }

    if (callback != NULL)
    {
        EZDEBUG("Executing callback");
        callback(cb_code, param1, param2);
    }
    else
    {
        EZERROR("No callback found");
    }

    /* Success or not. We set the current handle to NUL
     * to indicate the driver is available
     */
    uart_drivers[0].current_handle = NULL;
}


static void ezHalUart_Callback2(uint8_t cb_code,
                                void *param1,
                                void *param2)
{
    ezDriverCallback callback = NULL;

    if (uart_drivers[1].current_handle != NULL)
    {
        callback = uart_drivers[1].current_handle->callback;
        EZDEBUG("Handle Exists");
    }

    if (callback != NULL)
    {
        EZDEBUG("Executing callback");
        callback(cb_code, param1, param2);
    }
    else
    {
        EZERROR("No callback found");
    }

    /* Success or not. We set the current handle to NUL
     * to indicate the driver is available
     */
    uart_drivers[1].current_handle = NULL;
}

/* End of file*/
