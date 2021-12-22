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
#ifndef UNIT_TEST_ENABLE
#define UNIT_TEST_ENABLE     1U /**< Activate helper function for testing, must be deactivated in production*/
#endif

/******************************************************************************
* Module Typedefs
*******************************************************************************/

typedef enum
{
    CLI_NC_OK,
    CLI_NC_ERR,
    CLI_NC_BAD_ARG,
}CLI_NOTIFY_CODE;

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

typedef CLI_NOTIFY_CODE(*CLI_CALLBACK)(const char * pu8Command, void * pValueList);

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
#if(UNIT_TEST_ENABLE == 1U)
extern uint8_t  au8CommandBuffer[CLI_BUFF_SIZE];
#endif /* UNIT_TEST_ENABLE */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void    ezmCli_Init             (void);
uint8_t ezmCli_RegisterCommand  (const char * pu8Command, 
                                    const char *  pu8Description, 
                                    CLI_CALLBACK pfnCallback);

bool    ezmCli_AddArgument      (uint8_t u8CommandIndex, 
                                    const char * pu8LongForm, 
                                    const char * pu8ShortForm, 
                                    const char * pu8Description);

bool    ezmCli_CommandReceivedCallback(uint8_t u8NotifyCode, void * pu32Param1, void * pu32Param2);
#endif /* CLI */
#endif /* _CLI_H */

/* End of file*/
