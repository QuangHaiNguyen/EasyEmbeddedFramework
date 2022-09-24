
/*******************************************************************************
* Filename:         ezDriver.h
* Author:           Hai Nguyen
* Original Date:    20.09.2022
* Last Update:      20.09.2022
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
* Written by Hai Nguyen 20.09.2022
*
*******************************************************************************/

/** @file   ezDriver.h
 *  @author Hai Nguyen
 *  @date   20.09.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _EZDRIVER_H
#define _EZDRIVER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (CONFIG_DRIVERINF == 1U)

#include "stdint.h"
#include "stdbool.h"
#include "ezKernel/ezKernel.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief function pointer of the driver initialize function
 *
 */
typedef ezSTATUS (*ezDriverInitFunction)(void);


/** @brief -
 *
 */
typedef int (*StdInterfaceCallback)(uint8_t cb_code, void *param1, void *param2);


/** @brief -
 *
 */
typedef ezKernelTaskFunction StdInterfaceOpen;


/** @brief -
 *
 */
typedef ezKernelTaskFunction StdInterfaceClose;


/** @brief -
 *
 */
typedef ezKernelTaskFunction StdInterfaceWrite;


/** @brief -
 *
 */
typedef ezKernelTaskFunction StdInterfaceRead;


/** @brief -
 *
 */
struct ezStdInterface
{
    StdInterfaceOpen Open;
    StdInterfaceClose Close;
    StdInterfaceWrite Write;
    StdInterfaceRead Read;
};


struct ezStdInterfaceData
{
    uint8_t *data;
    uint32_t data_size;
    StdInterfaceCallback callback;
};


/** @brief -
 *
 */
struct ezDriver
{
    bool is_busy;       /**< */
    void *config;       /**< */
    ezDriverInitFunction Initialize;          /**< */
    void *ll_interface;                     /**< */
    struct ezStdInterface *std_interface;   /**< */
};

typedef struct ezDriver* ezDriveHandle;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */


/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function : ezDriver_Initialize
*//**
* @Description: -
*
* @param    -
* @return   -
*
*
*******************************************************************************/
void ezDriver_Initialize(void);


/******************************************************************************
* Function : ezDriver_GetDriver
*//**
* @Description: -
*
* @param    -
* @return   -
*
*
*******************************************************************************/
ezDriveHandle ezDriver_GetDriver(char *driver_name);


/******************************************************************************
* Function : ezDriver_Open
*//**
* @Description: -
*
* @param    -
* @return   -
*
*
*******************************************************************************/
ezSTATUS ezDriver_Open(ezDriveHandle handle, struct ezStdInterfaceData *data);


/******************************************************************************
* Function : ezDriver_Write
*//**
* @Description: -
*
* @param    -
* @return   -
*
*
*******************************************************************************/
ezSTATUS ezDriver_Write(ezDriveHandle handle, struct ezStdInterfaceData *data);


/******************************************************************************
* Function : ezDriver_Read
*//**
* @Description: -
*
* @param    -
* @return   -
*
*
*******************************************************************************/
ezSTATUS ezDriver_Read(ezDriveHandle handle, struct ezStdInterfaceData *data);


/******************************************************************************
* Function : ezDriver_Close
*//**
* @Description: -
*
* @param    -
* @return   -
*
*
*******************************************************************************/
ezSTATUS ezDriver_Close(ezDriveHandle handle, struct ezStdInterfaceData *data);


#endif /* CONFIG_DRIVERINF == 1U */
#endif /* _EZDRIVER_H */

/* End of file */

