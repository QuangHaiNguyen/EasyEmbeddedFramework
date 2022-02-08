/*******************************************************************************
* Title                 :   ezmDriver
* Filename              :   ezmDriver.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   08.02.2022
* Version               :   1.0.0
*
*******************************************************************************/

/** @file   ezmDriver.h
 *  @brief  Header file of ezmDriver module
 */


#ifndef _EZM_DRIVER_H
#define _EZM_DRIVER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "../app/app_config.h"

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
typedef bool    (*DriverInitFunction)   (void);


typedef enum
{
    DUMMY_DRIVER,
    NUM_OF_DRIVER
}DriverId;

typedef struct
{
    bool                is_busy;
    void                *driver_api;
    DriverInitFunction  init_function;
}Driver;

typedef Driver* (*GetDriverFunction)    (void);

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
bool ezmDriver_Init                 (void);
bool ezmDriver_GetDriverInstance    (DriverId id, void * driver_api);
bool ezmDriver_ReleaseDriverInstance(DriverId id);
bool ezmDriver_IsDriverBusy         (DriverId id);
/******************************************************************************
* Function Prototypes
*******************************************************************************/

#endif /* DRIVERINF */
#endif /* _EZM_DRIVER_H */

/* End of file*/
