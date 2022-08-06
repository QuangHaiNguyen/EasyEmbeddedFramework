/*******************************************************************************
* Title                 :   module 
* Filename              :   module.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 21.02.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  module.c
 *  @brief This is the source template for a module
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "uart_sim.h"
#include "app/app_config.h"

#define DEBUG_LVL   LVL_INFO       /**< logging level */
#define MOD_NAME    "HW_UART"

#if(HAL_UART == 1U && SUPPORTED_CHIP == WIN)
#include <stdint.h>
#include <stdio.h>
#include "ezmDebug/ezmDebug.h"
#include "utilities/hexdump/hexdump.h"
#include "utilities/logging/logging.h"


/******************************************************************************
* Module Typedefs
*******************************************************************************/

typedef struct
{
    UartDrvApi uart_api;
    UART_CALLBACK callback;
}HwUart;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
static uint16_t UartSim_Read                (uint8_t*buff, uint16_t size);
static uint16_t UartSim_Write               (uint8_t *buff, uint16_t size);
static void     UartSim_RegisterCallback    (UART_CALLBACK call_back);
static void     UartSim_UnRegisterCallback  (void);

static HwUart hw_uarts[NUM_OF_SUPPORTED_UART] = { 0 };

bool simUart_Init(void)
{
    hw_uarts[CLI_UART].uart_api.ezmUart_Receive = UartSim_Read;
    hw_uarts[CLI_UART].uart_api.ezmUart_Send = UartSim_Write;
    hw_uarts[CLI_UART].uart_api.ezmUart_RegisterCallback = UartSim_RegisterCallback;
    hw_uarts[CLI_UART].uart_api.ezmUart_UnregisterCallback = UartSim_UnRegisterCallback;
    INFO("init complete");
    return true;
}

UartDrvApi*simUart_GetApi(uint8_t hw_uart_index)
{
    UartDrvApi* api = NULL;
    if (hw_uart_index < NUM_OF_SUPPORTED_UART)
    {
        api = &hw_uarts[hw_uart_index].uart_api;
    }
    return api;
}


static uint16_t UartSim_Read(uint8_t* buff, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++)
    {
        buff[i] = (uint8_t)_getchar_nolock();
    }

    if (hw_uarts[CLI_UART].callback)
    {
        (void)hw_uarts[CLI_UART].callback((uint8_t)UART_RX_COMPLT, (uint16_t*)&size);
    }

    return size;
}

static uint16_t UartSim_Write(uint8_t* buff, uint16_t size)
{
    for (uint16_t i = 0; i < size; i++)
    {
        putchar(buff[i]);
    }

    if (hw_uarts[CLI_UART].callback)
    {
        (void)hw_uarts[CLI_UART].callback((uint8_t)UART_TX_COMPLT, NULL);
    }
    return size;
}

static void UartSim_RegisterCallback(UART_CALLBACK call_back)
{
    hw_uarts[CLI_UART].callback = call_back;
}

static void UartSim_UnRegisterCallback(void)
{
    hw_uarts[CLI_UART].callback = NULL;
}

#endif
/* End of file*/