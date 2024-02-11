
/*******************************************************************************
* Filename:         ez_driver_common.c
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

/** @file   ez_driver_common.c
 *  @author Hai Nguyen
 *  @date   30.06.2023
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ez_driver_common.h"
#include "utilities/linked_list/ez_linked_list.h"
#include "utilities/assert/ez_assert.h"

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
/* None */

/******************************************************************************
* External functions
*******************************************************************************/
ezDriverStatus_t ezDriverCommon_Initialized(struct ezDriver *driver)
{
    ezDriverStatus_t status = EZ_DRIVER_ERR_INIT;
    struct ezDriverCommon *common = NULL;
 
    if (driver)
    {
        common = (struct ezDriverCommon *)driver->target_driver;

        ASSERT(common != NULL);
        ASSERT(common->initialize != NULL);
        ASSERT(common->get_configuration != NULL);
        ASSERT(common->set_callback != NULL);
        ASSERT(driver->callback != NULL);

        if (common
            && common->initialize
            && common->get_configuration
            && common->set_callback)
        {
            if (common->initialize(common->index) == EZ_DRIVER_OK)
            {
                driver->configuration = common->get_configuration(common->index);

                if (driver->configuration)
                {
                    ezmLL_InitNode(&driver->parent_list);
                    common->set_callback(common->index, driver->callback);
                    status = EZ_DRIVER_OK;
                }/* else error cannot get configuration */
            }/* else error cannot initialize low level driver*/
        }/* else error common api are not set */
    }/* else error invalid arguments */

    return status;
}


ezDriverStatus_t ezDriverCommon_Deinitialized(struct ezDriver *driver)
{
    ezDriverStatus_t status = EZ_DRIVER_ERR_GENERIC;
    struct ezDriverCommon *common = NULL;

    if (driver)
    {
        common = (struct ezDriverCommon *)driver->target_driver;

        ASSERT(common != NULL);
        ASSERT(common->deinitialize != NULL);

        if (common
            && common->deinitialize
            && common->deinitialize(common->index) == EZ_DRIVER_OK)
        {
            driver->target_driver = NULL;
            driver->configuration = NULL;
            driver->current_handle = NULL;

            status = EZ_DRIVER_OK;
        }/* else error function pointer is null*/
    }/* else error invalid argument*/

    return status;
}


/******************************************************************************
* Internal functions
*******************************************************************************/
/* None */

/* End of file*/

