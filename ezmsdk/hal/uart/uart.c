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

#define DEBUG_LVL   LVL_INFO       /**< logging level */
#define MOD_NAME    "UART"

#if (NUM_OF_SUPPORTED_UART > 0U)
#include "ezmDriver/driver.h"
#include "ezmDebug/config.h"
#include "ezmDebug/ezmDebug.h"
#include "utilities/hexdump/hexdump.h"
#include "utilities/logging/logging.h"

#if (SUPPORTED_CHIP == ESP32)
#include "platforms/esp32/uart/esp32_uart.h"
#elif (SUPPORTED_CHIP == STM32)
//#include "../../Core/target_driver/stm32_uart.h"
#elif (SUPPORTED_CHIP == WIN)
#include "platforms/simulator/uart/uart_sim.h"
#endif /* SUPPORTED_CHIP == ESP32 */


/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static const char *cli_uart_name = "cli uart";
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
    if(espUart_Init(CLI_UART, &aszSupportedUart[CLI_UART].driver_api) == true)
    {
        aszSupportedUart[CLI_UART].init_function = EspUart0_Init;
        aszSupportedUart[CLI_UART].is_busy = false;
        ret_driver = &aszSupportedUart[CLI_UART];
        INFO("UART0 init complete");
    }
#elif(SUPPORTED_CHIP == STM32)
    aszSupportedUart[CLI_UART].is_busy = false;
    aszSupportedUart[CLI_UART].init_function = stmUart1_Init;
    aszSupportedUart[CLI_UART].driver_api = stmUart_GetApi(CLI_UART);
    ret_driver = &aszSupportedUart[CLI_UART];
    INFO("UART0 init complete");
#elif(SUPPORTED_CHIP == WIN)
    aszSupportedUart[CLI_UART].is_busy = false;
    aszSupportedUart[CLI_UART].init_function = simUart_Init;
    aszSupportedUart[CLI_UART].driver_api = simUart_GetApi(CLI_UART);
    aszSupportedUart[CLI_UART].driver_name = cli_uart_name;
    ret_driver = &aszSupportedUart[CLI_UART];
    INFO("UART0 init complete");
#endif /* SUPPORTED_CHIP */
    return (void*)ret_driver;
}


#endif /* NUM_OF_SUPPORTED_UART > 0U */
/* End of file*/
