
/*******************************************************************************
* Filename:         ez_windows_echo.c
* Author:           Hai Nguyen
* Original Date:    13.06.2023
* Last Update:      13.06.2023
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
* Written by Hai Nguyen 13.06.2023
*
*******************************************************************************/

/** @file   ez_windows_echo.c
 *  @author Hai Nguyen
 *  @date   13.06.2023
 *  @brief  This is the source of the echo driver implementation.
 *
 *  @details hardware-depent implementation of the echo driver
 *
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "ez_windows_echo.h"

#define DEBUG_LVL   LVL_CRITICAL            /**< logging level */
#define MOD_NAME    "ez_windows_echo"       /**< module name */

#include "utilities/logging/ez_logging.h"
#include "utilities/assert/ez_assert.h"
#include "utilities/ring_buffer/ez_ring_buffer.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define BUFF_SIZE           128  /**< Size of the buffer used for ring buffer */
#define ECHO_INTERFACE_1    0
#define ECHO_INTERFACE_2    1
#define NUM_OF_DRIVER       2


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief: Hold the internal data and API implementation
 */
struct ezEchoDriverInternal
{
    struct ezHalEchoDriver  hal_driver;         /**< Hold the implementation of the driver*/
    uint8_t                 buff[BUFF_SIZE];    /**< Buffer to hold echoed data */
    RingBuffer              ring_buff;          /**< Ring buffer */
    ezDriverCallback        callback;           /**< Pointer to callback function to notify event to the HALL layer */
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */


/******************************************************************************
* Function Definitions
*******************************************************************************/
static uint32_t ezEcho_Write(uint8_t driver_index, uint8_t *data, uint32_t data_size);
static uint32_t ezEcho_Read(uint8_t driver_index, uint8_t *data, uint32_t data_size);
static void ezEcho_Initialize(uint8_t driver_index);


static struct ezEchoDriverInternal driver_internal[NUM_OF_DRIVER] =
{
    {
        .hal_driver =
        {
            .index = ECHO_INTERFACE_1,
            .initialized = false,
            .api =
            {
                .Initialize = ezEcho_Initialize,
                .Read = ezEcho_Read,
                .Write = ezEcho_Write,
            },
            .config =
            {
                .size_of_ring_buff = BUFF_SIZE,
            },
        },
        .callback = NULL,
        /* buff is handle in function... */
        /* ring_buff is handle in function... */
    },
    {
        .hal_driver =
        {
            .index = ECHO_INTERFACE_2,
            .initialized = false,
            .api =
            {
                .Initialize = ezEcho_Initialize,
                .Read = ezEcho_Read,
                .Write = ezEcho_Write,
            },
            .config =
            {
                .size_of_ring_buff = BUFF_SIZE,
            },
        },
        .callback = NULL,
        /* buff is handle in function... */
        /* ring_buff is handle in function... */
    },
};


/******************************************************************************
* External functions
*******************************************************************************/
const struct ezHalEchoDriver *ezTargetEcho_GetDriver(uint8_t driver_index)
{
    ASSERT((driver_index < NUM_OF_DRIVER));

    EZDEBUG("ezEchoGetDriver(index = %d)", driver_index);

    const struct ezHalEchoDriver *ptr_driver = NULL;

    if (driver_index < NUM_OF_DRIVER)
    {
        ptr_driver = &driver_internal[driver_index].hal_driver;
    }/* else invalid arguments */

    return ptr_driver;
}


void ezTargetEcho_SetCallback(uint8_t driver_index, ezDriverCallback callback)
{
    ASSERT((driver_index < NUM_OF_DRIVER));
    ASSERT((callback != NULL));

    EZDEBUG("ezTargetEcho_SetCallback(index = %d)", driver_index);

    if (driver_index < NUM_OF_DRIVER && callback)
    {
        driver_internal[driver_index].callback = callback;

        EZDEBUG("Set callback OK");
    }/* else invalid arguments */
}


struct ezHalEchoConfiguration *ezTargetEcho_GetConfiguration(uint8_t driver_index)
{
    ASSERT((driver_index < NUM_OF_DRIVER));

    struct ezHalEchoConfiguration *ptr_config = NULL;

    EZDEBUG("ezTargetEcho_GetConfiguration(index = %d)", driver_index);

    if (driver_index < NUM_OF_DRIVER
        && driver_internal[driver_index].hal_driver.initialized)
    {
        ptr_config = &driver_internal[driver_index].hal_driver.config;

        EZDEBUG("ring buff size = %d", ptr_config->size_of_ring_buff);
    }/* else invalid arguments */

    return ptr_config;
}


/******************************************************************************
* Internal functions
*******************************************************************************/

/******************************************************************************
* Function : ezEcho_Initialize
*//**
* @Description: Initialize the internal data for the driver
*
* @param: (IN)driver_index: index of the driver.
*
* @return: -
*
*******************************************************************************/
static void ezEcho_Initialize(uint8_t driver_index)
{
    ASSERT((driver_index < NUM_OF_DRIVER));

    if (driver_index < NUM_OF_DRIVER)
    {
        ezmRingBuffer_Init(&driver_internal[driver_index].ring_buff,
            driver_internal[driver_index].buff,
            BUFF_SIZE);

        driver_internal[driver_index].hal_driver.initialized = true;
    }/* else invalid arguments */
}


/******************************************************************************
* Function : ezEcho_Write
*//**
* @Description: Write data to the ring buffer
*
* @param: (IN)driver_index: index of the driver
* @param: (IN)data: pointer to written data
* @param: (IN)data_size: size of written data
*
* @return: Number of written bytes
*
*******************************************************************************/
static uint32_t ezEcho_Write(uint8_t driver_index, uint8_t *data, uint32_t data_size)
{
    ASSERT((driver_index < NUM_OF_DRIVER));
    ASSERT((data != NULL));

    uint32_t ret_size = 0;

    EZDEBUG("ezEcho_Write(index = %d)", driver_index);

    if ((driver_index < NUM_OF_DRIVER) && (NULL != data) && (data_size > 0))
    {
        ret_size = ezmRingBuffer_Push(&driver_internal[driver_index].ring_buff,
                                        data,
                                        data_size);

        if (NULL != driver_internal[driver_index].callback)
        {
            driver_internal[driver_index].callback(ECHO_TX_CMPLT, NULL, NULL);
        }

        EZTRACE("wrote data to ring buff");
        EZHEXDUMP(data, ret_size);
    }/* else invalid arguments */

    return ret_size;
}


/******************************************************************************
* Function : ezEcho_Read
*//**
* @Description: Read data from the ring buffer
*
* @param: (IN)driver_index: index of the driver
* @param: (OUT)data: pointer to read data
* @param: (IN)data_size: size of read data
*
* @return: Number of read bytes
*
*******************************************************************************/
static uint32_t ezEcho_Read(uint8_t driver_index, uint8_t *data, uint32_t data_size)
{
    ASSERT((driver_index < NUM_OF_DRIVER));
    ASSERT((data != NULL));

    uint32_t ret_size = 0;

    EZDEBUG("ezEchoRead(index = %d)", driver_index);

    if ((driver_index < NUM_OF_DRIVER) && (NULL != data) && (data_size > 0))
    {
        ret_size = ezmRingBuffer_Pop(&driver_internal[driver_index].ring_buff,
                                        data,
                                        data_size);

        if (NULL != driver_internal[driver_index].callback)
        {
            driver_internal[driver_index].callback(ECHO_RX_CMPLT, (void*)data, (void *)&data_size);
        }

        EZTRACE("read data to buff");
        EZHEXDUMP(data, ret_size);
    }/* else invalid arguments */

    return ret_size;
}


/* End of file*/

