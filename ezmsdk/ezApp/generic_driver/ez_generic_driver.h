
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
#include "ezApp/ezSdk_config.h"

#if (EZ_GENERIC_DRIVER == 1U)
#include "service/kernel/ez_kernel.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Initialize driver function pointer declaration
 *
 */
typedef ezSTATUS (*ezDriverInitFunction)(void);


/** @brief Initialize driver function pointer declaration
 *
 */
typedef int (*StdInterfaceCallback)(uint8_t cb_code, void *param1, void *param2);


/** @brief Define kernel task as standard Open function
 *
 */
typedef ezKernelTaskFunction StdInterfaceOpen;


/** @brief Define kernel task as standard Close function
 *
 */
typedef ezKernelTaskFunction StdInterfaceClose;


/** @brief Define kernel task as standard Write function
 *
 */
typedef ezKernelTaskFunction StdInterfaceWrite;


/** @brief Define kernel task as standard Read function
 *
 */
typedef ezKernelTaskFunction StdInterfaceRead;


/** @brief  Standard interface for the driver. When there is not constrain, the
 *          application is encourage to use this type of interface to interact
 *          with the hardware peripheral instead of calling the HAL function.
 */
struct ezStdInterface
{
    StdInterfaceOpen Open;      /* TBD */
    StdInterfaceClose Close;    /* TBD */
    StdInterfaceWrite Write;    /* Transfer data function */
    StdInterfaceRead Read;      /* Read data function */
};


/** @brief A structure for sending data by using standard interface
 *
 */
struct ezStdInterfaceData
{
    uint8_t *data;          /**< Data to be read or write */
    uint32_t data_size;     /**< size of the data */
    StdInterfaceCallback callback;  /**< callback function to handle event */
};


/** @brief A structure to store data of a driver object
 *
 */
struct ezDriver
{
    bool is_busy;       /**< Busy flag, act as binary mutex */
    void *config;       /**< Dedicated configuration according to peripheral */
    ezDriverInitFunction Initialize;    /**< Pointer to driver init function */
    void *ll_interface;                 /**< Pointer to low layer interface */
    struct ezStdInterface *std_interface;   /**< Pointer to standard interface */
};


/** @brief handle for the ezDriver sructure
 *
 */
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
* @Description: Initialize the driver module
*
* @param    None
* @return   None
*
*
*******************************************************************************/
void ezDriver_Initialize(void);


/******************************************************************************
* Function : ezDriver_GetDriver
*//**
* @Description: Searching and returning the driver by its name
*
* @param    driver_name: (IN)Name of the driver
* @return   Driver handle or null
*
*
*******************************************************************************/
ezDriveHandle ezDriver_GetDriver(char *driver_name);


/******************************************************************************
* Function : ezDriver_Open
*//**
* @Description: Open function. Internally this function will create a kernel
*               task, which in turn, calls the open function from the standard
*               interface. See ezStdInterface.
*
* @param    handle: (IN)Driver handle
* @param    data:   (IN)Pointer to the data structure. See ezStdInterfaceData
* @return   -
*
*******************************************************************************/
ezSTATUS ezDriver_Open(ezDriveHandle handle, struct ezStdInterfaceData *data);


/******************************************************************************
* Function : ezDriver_Write
*//**
* @Description: Write function. Internally this function will create a kernel
*               task, which in turn, calls the write function from the standard
*               interface. See ezStdInterface.
*
* @param    handle: (IN)Driver handle
* @param    data:   (IN)Pointer to the data structure. See ezStdInterfaceData
* @return   -
*
*******************************************************************************/
ezSTATUS ezDriver_Write(ezDriveHandle handle, struct ezStdInterfaceData *data);


/******************************************************************************
* Function : ezDriver_Read
*//**
* @Description: Read function. Internally this function will create a kernel
*               task, which in turn, calls the read function from the standard
*               interface. See ezStdInterface.
*
* @param    handle: (IN)Driver handle
* @param    data:   (IN)Pointer to the data structure. See ezStdInterfaceData
* @return   -
*
*******************************************************************************/
ezSTATUS ezDriver_Read(ezDriveHandle handle, struct ezStdInterfaceData *data);


/******************************************************************************
* Function : ezDriver_Close
*//**
* @Description: Close function. Internally this function will create a kernel
*               task, which in turn, calls the close function from the standard
*               interface. See ezStdInterface.
*
* @param    handle: (IN)Driver handle
* @param    data:   (IN)Pointer to the data structure. See ezStdInterfaceData
* @return   -
*
*******************************************************************************/
ezSTATUS ezDriver_Close(ezDriveHandle handle, struct ezStdInterfaceData *data);


#endif /* CONFIG_DRIVERINF == 1U */
#endif /* _EZDRIVER_H */

/* End of file */

