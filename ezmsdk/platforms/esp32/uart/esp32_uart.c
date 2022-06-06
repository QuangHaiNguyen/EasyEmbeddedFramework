#include "esp_uart.h"

#if (SUPPORTED_CHIP == ESP32)
#include "hal/uart/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "driver/uart.h"
#include <string.h>
#include <stdio.h>

#define ESP_UART1_TX_PIN    23U
#define ESP_UART1_RX_PIN    22U
#define BUFF_SIZE           512

#if (MODULE_DEBUG == 1U) && (UART_DEBUG == 1U)
    #define TARGETPRINT1(a)                       PRINT_DEBUG1(a)               
    #define TARGETPRINT2(a,b)                     PRINT_DEBUG2(a,b)             
    #define TARGETPRINT3(a,b,c)                   PRINT_DEBUG3(a,b,c) 
    #define TARGETHEXDUMP(a,b,c)                  ezmHexdump(a, b, c)
#else 
    #define TARGETPRINT1(a)           
    #define TARGETPRINT2(a,b)           
    #define TARGETPRINT3(a,b,c)
    #define UARTHEXDUMP(a,b,c)
#endif

typedef struct
{
    uint8_t         u8EspUartInstance;
    QueueHandle_t   *phUartQueue;
    bool (*pfnCallback)(UART_NOTIFY_CODE eCode, void * pParam);
}EspUartHandle;


static QueueHandle_t    hUart0Queue;


EspUartHandle astHandle[NUM_OF_SUPPORTED_UART] = 
{
    {UART_NUM_1, &hUart0Queue, NULL},
};

/* Uart 0 section */
static void         EspUart0_EventTask(void *pvParameters);
static uint16_t     EspUart0_Send(uint8_t * au8Buffer, uint16_t u16Size);
static uint16_t     EspUart0_Receive(uint8_t * au8Buffer, uint16_t u16Size);
static void         EspUart0_RegisterCallBack(UART_CALLBACK pfnCallback);
static void         EspUart0_UnRegisterCallBack(void);

bool espUart_Init(uint8_t u8UartIndex, ezmUart * pstUat)
{
    bool bResult = false;
    uart_config_t stUartConfig;

    if (u8UartIndex == HAL_UART0 && pstUat != NULL)
    {
        /* Configure UART parameter */
        stUartConfig.baud_rate = 115200;
        stUartConfig.data_bits = UART_DATA_8_BITS;
        stUartConfig.parity = UART_PARITY_DISABLE;
        stUartConfig.stop_bits = UART_STOP_BITS_1;
        stUartConfig.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
        stUartConfig.source_clk = UART_SCLK_APB;

        /* init uart */
        uart_set_pin(astHandle[u8UartIndex].u8EspUartInstance, ESP_UART1_TX_PIN, ESP_UART1_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
        uart_driver_install(astHandle[u8UartIndex].u8EspUartInstance, BUFF_SIZE, BUFF_SIZE, 20, astHandle[u8UartIndex].phUartQueue, 0);
        uart_param_config(astHandle[u8UartIndex].u8EspUartInstance, &stUartConfig);

        /* init event task handle for UART */
        xTaskCreate(EspUart0_EventTask, "uart0_event_task", 2048, NULL, 12, NULL);

        /* bindind the high level API to low level API */
        pstUat->ezmUart_Send = EspUart0_Send;
        pstUat->ezmUart_Receive = EspUart0_Receive;
        pstUat->ezmUart_RegisterCallback = EspUart0_RegisterCallBack;
        pstUat->ezmUart_UnregisterCallback = EspUart0_UnRegisterCallBack;

        bResult = true;
    }

    return bResult;
}

static uint16_t EspUart0_Send(uint8_t * au8Buffer, uint16_t u16Size)
{
    uint16_t u16ReturnSize;
    int u32ReturnSize;

    u32ReturnSize = uart_write_bytes(astHandle[HAL_UART0].u8EspUartInstance, (const uint8_t*) au8Buffer, u16Size);

    if(u32ReturnSize == -1)
    {
        TARGETPRINT1("Param error");
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

    u32ReturnSize = uart_read_bytes(astHandle[HAL_UART0].u8EspUartInstance, au8Buffer, u16Size, portMAX_DELAY);

    if(u32ReturnSize == -1)
    {
        TARGETPRINT1("Param error");
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

#endif /* SUPPORTED_CHIP */