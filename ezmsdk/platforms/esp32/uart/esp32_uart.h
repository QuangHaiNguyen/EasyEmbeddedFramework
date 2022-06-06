/*******************************************************************************
* Filename:         esp32_uart.h
* Author:           Hai Nguyen
* Original Date:    06.06.2022
* Last Update:      06.06.2022
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
* Written by Hai Nguyen 06.06.2022
*
*******************************************************************************/

/** @file   esp32_uart.h
 *  @author Hai Nguyen
 *  @date   06.06.2022
 *  @brief  This is the header file of esp32 uart driver
 *  
 *  @detail Contain public API to intreract with the driver
 * 
 */

#ifndef ESP_UART_H
#define ESP_UART_H

#include "app/app_config.h"

#if (HAL_UART == 1U && SUPPORTED_CHIP == ESP32)
/*******************************************************************************
* Includes
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "hal/uart/uart.h"

/*******************************************************************************
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
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool espUart_Init(uint8_t u8UartIndex, void **pstUart);
bool EspUart0_Init(void);

#endif /* (HAL_UART == 1U && SUPPORTED_CHIP == ESP32 */

#endif /* ESP_UART_H */