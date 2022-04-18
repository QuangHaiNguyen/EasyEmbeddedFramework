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
#include "app/app_config.h"

#if (CLI == 1U)
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define CLI_HANDLE_INVALID       NUM_OF_CMD

/******************************************************************************
* Module Typedefs
*******************************************************************************/

/** @brief CLI notification code
 *
 */
typedef enum
{
    CLI_NC_OK,      /**< Ok code */
    CLI_NC_ERR,     /**< generic error code */
    CLI_NC_BAD_ARG, /**< input argument is wrong */
}CLI_NOTIFY_CODE;


/** @brief command callback function
 *
 */
typedef CLI_NOTIFY_CODE(*CLI_CALLBACK)(const char * pu8Command, void * pValueList);
typedef uint8_t CommandHandle;
/******************************************************************************
* Module Variable Definitions
*******************************************************************************/

/******************************************************************************
* Function Prototypes
*******************************************************************************/
void            ezmCli_Init             (void);
CommandHandle   ezmCli_RegisterCommand  (const char * pu8Command,
                                            const char *  pu8Description, 
                                            CLI_CALLBACK pfnCallback);

bool    ezmCli_AddArgument      (CommandHandle u8CommandIndex,
                                    const char * pu8LongForm, 
                                    const char * pu8ShortForm, 
                                    const char * pu8Description);

bool    ezmCli_CommandReceivedCallback(uint8_t u8NotifyCode, char* pu8CommandBuffer, uint16_t u16CommandBufferSize);
void    ezmCli_PrintMenu(void);
#endif /* CLI */
#endif /* _CLI_H */

/* End of file*/
