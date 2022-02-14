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


#ifndef _APP_CLI_H
#define _APP_CLI_H

/*******************************************************************************
* Includes
*******************************************************************************/


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


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/


/******************************************************************************
* Function Prototypes
*******************************************************************************/
void AppCli_Init(void);
int sum(int a, int b); /*Short description of function*/

#endif /* _APP_CLI_H */

/* End of file*/