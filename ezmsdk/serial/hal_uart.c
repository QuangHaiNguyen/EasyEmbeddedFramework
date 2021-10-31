#include "hal_uart.h"
#include "serial.h"
#include <stdlib.h>

HAL_Status HAL_UART_InternalInit(HAL_UART * str_Serial);

HAL_Status HAL_UART_GetConfig(HAL_UART * str_Serial)
{
    HAL_Status eStatus;

    /* point our serial to the hardware*/
    str_Serial->HAL_UART_Init = HAL_UART_InternalInit;
    str_Serial->HAL_UART_Write = Serial_Transmit;
    str_Serial->HAL_UART_Read = Serial_Receive;
    str_Serial->HAL_UART_Deinit = Serial_DeInit;
    str_Serial->HAL_UART_Getstatus = Serial_GetStatus;

    eStatus = HAL_OK;
    
    return eStatus;
}

HAL_Status HAL_UART_InternalInit(HAL_UART * str_Serial)
{
    HAL_Status eStatus;
    /* Check if hardware is init*/
    if(!Serial_IsInit())
    {
        Serial_Init();
    }
    else
    {
        /* Do nothing*/
    }
    eStatus = HAL_OK;
    return eStatus;
}

HAL_Status HAL_UART_DeConfig(HAL_UART * str_Serial)
{
    HAL_Status eStatus;
    str_Serial->HAL_UART_Init = NULL;
    str_Serial->HAL_UART_Write = NULL;
    str_Serial->HAL_UART_Read = NULL;
    str_Serial->HAL_UART_Deinit = NULL;
    str_Serial->HAL_UART_Getstatus = NULL;
    eStatus = HAL_OK;
    return eStatus;
}