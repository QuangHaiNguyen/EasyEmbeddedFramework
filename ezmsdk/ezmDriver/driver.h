/*******************************************************************************
* Filename:         driver.h
* Author:           Hai Nguyen
* Original Date:    29.05.2022
* Last Update:      29.05.2022
*
* -----------------------------------------------------------------------------
* Comany:           Easy Embedded
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Easy Embedded
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 29.05.2022
*
*******************************************************************************/

/** @file   driver.h
 *  @author Hai Nguyen
 *  @date   29.05.2022
 *  @brief  This is the header for driver module
 *
 *  @details Providing api and data structure to work with driver module
 *
 */

#ifndef _DRIVER_H
#define _DRIVER_H


/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (DRIVERINF == 1U)
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief function pointer of the driver initialize function
 *
 */
typedef bool    (*DriverInitFunction)   (void);

/** @brief enumerated list of available drivers
 *
 */
typedef enum
{
    DUMMY_DRIVER,   /**< for testing purpose only*/
#if(HAL_UART)
    UART0_DRIVER,   /**< uart driver, normally for cli*/
#endif
    NUM_OF_DRIVER   /**< number of available drivers*/
}DriverId;

/** @brief CLI notification code
 *
 */
typedef struct
{
    void                *driver_api;    /**< pointer to the driver api structure*/
    DriverInitFunction  init_function;  /**< pointer to the initialize  function*/
    bool                is_busy;        /**< busy flag */
}Driver;

/** @brief function pointer of the get driver function
 *
 */
typedef Driver* (*GetDriverFunction)    (void);

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
bool ezmDriver_Init                 (void);
void ezmDriver_GetDriverInstance    (DriverId id, void **driver_api);
bool ezmDriver_ReleaseDriverInstance(DriverId id);
bool ezmDriver_IsDriverBusy         (DriverId id);
/******************************************************************************
* Function Prototypes
*******************************************************************************/

#endif /* DRIVERINF */
#endif /* _DRIVER_H */

/* End of file*/