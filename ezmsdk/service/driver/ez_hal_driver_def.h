
/*******************************************************************************
* Filename:         ez_driver_def.h
* Author:           Hai Nguyen
* Original Date:    12.06.2023
* Last Update:      12.06.2023
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
* Written by Hai Nguyen 12.06.2023
*
*******************************************************************************/

/** @file   ez_driver_def.h
 *  @author Hai Nguyen
 *  @date   12.06.2023
 *  @brief  Boilerplate for the HAL driver module. All of the driver module must
 *          be implemented according to this template to ensure a unified and
 *          compatibility implementation of the HAL driver
 *  
 *  @details -
 * 
 */

#ifndef _EZ_HAL_DRIVER_DEF_H
#define _EZ_HAL_DRIVER_DEF_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdint.h"
#include "stdbool.h"
#include "utilities/linked_list/ez_linked_list.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Return status of the driver function
 */
typedef enum
{
    EZ_DRIVER_OK,           /**< OK, function as expected */
    EZ_DRIVER_BUSY,         /**< Driver is occupied by other module */
    EZ_DRIVER_ERR_NO_DRIVER,/**< Driver does not exist */
    EZ_DRIVER_ERR_INIT,     /**< Error occur during initialization */
    EZ_DRIVER_ERR_GENERIC,  /**< Generic, uncategorized error */
}ezDriverStatus_t;


/******************************************************************************
* Function : ezDriverCallback
*//**
* @Description: Callback to receive event from the target module
*
* @param    (IN)cb_code: event code
* @param    (IN)param1: first parameter, depending on cb_code
* @param    (IN)param1: second parameter, depending on cb_code
*
* @return   None
*
*
*******************************************************************************/
typedef void (*ezDriverCallback)(uint8_t cb_code,
                                    void *param1,
                                    void *param2);


/** @brief Define driver's configuration. The actual implementation depends on
 *         individual target
 */
typedef void* ezDriverConfiguration_t;


/** @brief Define target driver. The actual implementation depends on individual
 *         targer
 */
typedef void* ezTargetDriver_t;


/** @brief Define driver handle type
 */
typedef struct ezDriverHandle ezDriverHandle_t;


/** @brief Contain information, data, API of a driver
 */
struct ezDriver
{
    const char              *name;                  /**< Name of the driver */
    uint32_t                version;                /**< Version of the driver */
    ezDriverConfiguration_t ptr_configuration;      /**< Point to the configuration */
    ezTargetDriver_t        ptr_target_driver;      /**< Point to the hardware api */
    struct Node             parent_list;            /**< List of the module using the driver */
    ezDriverHandle_t  *current_handle;        /**< Handle currently "own" the driver */
};


/** @brief Hal driver handle for accessing the driver structure (see ezDriver)
 */
struct ezDriverHandle
{
    struct Node node;           /**< Node of linked list */
    ezDriverCallback callback;  /**< Callback function to receive event */
    struct ezDriver *driver;    /**< pointer to the driver struct */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */


/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* None */

#endif /* _EZ_HAL_DRIVER_DEF_H */

/* End of file */
