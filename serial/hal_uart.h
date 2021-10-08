#ifndef _HAL_UART_H
#define _HAL_UART_H

#include "hal_uart_config.h"

HAL_Status HAL_UART_GetConfig(HAL_UART * str_Serial);
HAL_Status HAL_UART_DeConfig(HAL_UART * str_Serial);


#endif /*_HAL_SERIAL_H*/