
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

#define DEBUG_LVL   LVL_CRITICAL        /**< logging level */
#define MOD_NAME    "ez_hal_echo"       /**< module name */
#include "utilities/logging/ez_logging.h"
#include "utilities/assert/ez_assert.h"

#if(WINDOWS_TARGET == 1 && EZ_TARGET_ECHO == 1)
#include "targets/windows/echo_driver/ez_windows_echo.h"
#endif

#include "service/driver/ez_hal_driver_def.h"
#include "ez_hal_echo_target_def.h"
#include "string.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define ECHO_INTERFACE_1        0   /**< index of the hardware 1st device */
#define ECHO_INTERFACE_2        1   /**< index of the hardware 1st device */
#define NUM_OF_ECHO_INTERFACE   2   /**< Number of the supported devices*/
#define GET_TARGET_DRIVER(x)    ((const struct ezHalEchoDriver*)x->ptr_target_driver)

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static struct ezDriver echo_drivers[NUM_OF_ECHO_INTERFACE] =
{
    /* first interface */
    {
        .name = ECHO_INTERFACE_1_NAME,
        .version = 10000,
        .ptr_configuration = NULL,
        .ptr_target_driver = NULL,
        .current_handle = NULL,
    },

    /* second interface */
    {
        .name = ECHO_INTERFACE_2_NAME,
        .version = 10000,
        .ptr_configuration = NULL,
        .ptr_target_driver = NULL,
        .current_handle = NULL,
    },
};


/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezHalEchoCallback1(uint8_t cb_code, void *param1, void *param2);
static void ezHalEchoCallback2(uint8_t cb_code, void *param1, void *param2);


/******************************************************************************
* External functions
*******************************************************************************/
ezDriverStatus_t ezHalEcho_Initialize(void)
{
    ezDriverStatus_t status = EZ_DRIVER_OK;
    const struct ezHalEchoDriver *target_driver = NULL;
 
    /* First driver **********************************************************/
    target_driver = ezTargetEcho_GetDriver(ECHO_INTERFACE_1);
    target_driver->api.Initialize(ECHO_INTERFACE_1);
    
    echo_drivers[ECHO_INTERFACE_1].ptr_target_driver = (ezTargetDriver_t)target_driver;
    echo_drivers[ECHO_INTERFACE_1].ptr_configuration = (ezDriverConfiguration_t)ezTargetEcho_GetConfiguration(ECHO_INTERFACE_1);
    
    ASSERT_MSG(echo_drivers[ECHO_INTERFACE_1].ptr_target_driver != NULL,
                "Target driver does not exist");
    ASSERT_MSG(echo_drivers[ECHO_INTERFACE_1].ptr_configuration != NULL,
                "Configuration does not exist");
    ezTargetEcho_SetCallback(ECHO_INTERFACE_1, ezHalEchoCallback1);
    ezmLL_InitNode(&echo_drivers[ECHO_INTERFACE_1].parent_list);

    /* Second driver *********************************************************/
    target_driver = ezTargetEcho_GetDriver(ECHO_INTERFACE_2);
    target_driver->api.Initialize(ECHO_INTERFACE_2);
    
    echo_drivers[ECHO_INTERFACE_2].ptr_target_driver = (ezTargetDriver_t)target_driver;
    echo_drivers[ECHO_INTERFACE_2].ptr_configuration = (ezDriverConfiguration_t)ezTargetEcho_GetConfiguration(ECHO_INTERFACE_2);
    
    ASSERT_MSG(echo_drivers[ECHO_INTERFACE_1].ptr_target_driver != NULL,
        "Target driver does not exist");
    ASSERT_MSG(echo_drivers[ECHO_INTERFACE_1].ptr_configuration != NULL,
        "Configuration does not exist");
    ezmLL_InitNode(&echo_drivers[ECHO_INTERFACE_2].parent_list);
    ezTargetEcho_SetCallback(ECHO_INTERFACE_2, ezHalEchoCallback2);

    return status;
}


ezDriverStatus_t ezHalEcho_GetDriver(char *driver_name,
                                     ezDriverHandle_t *handle,
                                     ezDriverCallback callback)
{
    EZDEBUG("ezHalEcho_GetDriver(name = %s)", driver_name);

    ezDriverStatus_t status = EZ_DRIVER_ERR_NO_DRIVER;

    if ((NULL != driver_name) && (NULL != handle) && (NULL != callback))
    {
        for (uint8_t i = 0; i < NUM_OF_ECHO_INTERFACE; i++)
        {
            if (strcmp(driver_name, echo_drivers[i].name) == 0)
            {
                EZMLL_ADD_TAIL(&echo_drivers[i].parent_list, &handle->node);
                handle->driver = &echo_drivers[i];
                handle->callback = callback;

                status = EZ_DRIVER_OK;

                EZDEBUG("  driver found");
                break;
            }
        }
    }

    return status;
};


ezDriverStatus_t ezHalEcho_ReleaseDriver(ezDriverHandle_t *handle)
{
    ezDriverStatus_t status = EZ_DRIVER_OK;

    EZDEBUG("ezHalEcho_ReleaseDriver(handle = %p)", (void*)handle);

    EZMLL_UNLINK_NODE(&handle->node);
    handle->callback = NULL;
    handle->driver = NULL;

    return status;

}


uint32_t ezHalEcho_Write(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size)
{
    uint32_t num_written_byte = 0;
    const struct ezHalEchoDriver *target_driver = NULL;

    if (NULL != buff
        && buff_size > 0
        && handle != NULL
        && handle->driver != NULL)
    {
        target_driver = GET_TARGET_DRIVER(handle->driver);

        /* No module uses the driver or the same module execute the function */
        if (handle->driver->current_handle == NULL 
            || handle->driver->current_handle == handle)
        {
            /* block it */
            handle->driver->current_handle = handle;
            num_written_byte = target_driver->api.Write(target_driver->index, buff, buff_size);

            /* Unblock when receive callback or timeout */
        }
        else
        {
            EZWARNING("Driver is occupied");
        }
    }

    return num_written_byte;
}


uint32_t ezHalEcho_Read(ezDriverHandle_t *handle, uint8_t *buff, uint32_t buff_size)
{

    uint32_t num_read_byte = 0;

    const struct ezHalEchoDriver *target_driver = NULL;

    if (NULL != buff
        && buff_size > 0
        && handle != NULL
        && handle->driver != NULL)
    {
        target_driver = GET_TARGET_DRIVER(handle->driver);

        /* No module uses the driver or the same module execute the function */
        if (handle->driver->current_handle == NULL
            || handle->driver->current_handle == handle)
        {
            /* block it */
            handle->driver->current_handle = handle;
            num_read_byte = target_driver->api.Read(target_driver->index, buff, buff_size);

            /* Unblock when receive callback or timeout */
        }
        else
        {
            EZWARNING("Driver is occupied");
        }
    }

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
    ezDriverCallback callback = NULL;

    if (echo_drivers[ECHO_INTERFACE_1].current_handle != NULL)
    {
        callback = echo_drivers[ECHO_INTERFACE_1].current_handle->callback;
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
    echo_drivers[ECHO_INTERFACE_1].current_handle = NULL;
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
    ezDriverCallback callback = NULL;

    if (echo_drivers[ECHO_INTERFACE_2].current_handle != NULL)
    {
        callback = echo_drivers[ECHO_INTERFACE_2].current_handle->callback;
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
    echo_drivers[ECHO_INTERFACE_1].current_handle = NULL;
}


/* End of file*/

