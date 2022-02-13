/*******************************************************************************
* Title                 :   module 
* Filename              :   module.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 21.02.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  module.c
 *  @brief This is the source template for a module
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "uart_sim.h"
#include "../../app/app_config.h"

#if(HAL_UART == 1U && SUPPORTED_CHIP == WIN)
#include "stdint.h"
#include "../../ezmDebug/ezmDebug.h"
#include "../../helper/hexdump/hexdump.h"

#define MOD_NAME    "HW_UART"
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
/** @brief definition the callback function pointer
 *
 */
typedef uint8_t(*hw_uart_callback)      (uint8_t eCode, void* pParam);

typedef struct
{
    ezmUart uart_api;
    hw_uart_callback callback;
}HwUart;
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
static uint16_t UartSim_Read                (uint8_t hw_id, uint8_t*buff, uint16_t size);
static uint16_t UartSim_Write               (uint8_t hw_id, uint8_t *buff, uint16_t size);
static void UartSim_RegisterCallback    (uint8_t hw_id, void *call_back);
static void UartSim_UnRegisterCallback  (uint8_t hw_id);

static HwUart hw_uarts[NUM_OF_SUPPORTED_UART] = { 0 };

bool simUart_Init(void)
{
    for (uint8_t i = 0; i < NUM_OF_SUPPORTED_UART; i++)
    {
        hw_uarts[i].uart_api.ezmUart_Receive = UartSim_Read;
        hw_uarts[i].uart_api.ezmUart_Send = UartSim_Write;
        hw_uarts[i].uart_api.ezmUart_RegisterCallback = UartSim_RegisterCallback;
        hw_uarts[i].uart_api.ezmUart_UnregisterCallback = UartSim_UnRegisterCallback;
    }

    UARTPRINT("init complete");
    return true;
}

ezmUart *simUart_GetApi(uint8_t hw_uart_index)
{
    ezmUart* api = NULL;
    if (hw_uart_index < NUM_OF_SUPPORTED_UART)
    {
        api = &hw_uarts[hw_uart_index].uart_api;
    }
    return api;
}
/******************************************************************************
* Function : sum
*//** 
* \b Description:
*
* This function initializes the ring buffer
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* \b Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
static uint16_t UartSim_Read(uint8_t hw_id, uint8_t* buff, uint16_t size)
{
    if (hw_id < NUM_OF_SUPPORTED_UART)
    {
        for (uint16_t i = 0; i < size; i++)
        {
            buff[i] = _getchar_nolock();
        }
    }
    return size;
}

static uint16_t UartSim_Write(uint8_t hw_id, uint8_t* buff, uint16_t size)
{
    if (hw_id < NUM_OF_SUPPORTED_UART)
    {
        for (uint16_t i = 0; i < size; i++)
        {
            putchar(buff[i]);
        }
    }
    return size;
}

static void UartSim_RegisterCallback(uint8_t hw_id, void* call_back)
{
    if (hw_id < NUM_OF_SUPPORTED_UART && call_back != NULL)
    {
        hw_uarts[hw_id].callback = (hw_uart_callback)call_back;
    }
}

static void UartSim_UnRegisterCallback(uint8_t hw_id)
{
    if (hw_id < NUM_OF_SUPPORTED_UART)
    {
        hw_uarts[hw_id].callback = NULL;
    }
}

#endif
/* End of file*/