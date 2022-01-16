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
#include "ezmDebug/config.h"
#include "ezmDebug/ezmDebug.h"
#include "helper/hexdump/hexdump.h"

#if (SUPPORTED_CHIP == ESP32)
#include "esp_uart/esp_uart.h"
#endif /* SUPPORTED_CHIP == ESP32 */

#if (MODULE_DEBUG == 1U) && (UART_DEBUG == 1U)
    #define UARTPRINT1(a)                       PRINT_DEBUG1(a)               
    #define UARTPRINT2(a,b)                     PRINT_DEBUG2(a,b)             
    #define UARTPRINT3(a,b,c)                   PRINT_DEBUG3(a,b,c) 
    #define UARTHEXDUMP(a,b,c)                  ezmHexdump(a, b, c)
#else 
    #define UARTPRINT1(a)           
    #define UARTPRINT2(a,b)           
    #define UARTPRINT3(a,b,c)
    #define UARTHEXDUMP(a,b,c)
#endif

#define UART_NO_OWNER           0xFFU
/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *
 */
typedef struct
{
    uint8_t u8OwnerId;          /**< The id of the module, which is using the uart*/
    ezmUart stPublicApi;        /**< Holding the Uart API*/
}ezmUartMetaData;


ezmUartMetaData aszSupportedUart[NUM_OF_SUPPORTED_UART];


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
static void ezmUart_Reset(void);
static bool ezmUart_IsInstanceBusy(uint8_t u8Index);

/********************** Public functions **************************************/

/******************************************************************************
* Function : ezmUart_Init
*//** 
* \b Description:
*
* This function initializes the HAL uart. It will also initializes the target's uart
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    None
* @return   true if success, else false
*
* \b Example Example:
* @code
* (void)ezmUart_Init();
* @endcode
*
*******************************************************************************/
bool ezmUart_Init(void)
{
    bool bResult = true;
    ezmUart_Reset();

    /* Calling the target uart initialization function deppend on the supported hardware*/
#if (SUPPORTED_CHIP == ESP32)
    bResult = bResult & espUart_Init(CLI_UART, &aszSupportedUart[CLI_UART].stPublicApi);
#endif /* SUPPORTED_CHIP */
    
    if(bResult)
    {
        UARTPRINT1("HAL UART init success");    
    }
    else
    {
        UARTPRINT1("HAL UART init fail");    
    }
    return bResult;
}

/******************************************************************************
* Function : ezmUart_GetInstance
*//** 
* \b Description:
*
* This function returns a free hardware uart instance
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    u8OwnerId: Id of the module, which use the uart, must be a unique number
* @param    u8UartIndex: the index of the uart, which we want to use.
*                        See app_config.h for availalbe hardware uart
*
* @return   pointer the ezmUart structure, or NULL if fails
*
* \b Example Example:
* @code
* ezmUart * pstUart0 = ezmUart_GetInstance(0x10, 0x00);
* @endcode
*
*******************************************************************************/
ezmUart * ezmUart_GetInstance (uint8_t u8OwnerId, uint8_t u8UartIndex)
{
    ezmUart * pstHardwareInstace = NULL;
    if(ezmUart_IsInstanceBusy(u8UartIndex) == false)
    {
        aszSupportedUart[u8UartIndex].u8OwnerId = u8OwnerId;
        pstHardwareInstace = &aszSupportedUart[u8UartIndex].stPublicApi;
    }

    return pstHardwareInstace;
}

/******************************************************************************
* Function : ezmUart_ReleaseInstace
*//** 
* \b Description:
*
* This function release an occupied hardware uart instance
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    u8UartIndex: the index of the uart, which we want to use.
*                        See app_config.h for availalbe hardware uart
*
* @return   None
*
* \b Example Example:
* @code
* ezmUart_ReleaseInstace(0x00);
* @endcode
*
*******************************************************************************/
void ezmUart_ReleaseInstace (uint8_t u8UartIndex)
{
    if( u8UartIndex < NUM_OF_SUPPORTED_UART)
    {
        aszSupportedUart[u8UartIndex].u8OwnerId = UART_NO_OWNER;
    }
}

/********************** Private functions **************************************/

/******************************************************************************
* Function : ezmUart_IsInstanceBusy
*//** 
* \b Description:
*
* This function check if the uart instance is busy (occupied) by a module
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    u8Index: the index of the uart
*
* @return   true if busy, else false
*
* \b Example Example:
* @code
* if(ezmUart_IsInstanceBusy(0x00))
* {
*       printf("busy\n");   
* }
* @endcode
*
*******************************************************************************/
static bool ezmUart_IsInstanceBusy(uint8_t u8Index)
{
    bool bIsBusy = true;
    if(u8Index < NUM_OF_SUPPORTED_UART &&
        aszSupportedUart[u8Index].u8OwnerId == UART_NO_OWNER)
    {
        bIsBusy = false;
    }

    return bIsBusy;
}

/******************************************************************************
* Function : ezmUart_Reset
*//** 
* \b Description:
*
* This function resets all uart intances
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   None
*
* \b Example Example:
* @code
* ezmUart_Reset();
* @endcode
*
*******************************************************************************/
static void ezmUart_Reset(void)
{
    for(uint8_t i = 0; i < NUM_OF_SUPPORTED_UART; i++)
    {
        aszSupportedUart[i].u8OwnerId = UART_NO_OWNER;
    }
}


#endif /* NUM_OF_SUPPORTED_UART > 0U */
/* End of file*/