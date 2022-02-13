/*******************************************************************************
* Title                 :   uart 
* Filename              :   uart.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   15.01.2021
* Version               :   1.0.0
*
*******************************************************************************/

/** @file  uart.c
 *  @brief This is the source template for a uart
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "uart.h"

#if (NUM_OF_SUPPORTED_UART > 0U)
#include "../../ezmDriver/ezmDriver.h"
#include "ezmDebug/config.h"
#include "ezmDebug/ezmDebug.h"
#include "helper/hexdump/hexdump.h"

#if (SUPPORTED_CHIP == ESP32)
#include "esp_uart/esp_uart.h"
#elif (SUPPORTED_CHIP == WIN)
#include "uart_sim.h"
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

/** @brief definition of a new type
 *
 */


static Driver aszSupportedUart[NUM_OF_SUPPORTED_UART] = {0};

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/

/********************** Public functions **************************************/
void* GetUart0Driver(void)
{
    ezmUart* api = aszSupportedUart[CLI_UART].driver_api;
    Driver* ret_driver = NULL;
#if (SUPPORTED_CHIP == ESP32)
    bResult = bResult & espUart_Init(CLI_UART, &aszSupportedUart[CLI_UART].stPublicApi);
#elif(SUPPORTED_CHIP == WIN)

    aszSupportedUart[CLI_UART].is_busy = false;
    aszSupportedUart[CLI_UART].init_function = simUart_Init;
    aszSupportedUart[CLI_UART].driver_api = simUart_GetApi(CLI_UART);
    ret_driver = &aszSupportedUart[CLI_UART];
    UARTPRINT("UART0 init complete");

    return (void*)ret_driver;
#endif /* SUPPORTED_CHIP */
}



#endif /* NUM_OF_SUPPORTED_UART > 0U */
/* End of file*/