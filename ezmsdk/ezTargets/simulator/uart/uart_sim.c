
/*******************************************************************************
* Filename:         uart_sim.c.c
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

/** @file   uart_sim.c
 *  @author Hai Nguyen
 *  @date   22.10.2022
 *  @brief  This is the source of the uart simulator module
 *
 *  @details this module simulates the uart driver on window OS
 *
 */

 /******************************************************************************
 * Includes
 *******************************************************************************/
#include "ezApp/ezSdk_config.h"

#if(CONFIG_SIM_UART == 1U && SUPPORTED_CHIP == WIN)

#define DEBUG_LVL   LVL_INFO       /**< logging level */
#define MOD_NAME    "HW_UART"
#include "ezUtilities/logging/logging.h"

#include "uart_sim.h"
#include "ezApp/ezDriver/ezDriver.h"


/******************************************************************************
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
static struct ezStdInterface sim_uart_std_inf = { 0 };
static UartDrvApi uart_sim_api = { 0 };
static UART_CALLBACK sim_uart_callback;


/******************************************************************************
* Function Definitions
*******************************************************************************/
static uint16_t UartSim_Read                (uint8_t*buff, uint32_t size);
static uint16_t UartSim_Write               (uint8_t *buff, uint32_t size);
static void     UartSim_RegisterCallback    (UART_CALLBACK call_back);
static void     UartSim_UnRegisterCallback  (void);
static KERNEL_TASK_STATUS simUart_Open(void *task_data,
    uint32_t task_data_size);

static KERNEL_TASK_STATUS simUart_Close(void *task_data,
    uint32_t task_data_size);

static KERNEL_TASK_STATUS simUart_Write(void *task_data,
    uint32_t task_data_size);

static KERNEL_TASK_STATUS simUart_Read(void *task_data,
    uint32_t task_data_size);


/******************************************************************************
* External functions
*******************************************************************************/

ezSTATUS simUart_Initialize(void)
{
    uart_sim_api.ezmUart_Receive = UartSim_Read;
    uart_sim_api.ezmUart_Send = UartSim_Write;
    uart_sim_api.ezmUart_RegisterCallback = UartSim_RegisterCallback;
    uart_sim_api.ezmUart_UnregisterCallback = UartSim_UnRegisterCallback;

    return ezSUCCESS;
}

void *simUart_GetStdInterface(void)
{
    sim_uart_std_inf.Open = simUart_Open;
    sim_uart_std_inf.Close = simUart_Close;
    sim_uart_std_inf.Write = simUart_Write;
    sim_uart_std_inf.Read = simUart_Read;

    return (void*)&sim_uart_std_inf;
}


UartDrvApi*simUart_GetApi(void)
{
    return &uart_sim_api;
}


/******************************************************************************
* Internal functions
*******************************************************************************/


/******************************************************************************
* Function : UartSim_Read
*//**
* @Description:
*
* @param    None
* @return   None
*
*******************************************************************************/
static uint16_t UartSim_Read(uint8_t* buff, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        buff[i] = (uint8_t)_getchar_nolock();
    }

    if (sim_uart_callback)
    {
        sim_uart_callback((uint8_t)UART_RX_COMPLT, (uint16_t*)&size);
    }

    return size;
}


/******************************************************************************
* Function : UartSim_Write
*//**
* @Description:
*
* @param    None
* @return   None
*
*******************************************************************************/
static uint16_t UartSim_Write(uint8_t* buff, uint32_t size)
{
    for (uint32_t i = 0; i < size; i++)
    {
        putchar(buff[i]);
    }

    if (sim_uart_callback)
    {
        sim_uart_callback((uint8_t)UART_TX_COMPLT, NULL);
    }
    return size;
}


/******************************************************************************
* Function : UartSim_RegisterCallback
*//**
* @Description:
*
* @param    None
* @return   None
*
*******************************************************************************/
static void UartSim_RegisterCallback(UART_CALLBACK call_back)
{
    sim_uart_callback = call_back;
}


/******************************************************************************
* Function : UartSim_UnRegisterCallback
*//**
* @Description:
*
* @param    None
* @return   None
*
*******************************************************************************/
static void UartSim_UnRegisterCallback(void)
{
    sim_uart_callback = NULL;
}


/******************************************************************************
* Function : simUart_Open
*//**
* @Description:
*
* @param    None
* @return   None
*
*******************************************************************************/
static KERNEL_TASK_STATUS simUart_Open(void *task_data,
    uint32_t task_data_size)
{
    KERNEL_TASK_STATUS status = TASK_STATUS_OK;

    return status;
}


/******************************************************************************
* Function : simUart_Close
*//**
* @Description:
*
* @param    None
* @return   None
*
*******************************************************************************/
static KERNEL_TASK_STATUS simUart_Close(void *task_data,
    uint32_t task_data_size)
{
    KERNEL_TASK_STATUS status = TASK_STATUS_OK;

    return status;
}


#endif /* CONFIG_SIM_UART */
/* End of file*/
