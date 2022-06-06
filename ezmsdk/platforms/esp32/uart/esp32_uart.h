#ifndef ESP_UART_H
#define ESP_UART_H

#include <stdint.h>
#include <stdbool.h>
#include "app/app_config.h"
#include "hal/uart/uart.h"

#if (SUPPORTED_CHIP == ESP32)

bool espUart_Init(uint8_t u8UartIndex, ezmUart * pstUat);

#endif /* SUPPORTED_CHIP */
#endif /* ESP_UART_H */