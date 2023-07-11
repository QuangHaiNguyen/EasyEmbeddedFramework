
/*******************************************************************************
* Filename:         ez_hal_echo_def.h.h
* Author:           Hai Nguyen
* Original Date:    18.06.2023
* Last Update:      18.06.2023
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
* Written by Hai Nguyen 18.06.2023
*
*******************************************************************************/

/** @file   ez_hal_echo_target_def.h
 *  @author Hai Nguyen
 *  @date   18.06.2023
 *  @brief  Boilerplate for the echo target. Every implmentation of the target
 *          must implemented according to this template
 *  
 *  @details
 * 
 */

#ifndef _EZ_HAL_ECHO_TARGET_DEF_H
#define _EZ_HAL_ECHO_TARGET_DEF_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "string.h"
#include "stdint.h"
#include "stdbool.h"
#include "service/driver/ez_driver_common.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief Define events returned by the target driver.
 */
typedef enum
{
    ECHO_TX_CMPLT,      /* Transmit commplete */
    ECHO_RX_CMPLT,      /* Receive complete */
    ECHO_ERR,           /* Error */
}ezHalEchoCallbackCode;


/******************************************************************************
* Function : ezEcho_TargetWrite
*//**
* @Description: Write data to buffer.
*               Define the function which must be implemented by the target driver
*
* @param: (IN)driver_index: index of the driver.
* @param: (IN)data: pointer to the written data.
* @param: (IN)data_size: size of written data.
*
* @return   Number of written data
*
*******************************************************************************/
typedef uint32_t(*ezEcho_TargetWrite)(uint8_t driver_index,
    uint8_t *data,
    uint32_t data_size);


/******************************************************************************
* Function : ezEcho_TargeRead
*//**
* @Description: Read data from buffer.
*               Define the function which must be implemented by the target driver
*
* @param: (IN)driver_index: index of the driver.
* @param: (OUT)data: pointer to the read data.
* @param: (IN)data_size: size of written data.
*
* @return   Number of read data
*
*******************************************************************************/
typedef uint32_t(*ezEcho_TargeRead)(uint8_t driver_index,
    uint8_t *data,
    uint32_t data_size);


/******************************************************************************
* Function : ezEcho_TargetInitialize
*//**
* @Description: Initialize driver.
*               Define the function which must be implemented by the target driver
*
* @param: (IN)driver_index: index of the driver.
*
* @return   -
*
*******************************************************************************/
typedef void (*ezEcho_TargetInitialize)(uint8_t driver_index);


/** @brief Configuration of the Echo Driver
 */
struct ezHalEchoConfiguration
{
    uint32_t size_of_ring_buff; /**< size of the ring buffer holding data for echoing */
};


/** @brief  Hold the functions must be implemented by the target driver.
 *          These functions are expected by the Echo HAL layer
 */
struct ezTargetEchoApi
{
    ezEcho_TargetWrite Write;           /**< Pointer to the write function */
    ezEcho_TargeRead Read;              /**< Pointer to the read function */
};


/** @brief Hold the data and api, which must be implemented by the targer driver.
 *         This block is the boiler plate for the implementation of the driver.
 */
struct ezHalEchoDriver
{
    struct ezDriverCommon common;           /**< Common data and function, MUST BE THE FIRST ELEMENT OF STRUCT */
    struct ezHalEchoConfiguration config;   /**< Configuration of the target driver */
    struct ezTargetEchoApi api;             /**< Pointer to the API implementation */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */


/******************************************************************************
* Function Prototypes
*******************************************************************************/
/* None */


#endif /* _EZ_HAL_ECHO_TARGET_DEF_H */

/* End of file */

