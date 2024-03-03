
/*******************************************************************************
* Filename:         esp32_uart.c
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

/** @file   esp32_uart.c
 *  @author Hai Nguyen
 *  @date   06.06.2022
 *  @brief  This is the source file of eso32 uart module
 *  
 *  @details This file contains the hardware-depedent implementation of esp32
 *  uart driver. The common functions are binded to a wrapper which provides the
 *  commonalities accross platforms and hides users from hardware impülementation 
 * 
 */


#include "esp32_uart.h"

#define DEBUG_LVL   LVL_ERROR       /**< logging level */
#define MOD_NAME    "ESP_UART"      /**< module name */

#if (HAL_UART == 1U && SUPPORTED_CHIP == ESP32 )

/******************************************************************************
* Includes
*******************************************************************************/
/* esp32 header */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include <string.h>
#include <stdio.h>

/* embedded sdk header */
#include "hal/uart/uart.h"
#include "utilities/logging/logging.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define ESP_UART1_TX_PIN    1U
#define ESP_UART1_RX_PIN    2U
#define BUFF_SIZE           512

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* @brief structure to hold the data of a hw uart
 */
typedef struct
{
    uint8_t         u8EspUartInstance;  /**< index of hw uart*/
    QueueHandle_t   *phUartQueue;       /**< pointer to message queue*/ 
    UART_CALLBACK   pfnCallback;        /**< pointer to call back function*/
    UartDrvApi      api;
}EspUartHandle;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static QueueHandle_t    hUart0Queue;

EspUartHandle astHandle[NUM_OF_SUPPORTED_UART] = 
{
    {UART_NUM_1, &hUart0Queue, NULL, {0}},
};

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* Uart 0 section */
static void         EspUart0_EventTask(void *pvParameters);
static uint16_t     EspUart0_Send(uint8_t * au8Buffer, uint16_t u16Size);
static uint16_t     EspUart0_Receive(uint8_t * au8Buffer, uint16_t u16Size);
static void         EspUart0_RegisterCallBack(UART_CALLBACK pfnCallback);
static void         EspUart0_UnRegisterCallBack(void);

/******************************************************************************
* External functions
*******************************************************************************/

/******************************************************************************
* Function : espUart_Init
*//** 
* @Description:
*
* This function initializes the hw uart
* 
* @param    u8UartIndex: (IN)Index of the hw UART
* @param    pstUart: (OUT)pointer to uart api, must be casted to UartDrvApi
* @return   true: success
*           false: fail
*
*
*******************************************************************************/
bool espUart_Init(uint8_t u8UartIndex, void **pstUart)
{
    bool bResult = true;

    DEBUG("espUart_Init()");

    if (u8UartIndex == CLI_UART)
    {
        astHandle[u8UartIndex].api.ezmUart_Send = EspUart0_Send;
        astHandle[u8UartIndex].api.ezmUart_Receive = EspUart0_Receive;
        astHandle[u8UartIndex].api.ezmUart_RegisterCallback = EspUart0_RegisterCallBack;
        astHandle[u8UartIndex].api.ezmUart_UnregisterCallback = EspUart0_UnRegisterCallBack;
        
        /* bindind the high level API to low level API */
        *pstUart = &astHandle[u8UartIndex].api;
        INFO("binding driver success");
    }
    else
    {
        pstUart = NULL;
        bResult = false;
        ERROR("wrong param [index = %d] or handle is NULL", u8UartIndex);
    }

    (void)pstUart;
    return bResult;
}

bool EspUart0_Init(void)
{
    DEBUG("EspUart0_Init()");
    bool success = true;
    esp_err_t err_code = ESP_OK;
    uart_config_t stUartConfig;
    uint8_t u8UartIndex = 0;

    /* Configure UART parameter */
    stUartConfig.baud_rate = 115200;
    stUartConfig.data_bits = UART_DATA_8_BITS;
    stUartConfig.parity = UART_PARITY_DISABLE;
    stUartConfig.stop_bits = UART_STOP_BITS_1;
    stUartConfig.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    stUartConfig.source_clk = UART_SCLK_APB;
    stUartConfig.rx_flow_ctrl_thresh = 0;

    /* init uart */
    err_code = uart_set_pin(astHandle[u8UartIndex].u8EspUartInstance, 
                                ESP_UART1_TX_PIN, ESP_UART1_RX_PIN, 
                                UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    err_code = err_code | uart_driver_install(astHandle[u8UartIndex].u8EspUartInstance, 
                                                BUFF_SIZE, BUFF_SIZE, 
                                                20, 
                                                astHandle[u8UartIndex].phUartQueue, 
                                                0);

    err_code = err_code | uart_param_config(astHandle[u8UartIndex].u8EspUartInstance, 
                                                &stUartConfig);

    if (err_code == ESP_OK)
    {
        /* init event task handle for UART */
        xTaskCreate(EspUart0_EventTask, "uart0_event_task", 2048, NULL, 12, NULL);
        INFO("EspUart0_Init OK");
        uint8_t const test[] = "hello from esp uart\n";
        uart_write_bytes(UART_NUM_1, test, sizeof(test));
    }
    else
    {
        ERROR("EspUart0_Init FAILED");
        success = false;
    }

    return success;
}

/******************************************************************************
* Internal functions
*******************************************************************************/

static uint16_t EspUart0_Send(uint8_t * au8Buffer, uint16_t u16Size)
{
    uint16_t u16ReturnSize;
    int u32ReturnSize;

    TRACE("EspUart0_Send()");
    u32ReturnSize = uart_write_bytes(astHandle[HAL_UART0].u8EspUartInstance, 
                                        (const uint8_t*) au8Buffer, 
                                        u16Size);

    if(u32ReturnSize == -1)
    {
        ERROR("Write error");
        u16ReturnSize = 0U;
    }
    else
    {
        u16ReturnSize = (uint16_t)u32ReturnSize;
    }

    return u16ReturnSize;
}

static uint16_t EspUart0_Receive(uint8_t * au8Buffer, uint16_t u16Size)
{
    uint16_t u16ReturnSize;
    int u32ReturnSize;

    u32ReturnSize = uart_read_bytes(astHandle[HAL_UART0].u8EspUartInstance, 
                                    au8Buffer,
                                    u16Size,
                                    portMAX_DELAY);

    if(u32ReturnSize == -1)
    {
        ERROR("Receive error");
        u16ReturnSize = 0U;
    }
    else
    {
        u16ReturnSize = (uint16_t)u32ReturnSize;
    }

    return u16ReturnSize;
}

static void EspUart0_RegisterCallBack(UART_CALLBACK pfnCallback)
{
    astHandle[HAL_UART0].pfnCallback = pfnCallback;
}

static void EspUart0_UnRegisterCallBack(void)
{
    astHandle[HAL_UART0].pfnCallback = NULL;
}

static void EspUart0_EventTask(void *pvParameters)
{
    uart_event_t stUartEvent;
    while(1)
    {
        if(xQueueReceive(*(astHandle[HAL_UART0].phUartQueue), (void * )&stUartEvent, (portTickType)portMAX_DELAY)) 
        {
            switch(stUartEvent.type) 
            {
                case UART_DATA:
                    if(astHandle[HAL_UART0].pfnCallback != NULL)
                    {
                        TRACE("receive data, [size = %d]", stUartEvent.size);
                        (void)astHandle[HAL_UART0].pfnCallback(UART_RX_COMPLT, &stUartEvent.size);
                    }
                    break;
                case UART_FIFO_OVF:
                    if(astHandle[HAL_UART0].pfnCallback != NULL)
                    {
                        (void)astHandle[HAL_UART0].pfnCallback(UART_UNSUPPORTED, &stUartEvent.size);
                    }
                    uart_flush_input(astHandle[HAL_UART0].u8EspUartInstance);
                    xQueueReset(*(astHandle[HAL_UART0].phUartQueue));
                    break;
                case UART_BUFFER_FULL:
                    if(astHandle[HAL_UART0].pfnCallback != NULL)
                    {
                        (void)astHandle[HAL_UART0].pfnCallback(UART_BUFF_FULL, &stUartEvent.size);
                    }
                    uart_flush_input(astHandle[HAL_UART0].u8EspUartInstance);
                    xQueueReset(*(astHandle[HAL_UART0].phUartQueue));
                    break;
                case UART_BREAK:
                    if(astHandle[HAL_UART0].pfnCallback != NULL)
                    {
                        (void)astHandle[HAL_UART0].pfnCallback(UART_UNSUPPORTED, &stUartEvent.size);
                    }
                    break;
                case UART_PARITY_ERR:
                    if(astHandle[HAL_UART0].pfnCallback != NULL)
                    {
                        (void)astHandle[HAL_UART0].pfnCallback(UART_UNSUPPORTED, &stUartEvent.size);
                    }
                    break;
                case UART_FRAME_ERR:
                    if(astHandle[HAL_UART0].pfnCallback != NULL)
                    {
                        (void)astHandle[HAL_UART0].pfnCallback(UART_UNSUPPORTED, &stUartEvent.size);
                    }
                    break;
                case UART_PATTERN_DET:
                    if(astHandle[HAL_UART0].pfnCallback != NULL)
                    {
                        (void)astHandle[HAL_UART0].pfnCallback(UART_UNSUPPORTED, &stUartEvent.size);
                    }
                    break;
                default:
                    if(astHandle[HAL_UART0].pfnCallback != NULL)
                    {
                        (void)astHandle[HAL_UART0].pfnCallback(UART_UNSUPPORTED, &stUartEvent.size);
                    }
                    break;
            }
        }
    }
    vTaskDelete(NULL);
}

#endif /* (HAL_UART == 1U && SUPPORTED_CHIP == ESP32 */