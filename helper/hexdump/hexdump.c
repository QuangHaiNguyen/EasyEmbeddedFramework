/*******************************************************************************
* Title                 :   hexdump 
* Filename              :   hexdump.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   16.05.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 16.05.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  hexdump.c
 *  @brief This is the source code of hexdump api
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "hexdump.h"
#include "stdio.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function : Hexdump
*//** 
* \b Description:
*
* This function display the data as hex and ascii
*
* PRE-CONDITION: Must be sure that stdio is availalbe since this function required printf
*
* POST-CONDITION: None
* 
* @param    pAddress: (IN)pointer to the ring buffer
* @param    u16Size: (IN)size of the ring buffer
* @param    bIsPrintAscii: (IN) flag to activate print data also as ascii
* @return   None
*
* \b Example Example:
* @code
* uint8_t au8Buffer[4] = {0x01, 0x02, 0x03, 0x04};
* Hexdump((void *)&au8Buffer, 4, true);
* @endcode
*
* @see sum
*
*******************************************************************************/
void ezmHexdump( void * pAddress, uint16_t u16Size, bool bIsPrintAscii)
{
    void * ulStartingAddress = pAddress;

    printf("\n\nAddress: %p - size: %d\n", pAddress, u16Size);
    printf("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
    while(ulStartingAddress < pAddress + u16Size)
    {
        for(uint8_t i = 0; i < 16; i = i + 1)
        {
            if((uint8_t *)(ulStartingAddress + i) - (uint8_t *) pAddress <= u16Size)
            {
                PRINTF_NO_NL("%02x ", *(uint8_t *)(ulStartingAddress + i));
            }
            else
            {
                PRINTF_NO_NL("%s","   ");
            }
        }

        if(bIsPrintAscii)
        {
            PRINTF_NO_NL("%s", "| ");
            for(uint8_t i = 0; i < 16; i = i + 1)
            {
                /* Only print readable ascii character */
                if( (*(uint8_t *)(ulStartingAddress + i) >= 33U && *(uint8_t *)(ulStartingAddress + i) <= 126U) )
                {
                    PRINTF_NO_NL("%c", *(char *)(ulStartingAddress + i));
                }
                else
                {
                    PRINTF_NO_NL("%s",".");
                }
            }
        }
        PRINT1("");
        ulStartingAddress = ulStartingAddress + 16;
    }
    PRINT1("");
}

/* End of file*/