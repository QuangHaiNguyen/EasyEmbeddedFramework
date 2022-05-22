/*******************************************************************************
* Filename:         cli.h
* Author:           Quang Hai Nguyen
* Original Date:    22.05.2022
* Last Update:      22.05.2022
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
* Copyright Quang Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Quang Hai Nguyen 22.05.2022
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
#include "ezmDriver/ezmDriver.h"
#include "hal/uart/uart.h"

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

/** @brief handle of a command
 *
 */
typedef uint8_t CommandHandle;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* Function Prototypes
*******************************************************************************/
bool            ezmCli_Init             (UartDrvApi* uart_driver);
CommandHandle   ezmCli_RegisterCommand  (const char * pu8Command,
                                            const char *  pu8Description, 
                                            CLI_CALLBACK pfnCallback);
bool    ezmCli_AddArgument (CommandHandle u8CommandIndex,
                                const char * pu8LongForm, 
                                const char * pu8ShortForm, 
                                const char * pu8Description);
void    ezmCli_Run(void);
bool    ezmCli_CommandReceivedCallback(uint8_t u8NotifyCode,
                                    char* pu8CommandBuffer,
                                    uint16_t u16CommandBufferSize);
void    ezmCli_PrintMenu(void);
#endif /* CLI */
#endif /* _CLI_H */

/* End of file*/
