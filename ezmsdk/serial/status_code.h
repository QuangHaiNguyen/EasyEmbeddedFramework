#ifndef _STATUS_CODE_H
#define _STATUS_CODE_H

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/**
 *
 * @brief   Status of the serial module
**/
typedef enum
{
    HAL_OK,
    /**< serial ok */
    HAL_TX_BUSY,
    /**< hardware is busy */
    HAL_RX_BUSY,
    /**< hardware is busy */
    HAL_NO_MEM,
    /**< buffer dont have enough memory */
    HAL_DATA_AVAIL,
    /**< data available in buffer */
    HAL_DATA_EMPTY,
    /**< buffer has no data */
    HAL_DATA_FULL,
    /**< buffer is full */
    HAL_ERR,        
    /**< serial module error */
}HAL_Status;

#endif