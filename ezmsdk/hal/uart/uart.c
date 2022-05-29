/*******************************************************************************
* Title                 :   uart 
* Filename              :   uart.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   15.01.2021
* Version               :   1.0.0
*
*******************************************************************************/

/** @file  uart.c
 *  @brief This is the source file for uart module
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "uart.h"

#if (NUM_OF_SUPPORTED_UART > 0U)
#include "ezmDriver/ezmDriver.h"
#include "ezmDebug/config.h"
#include "ezmDebug/ezmDebug.h"
#include "helper/hexdump/hexdump.h"

#if (SUPPORTED_CHIP == ESP32)
#include "esp_uart/esp_uart.h"
#elif (SUPPORTED_CHIP == STM32)
#include "../../Core/target_driver/stm32_uart.h"
#elif (SUPPORTED_CHIP == WIN)
#include "platforms/simulator/uart/uart_sim.h"
#endif /* SUPPORTED_CHIP == ESP32 */

#define MOD_NAME    "UART"
#if (MODULE_DEBUG == 1U) && (UART_DEBUG == 1U)
    #define UARTPRINT(a)                        PRINT_DEBUG(MOD_NAME,a)
    #define UARTPRINT1(a,b)                     PRINT_DEBUG1(MOD_NAME,a,b)
    #define UARTPRINT2(a,b,c)                   PRINT_DEBUG2(MOD_NAME,a,b,c)
    #define UARTPRINT3(a,b,c,d)                 PRINT_DEBUG3(MOD_NAME,a,b,c,d)
    #define UARTPRINT4(a,b,c,d,e)               PRINT_DEBUG4(MOD_NAME,a,b,c,d,e)
    #define UARTHEXDUMP(a,b)                    ezmHexdump(a, b)
#else 
    #define UARTPRINT(a)
    #define UARTPRINT1(a,b)
    #define UARTPRINT2(a,b,c)
    #define UARTPRINT3(a,b,c,d)
    #define UARTPRINT4(a,b,c,d,e)
    #define UARTHEXDUMP(a,b)
#endif


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static Driver aszSupportedUart[NUM_OF_SUPPORTED_UART] = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/********************** Public functions **************************************/
void* GetUart0Driver(void)
{
    Driver* ret_driver = NULL;
#if (SUPPORTED_CHIP == ESP32)
    bResult = bResult & espUart_Init(CLI_UART, &aszSupportedUart[CLI_UART].stPublicApi);
#elif(SUPPORTED_CHIP == STM32)
    aszSupportedUart[CLI_UART].is_busy = false;
    aszSupportedUart[CLI_UART].init_function = stmUart1_Init;
    aszSupportedUart[CLI_UART].driver_api = stmUart_GetApi(CLI_UART);
    ret_driver = &aszSupportedUart[CLI_UART];
    UARTPRINT("UART0 init complete");
#elif(SUPPORTED_CHIP == WIN)
    aszSupportedUart[CLI_UART].is_busy = false;
    aszSupportedUart[CLI_UART].init_function = simUart_Init;
    aszSupportedUart[CLI_UART].driver_api = simUart_GetApi(CLI_UART);
    ret_driver = &aszSupportedUart[CLI_UART];
    UARTPRINT("UART0 init complete");
#endif /* SUPPORTED_CHIP */
    return (void*)ret_driver;
}


#endif /* NUM_OF_SUPPORTED_UART > 0U */
/* End of file*/
