/*******************************************************************************
* Title                 :   module
* Filename              :   module.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  21.02.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   module.h
 *  @brief  Header template for a module
 */


#ifndef _DUMMY_DRIVER_H
#define _DUMMY_DRIVER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"


#if (CONFIG_DRIVERINF == 1U)

#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

typedef struct
{
    void (*dummy_open)(void);
    void (*dummy_close)(void);
    void (*dummy_write)(void);
    void (*dummy_read)(void);
}DummyApi;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void * DummyDriver_GetDriver(void);

#endif /* CONFIG_DRIVERINF */
#endif /* _DUMMY_DRIVER_H */

/* End of file*/
