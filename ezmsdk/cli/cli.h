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


#ifndef _CLI_H
#define _CLI_H

/*******************************************************************************
* Includes
*******************************************************************************/
#include "../app/app_config.h"
#if (CLI == 1U)
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1
/**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief definition of a new type
 *  
 */

typedef struct
{
    int a;
    /**< an integer */
    int b;
    /**< an integer */
}aType;

typedef uint32_t (*CLI_CALLBACK)(void * pArgumentList, void * pValueList);

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void    ezmCli_Init(void);
uint8_t ezmCli_RegisterCommand(const char * pu8Command, const char * pu8Description, CLI_CALLBACK pfnCallback);
bool    ezmCli_AddArgument(const char * pu8LongForm, const char * pu8ShortForm, const char * pu8Description);

#endif /* CLI */
#endif /* _CLI_H */

/* End of file*/
