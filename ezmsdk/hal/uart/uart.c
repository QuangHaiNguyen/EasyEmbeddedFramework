/*******************************************************************************
* Title                 :   uart 
* Filename              :   uart.c
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

/** @file  uart.c
 *  @brief This is the source template for a uart
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "uart.h"

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef struct HAL_UART
{
    uint8_t u8UartHandle;
    void(*ezmUartRead)(uint8_t * pu8Buffer, uint16_t u16BuffSize);
    void(*ezmUartWrite)(uint8_t * pu8Buffer, uint16_t u16BuffSize);
    void(*ezmUartInit)(void);
    void(*ezmUartDeInit)(void);
    void(*ezmUartReset)(void);
}HAL_UART;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

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
int sum(int a, int b)
{
    return a + b;
}

/* End of file*/