/*******************************************************************************
* Title                 :   hexdump.h
* Filename              :   hexdump.h
* Author                :   Quang Hai Nguyen
* Origin Date           :   16.05.2021
* Version               :   1.0.0
*
*******************************************************************************/

/*************** INTERFACE CHANGE LIST *****************************************
*
*  Date         Version     Author              Description 
*  16.05.2021   1.0.0       Quang Hai Nguyen    Interface Created.
*
*******************************************************************************/
/** @file   hexdump.h
 *  @brief  Contains public functions of HUxdump module
 */

#ifndef _HEXDUMP_H
#define _HEXDUMP_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "stdint.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef enum
{
    SHOW_ASCII = 0U,
    /**< hexdump shows ascii value*/
    NOT_SHOW_ASCII,
    /**< hexdump does not show ascii value*/
}ENUM_SHOW_ASCII;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/

void ezmHexdump( void * pAddress, uint16_t u16Size, ENUM_SHOW_ASCII eShowAscii); /* This function display the data as hex and ascii */
#endif /* _HEXDUMP_H */

/* End of file*/
