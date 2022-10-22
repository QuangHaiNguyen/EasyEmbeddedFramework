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
#include "ezUtilities/logging/logging.h"

#if (CONFIG_HAL_UART == 1U)

#include "ezApp/ezDriver/ezDriver.h"


#if (CONFIG_ESP32 == 1U)
#include "platforms/esp32/uart/esp32_uart.h"
#endif /* CONFIG_WIN */

#if (CONFIG_STM32 == 1U)
//#include "../../Core/target_driver/stm32_uart.h"
#endif

#if (CONFIG_WIN == 1U)
#include "ezTargets/simulator/uart/uart_sim.h"
#endif

#if(CONFIG_VIRTUAL_COM == 1U)
#include "ezTargets/simulator/virtual_com/virtual_com_driver.h"
#endif /* CONFIG_VIRTUAL_COM */



/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

#if (CONFIG_VIRTUAL_COM == 1U)
static const char *virtual_com_name = "virtual com";
#endif /* CONFIG_VIRTUAL_COM == 1U */

static struct ezDriver cli_uart = { 0 };
static struct ezDriver virtual_com = { 0 };


/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/********************** Public functions **************************************/

void *ezHal_Uart_LinkCliDriv(void)
{
    cli_uart.is_busy = false;
    cli_uart.config = NULL;
    cli_uart.Initialize = simUart_Initialize;
    cli_uart.ll_interface = (void*)simUart_GetApi();
    cli_uart.std_interface = (struct ezStdInterface*)simUart_GetStdInterface();

    return (void *)&cli_uart;
}


#if (CONFIG_VIRTUAL_COM == 1U)
void *ezHal_VirtualCom_LinkDriv(void)
{
    virtual_com.is_busy = false;
    virtual_com.config = NULL;
    virtual_com.Initialize = VirtualCom_Initialize;
    virtual_com.ll_interface = (void *)VirtualCom_GetLowLayerInterface();
    virtual_com.std_interface = (struct ezStdInterface *)VirtualCom_GetStdInterface();

    return (void *)&virtual_com;
}
#endif /* VIRTUAL_COM */


#endif /* CONFIG_HAL_UART > 0U */
/* End of file*/
