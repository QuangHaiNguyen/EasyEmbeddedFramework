/*******************************************************************************
* Title                 :   module
* Filename              :   module.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  21.02.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   uart.h
 *  @brief  Header of HAL UART modulec
 */


#ifndef _HAL_UART_H
#define _HAL_UART_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "ezApp/ezSdk_config.h"

#if (CONFIG_HAL_UART == 1U)

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */


/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of the notification code
 *  
 */
typedef enum
{
    UART_TX_COMPLT,     /**< UART transmit completed */
    UART_RX_COMPLT,     /**< UART receive completed */
    UART_BUFF_FULL,     /**< UART buffer is full */
    UART_UNSUPPORTED,   /**< UART unsupported callback */

    /**More code can be added from here.
     * Do not delete the one above for compatibility
     */

}UART_NOTIFY_CODE;


/** @brief Number of stop bit
 *
 */
typedef enum
{
    ONE_BIT,            /**< 1 stop bit */
    ONE_AND_HALF_BIT,   /**< 1.5 stop bit */
    TWO_BITS,           /**< 2 stop bits */
}UART_STOP_BIT;


/** @brief Parity value
 *
 */
typedef enum
{
    NONE,               /**< */
    ODD,                /**< */
    EVEN,               /**< */
    MARK,               /**< */
    SPACE,              /**< */
}UART_PARITY;


typedef struct
{
    char            *port_name;
    uint8_t         byte_size;
    uint32_t        baudrate;
    UART_PARITY     parity;
    UART_STOP_BIT   stop_bit;
}UartConfiguration;


/** @brief definition the callback function pointer 
 *  
 */
typedef uint8_t (*UART_CALLBACK)(uint8_t eCode, void *param1);


/** @brief definition of api set for UART
 *  
 */
typedef struct
{
    uint16_t(*ezmUart_Send)             (uint8_t *au8Buffer, uint32_t u16Size);
    uint16_t(*ezmUart_Receive)          (uint8_t *au8Buffer, uint32_t u16Size);
    uint16_t(*ezmUart_SendBlocking)     (uint8_t *au8Buffer, uint32_t u16Size);
    uint16_t(*ezmUart_ReceiveBlocking)  (uint8_t *au8Buffer, uint32_t u16Size);
    bool(*ezmUart_Configure)            (UartConfiguration *config);
    void(*ezmUart_RegisterCallback)     (UART_CALLBACK pfnCallback);
    void(*ezmUart_UnregisterCallback)   (void);
}UartDrvApi;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void *ezHal_Uart_LinkCliDriv(void);

#if (CONFIG_VIRTUAL_COM == 1U)
void *ezHal_VirtualCom_LinkDriv(void);
#endif /* CONFIG_VIRTUAL_COM */

#endif /* CONFIG_HAL_UART == 1U */
#endif /* _HAL_UART_H */

/* End of file*/
