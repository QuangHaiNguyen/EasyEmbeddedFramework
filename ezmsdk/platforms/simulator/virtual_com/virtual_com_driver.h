
/*******************************************************************************
* Filename:         virtual_com_driver.h
* Author:           Hai Nguyen
* Original Date:    07.08.2022
* Last Update:      07.08.2022
*
* -----------------------------------------------------------------------------
* Comany:           Easy Embedded
*                   Address Line 1
*                   Address Line 2
*
* -----------------------------------------------------------------------------
* Contact:          Easy Embedded
*                   hainguyen.ezm@gmail.com
*
* -----------------------------------------------------------------------------
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 07.08.2022
*
*******************************************************************************/

/** @file   virtual_com_driver.h
 *  @author Hai Nguyen
 *  @date   07.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

#ifndef _VIRTUAL_COM_DRIVER_H
#define _VIRTUAL_COM_DRIVER_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "app/app_config.h"

#if (CONFIG_VIRTUAL_COM == 1U)
#include <stdbool.h>
#include <stdint.h>

#include "hal/uart/uart.h"

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


/******************************************************************************
* Function : VirtualCom_Initialization
*//**
* @Description:
*
* @param
* @return
*
* @Example Example:
* @code
*
* @endcode
*
*******************************************************************************/
bool VirtualCom_Initialization(void);




/******************************************************************************
* Function : VirtualCom_Initialization
*//**
* @Description:
*
* @param
* @return
*
* @Example Example:
* @code
*
* @endcode
*
*******************************************************************************/
UartDrvApi *VirtualCom_GetInterface(void);


#endif /* CONFIG_VIRTUAL_COM */
#endif /* _VIRTUAL_COM_DRIVER_H */

/* End of file */

