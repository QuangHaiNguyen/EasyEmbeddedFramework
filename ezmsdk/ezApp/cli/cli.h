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
#include "ezApp/ezSdk_config.h"

#if (CONFIG_CLI == 1U)
#include "stdint.h"
#include "stdbool.h"
#include "ezApp/ezmDriver/driver.h"
#include "ezHal/uart/uart.h"

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define CLI_HANDLE_INVALID       CONFIG_NUM_OF_CMD

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
CommandHandle   ezmCli_RegisterCommand  (const char * command,
                                            const char *  description, 
                                            CLI_CALLBACK callback);
bool    ezmCli_AddArgument (CommandHandle cmd_handle,
                                const char * long_arg, 
                                const char * short_arg, 
                                const char * description);
void    ezmCli_Run(void);
bool    ezmCli_CommandReceivedCallback(uint8_t notify_code,
                                        char* command_buffer,
                                        uint16_t command_buff_size);
void    ezmCli_PrintMenu(void);
void    ezmCli_Printf(char *fmt, ...);
#endif /* CLI */
#endif /* _CLI_H */

/* End of file*/
