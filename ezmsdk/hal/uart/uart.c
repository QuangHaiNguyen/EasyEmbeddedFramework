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

#if (CONFIG_HAL_UART == 1U)

#include "ezmDriver/driver.h"
#include "ezmDebug/config.h"
#include "ezmDebug/ezmDebug.h"
#include "utilities/hexdump/hexdump.h"
#include "utilities/logging/logging.h"

#if (CONFIG_ESP32 == 1U)
#include "platforms/esp32/uart/esp32_uart.h"
#endif /* CONFIG_WIN */

#if (CONFIG_STM32 == 1U)
//#include "../../Core/target_driver/stm32_uart.h"
#endif

#if (CONFIG_WIN == 1U)
#include "platforms/simulator/uart/uart_sim.h"
#endif

#if(CONFIG_VIRTUAL_COM == 1U)
#include "platforms/simulator/virtual_com/virtual_com_driver.h"
#endif /* CONFIG_VIRTUAL_COM */



/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static const char *cli_uart_name = "cli uart";

#if (CONFIG_VIRTUAL_COM == 1U)
static const char *virtual_com_name = "virtual com";
#endif /* CONFIG_VIRTUAL_COM == 1U */

static Driver aszSupportedUart[NUM_OF_SUPPORTED_UART] = { 0 };

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/********************** Public functions **************************************/
void* GetUart0Driver(void)
{
    Driver* ret_driver = NULL;
#if (CONFIG_ESP32 == 1U)
    if(espUart_Init(CLI_UART, &aszSupportedUart[CLI_UART].driver_api) == true)
    {
        aszSupportedUart[CLI_UART].init_function = EspUart0_Init;
        aszSupportedUart[CLI_UART].is_busy = false;
        ret_driver = &aszSupportedUart[CLI_UART];
        INFO("UART0 init complete");
    }
#endif

#if(CONFIG_STM32 == 1U)
    aszSupportedUart[CLI_UART].is_busy = false;
    aszSupportedUart[CLI_UART].init_function = stmUart1_Init;
    aszSupportedUart[CLI_UART].driver_api = stmUart_GetApi(CLI_UART);
    ret_driver = &aszSupportedUart[CLI_UART];
    INFO("UART0 init complete");
#endif

#if(CONFIG_WIN == 1U)
    aszSupportedUart[CLI_UART].is_busy = false;
    aszSupportedUart[CLI_UART].init_function = simUart_Init;
    aszSupportedUart[CLI_UART].driver_api = simUart_GetApi(CLI_UART);
    aszSupportedUart[CLI_UART].driver_name = cli_uart_name;
    ret_driver = &aszSupportedUart[CLI_UART];
    INFO("UART0 init complete");
#endif /* SUPPORTED_CHIP */

    return (void*)ret_driver;
}

#if (CONFIG_VIRTUAL_COM == 1U)
void *GetVirtualComDriver(void)
{
    Driver* ret_driver = NULL;

    aszSupportedUart[VCP_UART].is_busy = false;
    aszSupportedUart[VCP_UART].init_function = VirtualCom_Initialization;
    aszSupportedUart[VCP_UART].driver_api = VirtualCom_GetInterface();
    aszSupportedUart[VCP_UART].driver_name = virtual_com_name;
    ret_driver = &aszSupportedUart[VCP_UART];
    INFO("virtual com init complete");

    return ret_driver;
}
#endif /* VIRTUAL_COM */

#endif /* CONFIG_HAL_UART > 0U */
/* End of file*/
