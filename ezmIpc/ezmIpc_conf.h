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
/** @file   module.h
 *  @brief  Header template for a module
 */


#ifndef _IPC_CONFIG_H
#define _IPC_CONFIG_H

/*******************************************************************************
* Includes
*******************************************************************************/
//#include "../app/app_config.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define MODULE_NAME         1U
/**< turn on/off module name in string*/


/******************************************************************************
* Module Typedefs
*******************************************************************************/
uint8_t au8RegisteredModule[] = 
{
    0x01,
    0x02,
};


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
int sum(int a, int b); /*Short description of function*/

#endif /* _IPC_CONFIG_H */

/* End of file*/
