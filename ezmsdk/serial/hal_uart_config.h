#ifndef _HAL_UART_CONFIG_H
#define _HAL_UART_CONFIG_H

#include "status_code.h"
#include "../ring_buffer/ring_buffer.h"

#define BUFF_SIZE 20U

typedef struct HAL_UART
{
    HAL_Status (*HAL_UART_Init)(struct HAL_UART *self);
    HAL_Status (*HAL_UART_Deinit)(void);
    HAL_Status (*HAL_UART_Read)(struct HAL_UART *self, uint8_t u8Size);
    HAL_Status (*HAL_UART_Write)(struct HAL_UART *self, uint8_t u8Size);
    HAL_Status (*HAL_UART_Getstatus)(void);
    uint8_t ptr_u8Buffer[BUFF_SIZE];
}HAL_UART;



#define USING_CALLBACK  0 /**< macro to enable using callback */
#define EMBEDDED        0 /**< macro to identify running on emdedded device*/
#define SIMULATION      1 /**< macro to identify running simulation*/

#endif