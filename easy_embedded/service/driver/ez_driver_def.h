/*****************************************************************************
* Filename:         ez_driver_def.h
* Author:           Hai Nguyen
* Original Date:    15.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_driver_def.h
 *  @author Hai Nguyen
 *  @date   15.03.2024
 *  @brief  Definition of the driver component
 *
 *  @details
 */

#ifndef _EZ_DRIVER_DEF_H
#define _EZ_DRIVER_DEF_H

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_DRIVER_ENABLE == 1)
#include "stdint.h"
#include "stdbool.h"
#include "ez_linked_list.h"

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
/* None*/


/*****************************************************************************
* Component Typedefs
*****************************************************************************/
typedef enum
{
    STATUS_OK,                  /**< OK, working as expected */
    STATUS_BUSY,                /**< Driver is occupied by other ezDrvInstance instantace*/
    STATUS_TIMEOUT,             /**< Operation timeout */
    STATUS_ERR_GENERIC,         /**< Generic error */
    STATUS_ERR_ARG,             /**< Invalid function argument */
    STATUS_ERR_DRV_NOT_FOUND,   /**< HW driver implmentation not found */
    STATUS_ERR_INF_NOT_EXIST,   /**< Implementation of an interface not found */
}EZ_DRV_STATUS;


typedef void (*ezDrvCallback)(uint8_t event_code, void *param1, void *param2);


struct ezDrvInstance
{
    ezDrvCallback   calback;   /**< Callback funtion to handle the event from the HW driver */
    void            *driver;    /**< Pointer to the HAL driver, depending on the implmentation */
};


typedef struct ezDrvInstance ezDrvInstance_t;


struct ezDriverCommon
{
    const char*     name;           /* Name of the driver instance */
    uint8_t         version[3];     /* Version number including major, minor, patch */
    ezDrvInstance_t *curr_inst;     /* Point to the current instance using the driver. NULL means the driver is available. */
};


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */


/*****************************************************************************
* Function Prototypes
*****************************************************************************/
static inline void *ezDriver_GetDriverFromInstance(ezDrvInstance_t *inst)
{
    void *drv = NULL;
    if(inst != NULL && inst->driver != NULL)
    {
        drv = inst->driver;
    }

    return drv;
}


static inline bool ezDriver_IsDriverAvailable(ezDrvInstance_t *inst, struct ezDriverCommon *drv_common)
{
    bool ret = false;
    if(inst != NULL && drv_common != NULL)
    {
        ret = ((drv_common->curr_inst == NULL) || (drv_common->curr_inst == inst));
    }
    return ret;
}


static inline void ezDriver_LockDriver(ezDrvInstance_t *inst, struct ezDriverCommon *drv_common)
{
    if(inst != NULL && drv_common != NULL)
    {
        drv_common->curr_inst = inst;
    }
}


static inline void ezDriver_UnlockDriver(struct ezDriverCommon *drv_common)
{
    if(drv_common != NULL)
    {
        drv_common->curr_inst = NULL;
    }
}


#endif /* EZ_DRIVER_ENABLE == 1 */
#endif /* _EZ_DRIVER_DEF_H */


/* End of file */
