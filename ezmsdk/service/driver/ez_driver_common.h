
/*******************************************************************************
* Filename:         ez_driver_common.h
* Author:           Hai Nguyen
* Original Date:    30.06.2023
* Last Update:      30.06.2023
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
* Written by Hai Nguyen 30.06.2023
*
*******************************************************************************/

/** @file   ez_driver_common.h
 *  @author Hai Nguyen
 *  @date   30.06.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _EZ_DRIVER_COMMON_H
#define _EZ_DRIVER_COMMON_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdint.h"
#include "stdbool.h"
#include "ez_hal_driver_def.h"
#include "utilities/linked_list/ez_linked_list.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/**< Get the target driver structure */
#define GET_TARGET_DRIVER(target_driver_type, driver_handle)    \
        ((target_driver_type)driver_handle->target_driver)  \


/**< Initialize a driver structure */
#define INIT_DRIVER_STRUCT(driver_name, version_number, callback_func)  \
        {                                                               \
            .initialized = false,                                       \
            .name = driver_name,                                        \
            .version = version_number,                                  \
            .configuration = NULL,                                      \
            .target_driver = NULL,                                      \
            .current_handle = NULL,                                     \
            .callback = callback_func,                                  \
        }                                                               \


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */


/******************************************************************************
* Inline functions
*******************************************************************************/

/******************************************************************************
* Function : ezDriverCommon_LockDriver
*//**
* @Description: Lock the driver by setting the handle as the current handle.
*               Users must check if the driver is free be for lock it by calling
*               ezDriverCommon_IsDriverAvail()
*
* @param    (IN)handle: pointer to the handle
* @return   None
*
*******************************************************************************/
static inline void ezDriverCommon_LockDriver(ezDriverHandle_t *handle)
{
    handle->driver->current_handle = handle;
}


/******************************************************************************
* Function : ezDriverCommon_UnlockDriver
*//**
* @Description: This function unlocks the driver by setting the current driver
*               handle to NULL
*
* @param    (IN)handle: pointer to the handle
* @return   None
*
*******************************************************************************/
static inline void ezDriverCommon_UnlockDriver(ezDriverHandle_t *handle)
{
    handle->driver->current_handle = NULL;
}


/******************************************************************************
* Function : ezDriverCommon_IsDriverAvail
*//**
* @Description: This function checks if the driver is available. The function
*               must be called prior to the lock function.
*
* @param    (IN)handle: pointer to the handle
* @return   true if the driver is available, else false
*
*******************************************************************************/
static inline bool ezDriverCommon_IsDriverAvail(ezDriverHandle_t *handle)
{
    return ((handle->driver->current_handle == NULL)
             || (handle->driver->current_handle == handle));
}


/******************************************************************************
* Function : ezDriverCommon_IsHandleRegistered
*//**
* @Description: This function checks if the handle is registered to the driver
*               structure. The register function is normall the XXX_GetDriver()
*               function.
*
* @param    (IN)handle: pointer to the handle
* @return   true is handle is registered, else false
*
*******************************************************************************/
static inline bool ezDriverCommon_IsHandleRegistered(ezDriverHandle_t *handle)
{
    return ((handle != NULL)
             && (handle->driver != NULL)
             && (ezmLL_IsNodeInList(&handle->driver->parent_list, &handle->node)));
}


/******************************************************************************
* Function : ezDriverCommon_Initialized
*//**
* @Description: This function initializes the driver structure. It must be
*               called by every HAL driver to ensure coherence operation.
*
* Internally, this function link the target driver to the HAL driver and
* initializes the target driver.
*
* @param    (IN)handle: pointer to the handle
* @return   EZ_DRIVER_OK if success, else EZ_DRIVER_ERR_INIT
*
*******************************************************************************/
ezDriverStatus_t ezDriverCommon_Initialized(struct ezDriver *driver);


/******************************************************************************
* Function : ezDriverCommon_Deinitialized
*//**
* @Description: This function deinitializes the driver structure. It must be
*               called by every HAL driver to ensure coherence operation.
*
* Internally, this function deinitializes the target driver and free the
* resources. The function can be used as the reset function to set the driver
* to known state.
*
* @param    (IN)handle: pointer to the handle
* @return   EZ_DRIVER_OK if success, else EZ_DRIVER_ERR_GENERIC
*
*******************************************************************************/
ezDriverStatus_t ezDriverCommon_Deinitialized(struct ezDriver *driver);


/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* None */

#endif /* _EZ_DRIVER_COMMON_H */
/* End of file */

