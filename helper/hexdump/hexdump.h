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
#include "stdbool.h"
#include "../../app/app_config.h"

#if (DEBUG == 1)
#include "../../ezmDebug/ezmDebug.h"
#else
#error DEBUG module must be activated
#endif

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
/* None */

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/

void ezmHexdump( void * pAddress, uint16_t u16Size, bool bIsPrintAscii); /* This function display the data as hex and ascii */

#endif /* _HEXDUMP_H */

/* End of file*/
