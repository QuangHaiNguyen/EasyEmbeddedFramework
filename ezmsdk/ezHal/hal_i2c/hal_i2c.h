
/*******************************************************************************
* Filename:         hal_i2c.h
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

/** @file   hal_i2c.h
 *  @author Hai Nguyen
 *  @date   22.10.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _HAL_I2C_H
#define _HAL_I2C_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezApp/ezSdk_config.h"

#if (CONFIG_HAL_I2C == 1U)

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief I2C mode
 *
 */
typedef enum
{
    MODE_MASTER,    /**< Master mode */
    MODE_SLAVE,     /**< Slave mode */
}EZ_I2C_MODE;


/** @brief I2C speed
 *
 */
typedef enum
{
    STANDARD_SPEED,     /**< 100kbps */
    FAST_SPEED,         /**< 400 kbps */
    HIGH_SPEED,         /**< 3.4 Mbps */
    ULTRA_FAST_SPEED,   /**< 5 Mbps */
}EZ_I2C_SPEED;


/** @brief Configuration to send to the low layer for setting up I2C
 *  
 */
struct ezI2cConfig
{
    EZ_I2C_MODE mode;   /**< mode, see EZ_I2C_MODE*/
    EZ_I2C_SPEED speed; /**< speed, see EZ_I2C_SPEED*/
};


/** @brief I2C data frame
 *
 */
struct ezI2cDataFrame
{
    uint16_t address;
    uint8_t *data;
    uint32_t data_size;
};


/** @brief pointer to I2C callback
 *
 */
typedef uint32_t(*ezI2cCallback)(uint32_t i2c_notify_code,
    void *param1,
    void *param2);


/** @brief Pointer to I2C send function
 *
 */
typedef ezSTATUS(*ezI2cSend)(struct ezI2cDataFrame *i2c_data_frame);


/** @brief Pointer to I2C receive function
 *
 */
typedef ezSTATUS(*ezI2cReceive)(struct ezI2cDataFrame *i2c_data_frame);


/** @brief Pointer to I2C set config function
 *
 */
typedef ezSTATUS(*ezI2cSetConfig)(struct ezI2cConfig *config);


/** @brief Pointer to I2C get function
 *
 */
typedef ezSTATUS(*ezI2cGetConfig)(struct ezI2cConfig *config);


/** @brief Pointer to I2C register callback function
 *
 */
typedef ezSTATUS(*ezI2cRegisterCallback)(ezI2cCallback callback);


/** @brief Pointer to I2C unregister callback function
 *
 */
typedef ezSTATUS(*ezI2cUnregisterCallback)(void);


/** @brief structure defining the I2C low level drier interface
 *
 */
struct ezI2cHalDrvInf
{
    ezI2cSend send;                 /**< pointer to send function*/
    ezI2cReceive receive;           /**< pointer to receive function*/
    ezI2cSetConfig set_config;      /**< pointer to set config function*/
    ezI2cGetConfig get_config;      /**< pointer to get config function*/
    ezI2cRegisterCallback register_callback;    /**< pointer to reg callback function*/
    ezI2cUnregisterCallback unregister_callback;/**< pointer to un-reg callback function*/
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/

/******************************************************************************
* Function : ezI2c_LinkDriver
*//**
* @Description:
*
* This function initializes the ring buffer
*
* @param    None
* @return   None
*
*
*******************************************************************************/
void *ezI2c_LinkDriver(void);

#endif /* CONFIG_HAL_I2C */
#endif /* _HAL_I2C_H */

/* End of file */

