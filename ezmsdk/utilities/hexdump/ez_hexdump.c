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

#if(EZ_HEXDUMP == 1U)
#include "ez_hexdump.h"
#include "logging/ez_logging.h"
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
static void PrintAscii(char c);

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
* @param    eShowAscii: (IN) flag to activate print data also as ascii
* @return   None
*
* \b Example Example:
* @code
* uint8_t au8Buffer[4] = {0x01, 0x02, 0x03, 0x04};
* Hexdump((void *)&au8Buffer, 4, true);
* @endcode
*
*******************************************************************************/
void ezmHexdump( void * pAddress, uint16_t u16Size)
{
    void * ulStartingAddress = pAddress;

    dbg_print("\n\nAddress: %p - size: %d\n", pAddress, u16Size);
    dbg_print("00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");
    while((uint8_t*)ulStartingAddress < (uint8_t*)pAddress + u16Size)
    {
        for(uint8_t i = 0; i < 16; i = i + 1)
        {
            if((uint8_t *)((uint8_t*)ulStartingAddress + i) - (uint8_t *) pAddress < u16Size)
            {
                dbg_print("%02x ", *((uint8_t*)ulStartingAddress + i));
            }
            else
            {
                dbg_print("%s","   ");
            }
        }

        dbg_print("%s", "| ");
        for(uint8_t i = 0; i < 16; i = i + 1)
        {
            if ((uint8_t*)((uint8_t*)ulStartingAddress + i) - (uint8_t*)pAddress < u16Size)
            {
                PrintAscii(*(char*)((uint8_t*)ulStartingAddress + i));
            }
        }

        dbg_print("\n");
        ulStartingAddress = (uint8_t*)ulStartingAddress + 16;
    }
    dbg_print("");
}

static void PrintAscii(char c)
{
    /* Only print readable ascii character */
    if (c >= 33U && c <= 126U)
    {
        dbg_print("%c", c);
    }
    else
    {
        dbg_print("%s", ".");
    }
}
#endif /* CONFIG_HELPER_HEXDUMP */
/* End of file*/