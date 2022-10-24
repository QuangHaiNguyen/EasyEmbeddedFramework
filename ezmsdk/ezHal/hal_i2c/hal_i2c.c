
/*******************************************************************************
* Filename:         hal_i2c.c
* Author:           Hai Nguyen
* Original Date:    22.10.2022
* Last Update:      22.10.2022
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
* Written by Hai Nguyen 22.10.2022
*
*******************************************************************************/

/** @file   hal_i2c.c
 *  @author Hai Nguyen
 *  @date   22.10.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "hal_i2c.h"

#if (CONFIG_HAL_I2C == 1U)

#define DEBUG_LVL   LVL_TRACE   /**< logging level */
#define MOD_NAME    "hal_i2c"       /**< module name */
#include "ezUtilities/logging/logging.h"
#include "ezApp/ezDriver/ezDriver.h"

/*the rest of include go here*/

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
static struct ezDriver i2c_drv = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/
void *ezI2c_LinkDriver(void)
{
    i2c_drv.is_busy = false;
    i2c_drv.config = NULL;
    i2c_drv.Initialize = NULL;
    i2c_drv.ll_interface = NULL;
    i2c_drv.std_interface = NULL;

    return (void *)&i2c_drv;
}

/******************************************************************************
* Internal functions
*******************************************************************************/

#endif /* CONFIG_HAL_I2C == 1U */
/* End of file*/

