
/*******************************************************************************
* Filename:         ez_hal_echo.c
* Author:           Hai Nguyen
* Original Date:    13.06.2023
* Last Update:      13.06.2023
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
* Written by Hai Nguyen 13.06.2023
*
*******************************************************************************/

/** @file   ez_hal_echo.c
 *  @author Hai Nguyen
 *  @date   13.06.2023
 *  @brief  Implementation of the HAL echo driver
 *
 *  @details -
 *
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ez_hal_echo.h"

#define DEBUG_LVL   LVL_DEBUG           /**< logging level */
#define MOD_NAME    "ez_hal_echo"       /**< module name */

#include "utilities/logging/ez_logging.h"
#include "utilities/assert/ez_assert.h"

#if(WINDOWS_TARGET == 1)
#include "targets/windows/echo_driver/ez_windows_echo.h"
#endif

#include "service/driver/ez_driver_common.h"
#include "service/driver/ez_hal_driver_def.h"
#include "ez_hal_echo_target_def.h"
#include "string.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define ECHO_INTERFACE_1        0   /**< index of the hardware 1st device */
#define ECHO_INTERFACE_2        1   /**< index of the hardware 1st device */
#define NUM_OF_ECHO_INTERFACE   2   /**< Number of the supported devices*/


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
static void ezHalEchoCallback1(uint8_t cb_code, void *param1, void *param2);
static void ezHalEchoCallback2(uint8_t cb_code, void *param1, void *param2);


static struct ezDriver echo_drivers[NUM_OF_ECHO_INTERFACE] =
{
    INIT_DRIVER_STRUCT(ECHO_INTERFACE_1_NAME, 10000, ezHalEchoCallback1),
    INIT_DRIVER_STRUCT(ECHO_INTERFACE_2_NAME, 10000, ezHalEchoCallback2),
};


/******************************************************************************
* External functions
*******************************************************************************/
ezDriverStatus_t ezHalEcho_Initialize(void)
{
    EZDEBUG("ezHalEcho_Initialize()");
    ezDriverStatus_t status = EZ_DRIVER_ERR_INIT;;
 

    for (uint8_t i = 0; i < NUM_OF_ECHO_INTERFACE; i++)
    {
        EZDEBUG("  Init driver index = %d, name = %s", i, echo_drivers[i].name);
        status = EZ_DRIVER_ERR_INIT;

#if(WINDOWS_TARGET == 1)
        echo_drivers[i].target_driver = (ezTargetDriver_t)ezTargetEcho_GetDriver(i);
#endif /* WINDOWS_TARGET == 1 */

        status = ezDriverCommon_Initialized(&echo_drivers[i]);
        if (status == EZ_DRIVER_OK)
        {
            /* set flag to indicate initialized is completed */
            echo_drivers[i].initialized = true;
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


ezDriverStatus_t ezHalEcho_GetDriver(char *driver_name,
                                     ezDriverHandle_t *handle,
                                     ezDriverCallback callback)
{
    EZDEBUG("ezHalEcho_GetDriver(name = %s)", driver_name);

    ASSERT(driver_name != NULL);
    ASSERT(handle != NULL);
    ASSERT(callback != NULL);

    ezDriverStatus_t status = EZ_DRIVER_ERR_NO_DRIVER;
    struct ezDriver *driver = NULL;

    if (driver_name && handle && callback)
    {
        for (uint8_t i = 0; i < NUM_OF_ECHO_INTERFACE; i++)
        {
            if (strcmp(driver_name, echo_drivers[i].name) == 0)
            {
                driver = &echo_drivers[i];
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
};


ezDriverStatus_t ezHalEcho_ReleaseDriver(ezDriverHandle_t *handle)
{
    EZDEBUG("ezHalEcho_ReleaseDriver()");
    ezDriverStatus_t status = EZ_DRIVER_OK;

    ASSERT(handle != NULL);

    if (handle)
    {
        EZMLL_UNLINK_NODE(&handle->node);
        handle->driver = NULL;
        handle->callback = NULL;
    }/* else invalid argument */

    return status;
}


uint32_t ezHalEcho_Write(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size)
{
    uint32_t num_written_byte = 0;
    const struct ezHalEchoDriver *target_driver = NULL;

    if (ezDriverCommon_IsHandleRegistered(handle)
        && ezDriverCommon_IsDriverAvail(handle)
        && buff
        && buff_size > 0)
    {
        target_driver = GET_TARGET_DRIVER(const struct ezHalEchoDriver*, handle->driver);

        if (target_driver->api.Write)
        {
            ezDriverCommon_LockDriver(handle);
            num_written_byte = target_driver->api.Write(target_driver->common.index, buff, buff_size);

            /* Unlock when receive callback or timeout */
        }/* else error function pointer is null */
    }

    return num_written_byte;
}


uint32_t ezHalEcho_Read(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size)
{
    uint32_t num_read_byte = 0;
    const struct ezHalEchoDriver *target_driver = NULL;

    if (ezDriverCommon_IsHandleRegistered(handle)
        && ezDriverCommon_IsDriverAvail(handle)
        && buff
        && buff_size > 0)
    {
        target_driver = GET_TARGET_DRIVER(const struct ezHalEchoDriver*, handle->driver);

        if (target_driver->api.Read)
        {
            ezDriverCommon_LockDriver(handle);
            num_read_byte = target_driver->api.Read(target_driver->common.index, buff, buff_size);

            /* Unblock when receive callback or timeout */
        }/* else error function pointer is null */
    }/* else error invalid arguments */

    return num_read_byte;
}


/******************************************************************************
* Internal functions
*******************************************************************************/

/******************************************************************************
* Function : ezHalEchoCallback1
*//**
* @Description: Callback function to receive and handle the event from the
*               target layer
*
* @param (IN)cb_code: event code from the tartget layer
* @param (IN)param1: pointer to the first parameter. Depend on cb_code
* @param (IN)param2: pointer to the second parameter. Depend on cb_code
*
* @return -
*
*******************************************************************************/
static void ezHalEchoCallback1(uint8_t cb_code, void *param1, void *param2)
{
    if (echo_drivers[0].current_handle
        && echo_drivers[0].current_handle->callback)
    {
        EZDEBUG("Executing callback");
        echo_drivers[0].current_handle->callback(cb_code, param1, param2);
    }
    else
    {
        EZERROR("No callback found");
    }

    ezDriverCommon_UnlockDriver(echo_drivers[0].current_handle);
}


/******************************************************************************
* Function : ezHalEchoCallback2
*//**
* @Description: Callback function to receive and handle the event from the
*               target layer
*
* @param (IN)cb_code: event code from the tartget layer
* @param (IN)param1: pointer to the first parameter. Depend on cb_code
* @param (IN)param2: pointer to the second parameter. Depend on cb_code
*
* @return -
*
*******************************************************************************/
static void ezHalEchoCallback2(uint8_t cb_code, void *param1, void *param2)
{
    if (echo_drivers[1].current_handle
        && echo_drivers[1].current_handle->callback)
    {
        EZDEBUG("Executing callback");
        echo_drivers[1].current_handle->callback(cb_code, param1, param2);
    }
    else
    {
        EZERROR("No callback found");
    }

    ezDriverCommon_UnlockDriver(echo_drivers[1].current_handle);
}


/* End of file*/

