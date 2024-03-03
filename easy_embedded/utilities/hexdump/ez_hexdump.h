/*****************************************************************************
* Filename:         ez_hexdump.h
* Author:           Hai Nguyen
* Original Date:    18.02.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_hexdump.h
 *  @author Hai Nguyen
 *  @date   18.02.2024
 *  @brief  Public functions of hexdump component
 *
 *  @details
 * 
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

/*****************************************************************************
* Function: ezHexdump
*//** 
* @brief Print data at a certain address as hex value
*
* @details
*
* @param    address: (IN)Starting address where data will be printed
* @param    size: (IN)size of of data, in byte
* @return   None
*
* @pre None
* @post None
*
* @code
* char test[3] = {'a', 'b', 'c'};
* ezHexdump(test, 3);
* @endcode
*
*****************************************************************************/
void ezHexdump(void *address, uint16_t size);
#endif /* _HEXDUMP_H */

/* End of file*/
