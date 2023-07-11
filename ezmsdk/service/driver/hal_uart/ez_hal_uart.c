
/*******************************************************************************
* Filename:         ez_hal_uart.c
* Author:           Hai Nguyen
* Original Date:    25.06.2023
* Last Update:      07.03.2023
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
 *  @brief  Implementation of the UART HAL Driver
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

#include "service/driver/ez_driver_common.h"

#include "utilities/logging/ez_logging.h"
#include "utilities/assert/ez_assert.h"
#include "utilities/linked_list/ez_linked_list.h"

#if(WINDOWS_TARGET)
#include "targets/windows/uart_driver/ez_windows_uart.h"
#endif


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
/* None */


/******************************************************************************
* Function Definitions
*******************************************************************************/

#if (NUM_OF_UART_INTERFACE > 0)
static void ezHalUart_Callback1(uint8_t cb_code,
                                void *param1,
                                void *param2);
#endif /* (NUM_OF_UART_INTERFACE > 0) */

#if (NUM_OF_UART_INTERFACE > 1)
static void ezHalUart_Callback2(uint8_t cb_code,
                                void *param1,
                                void *param2);
#endif /* (NUM_OF_UART_INTERFACE > 1) */

static struct ezDriver uart_drivers[NUM_OF_UART_INTERFACE] =
{
#if (NUM_OF_UART_INTERFACE > 0)
    INIT_DRIVER_STRUCT(UART0_NAME, 10000, ezHalUart_Callback1),
#endif /* (NUM_OF_UART_INTERFACE > 0) */

#if (NUM_OF_UART_INTERFACE > 1)
    INIT_DRIVER_STRUCT(UART1_NAME, 10000, ezHalUart_Callback2),
#endif /* (NUM_OF_UART_INTERFACE > 1) */
};

/******************************************************************************
* External functions
*******************************************************************************/

ezDriverStatus_t ezHalUart_Initialize(void)
{
    EZDEBUG("ezHalUart_Initialize()");
    ezDriverStatus_t status = EZ_DRIVER_ERR_INIT;

    for (uint8_t i = 0; i < NUM_OF_UART_INTERFACE; i++)
    {
        EZDEBUG("  Init driver index = %d, name = %s", i, uart_drivers[i].name);
        status = EZ_DRIVER_ERR_INIT;

#if(WINDOWS_TARGET == 1)
        uart_drivers[i].target_driver = (ezTargetDriver_t)ezWinUart_GetDriver(i);
#endif /* WINDOWS_TARGET == 1 */

        status = ezDriverCommon_Initialized(&uart_drivers[i]);
        if (status == EZ_DRIVER_OK)
        {
            /* set flag to indicate initialized is completed */
            uart_drivers[i].initialized = true;
        }
        else
        {
            /* error, stop init proccess */
            EZERROR("  Initialization failed");
            break;
        }
    }

    return status;
}


ezDriverStatus_t ezHalUart_Deinitialize(void)
{
    EZDEBUG("ezHalUart_Deinitialize()");

    ezDriverStatus_t status = EZ_DRIVER_ERR_GENERIC;

    for (uint8_t i = 0; i < NUM_OF_UART_INTERFACE; i++)
    {
        status = ezDriverCommon_Deinitialized(&uart_drivers[i]);
        if (status == EZ_DRIVER_OK)
        {
            uart_drivers[i].initialized = false;
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
            if (!driver->initialized)
            {
                EZERROR("  Driver is not initialized");
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
    }/* else invalid argument */

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

    if (ezDriverCommon_IsHandleRegistered(handle)
        && ezDriverCommon_IsDriverAvail(handle)
        && buff
        && buff_size > 0)
    {
        driver = GET_TARGET_DRIVER(const struct ezTargetUartDriver*, handle->driver);
        ezDriverCommon_LockDriver(handle);

        if (driver->api.write_blocking)
        {
            num_written_bytes = driver->api.write_blocking(driver->common.index,
                                                          buff,
                                                          buff_size);
        }/* else error function pointer is null */

        ezDriverCommon_UnlockDriver(handle);
    }/* else error invalid arguments */

    return num_written_bytes;
}


uint32_t ezHalUart_ReadBlocking(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size)
{
    ASSERT(handle != NULL);
    ASSERT(buff != NULL);

    uint32_t num_read_bytes = 0;
    const struct ezTargetUartDriver *driver = NULL;

    if (ezDriverCommon_IsHandleRegistered(handle)
        && ezDriverCommon_IsDriverAvail(handle)
        && buff
        && buff_size > 0)
    {
        driver = GET_TARGET_DRIVER(const struct ezTargetUartDriver*, handle->driver);
        ezDriverCommon_LockDriver(handle);

        if (driver->api.read_blocking)
        {
            num_read_bytes = driver->api.read_blocking(driver->common.index,
                                                      buff,
                                                      buff_size);
        }/* else error function pointer is null */
        
        /* Blocking operation so unlock the driver */
        ezDriverCommon_UnlockDriver(handle);

    }/* else error invalid arguments */

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

    if (ezDriverCommon_IsHandleRegistered(handle)
        && ezDriverCommon_IsDriverAvail(handle)
        && buff
        && buff_size > 0)
    {
        driver = GET_TARGET_DRIVER(const struct ezTargetUartDriver*, handle->driver);

        if (driver->api.write)
        {
            ezDriverCommon_LockDriver(handle);
            num_written_bytes = driver->api.write(driver->common.index,
                                                  buff,
                                                  buff_size);
        }
        else
        {
            EZWARNING("  ezHalUart_Write() is not supported by target driver");
        }

    }/* else error invalid arguments */

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

    if (ezDriverCommon_IsHandleRegistered(handle)
        && ezDriverCommon_IsDriverAvail(handle)
        && buff
        && buff_size > 0)
    {
        driver = GET_TARGET_DRIVER(const struct ezTargetUartDriver*, handle->driver);

        if (driver->api.read)
        {
            ezDriverCommon_LockDriver(handle);
            num_read_bytes = driver->api.read(driver->common.index,
                                                buff,
                                                buff_size);
        }
        else
        {
            EZWARNING("  ezHalUart_Read() is not supported by target driver");
        }

    }/* else error invalid arguments */

    return num_read_bytes;
}


/******************************************************************************
* Internal functions
*******************************************************************************/

#if (NUM_OF_UART_INTERFACE > 0)
/******************************************************************************
* Function : ezHalUart_Callback1
*//**
* @Description:
*
* This function recieves the event from the target driver, checks which module is
* using the driver and forwards the event and data to that module. In case no
* module uses the driver (error), the event and its data will be discarded.
*
* @param    (IN)cb_code: event code, depend on the driver. See ezHalUartCallbackCode_t
* @param    (IN)param1: Point to the first parameter, can be NULL.
* @param    (IN)param2: Point to the second parameter, can be NULL.
*
* @return   None
*
*******************************************************************************/
static void ezHalUart_Callback1(uint8_t cb_code,
                                void *param1,
                                void *param2)
{
    if (uart_drivers[0].current_handle
        && uart_drivers[0].current_handle->callback)
    {
        EZDEBUG("Executing callback");
        uart_drivers[0].current_handle->callback(cb_code, param1, param2);
    }
    else
    {
        EZERROR("No callback found");
    }

    ezDriverCommon_UnlockDriver(uart_drivers[0].current_handle);
}
#endif /* (NUM_OF_UART_INTERFACE > 0) */

#if (NUM_OF_UART_INTERFACE > 1)
/******************************************************************************
* Function : ezHalUart_Callback1
*//**
* @Description:
*
* This function recieves the event from the target driver, checks which module is
* using the driver and forwards the event and data to that module. In case no
* module uses the driver (error), the event and its data will be discarded.
*
* @param    (IN)cb_code: event code, depend on the driver. See ezHalUartCallbackCode_t
* @param    (IN)param1: Point to the first parameter, can be NULL.
* @param    (IN)param2: Point to the second parameter, can be NULL.
*
* @return   None
*
*******************************************************************************/
static void ezHalUart_Callback2(uint8_t cb_code,
                                void *param1,
                                void *param2)
{
    if (uart_drivers[1].current_handle
        && uart_drivers[1].current_handle->callback)
    {
        EZDEBUG("Executing callback");
        uart_drivers[1].current_handle->callback(cb_code, param1, param2);
    }
    else
    {
        EZERROR("No callback found");
    }

    ezDriverCommon_UnlockDriver(uart_drivers[1].current_handle);
}
#endif /* (NUM_OF_UART_INTERFACE > 1) */

/* End of file*/
