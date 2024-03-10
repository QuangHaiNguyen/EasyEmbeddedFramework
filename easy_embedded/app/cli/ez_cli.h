/*****************************************************************************
* Filename:         ez_cli.h
* Author:           Hai Nguyen
* Original Date:    10.03.2024
*
* ----------------------------------------------------------------------------
* Contact:          Hai Nguyen
*                   hainguyen.eeit@gmail.com
*
* ----------------------------------------------------------------------------
* License: This file is published under the license described in LICENSE.md
*
*****************************************************************************/

/** @file   ez_cli.h
 *  @author Hai Nguyen
 *  @date   10.03.2024
 *  @brief  Public API for Command Line Interface application
 *
 *  @details 
 */

#ifndef _EZ_CLI_H
#define _EZ_CLI_H

/*****************************************************************************
* Includes
*****************************************************************************/
#if (EZ_CLI_ENABLE == 1)
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

/*****************************************************************************
* Component Preprocessor Macros
*****************************************************************************/
#ifndef CONFIG_NUM_OF_ARGUMENT
#define CONFIG_NUM_OF_ARGUMENT      4
#endif

#ifndef CONFIG_NUM_OF_CMD
#define CONFIG_NUM_OF_CMD      4
#endif

#define CLI_HANDLE_INVALID       CONFIG_NUM_OF_CMD


/*****************************************************************************
* Component Typedefs
*****************************************************************************/

/** @brief CLI notification code
 */
typedef enum
{
    CLI_NC_OK,      /**< Ok code */
    CLI_NC_ERR,     /**< generic error code */
    CLI_NC_BAD_ARG, /**< input argument is wrong */
}CLI_NOTIFY_CODE;

/** @brief command callback function
 */
typedef CLI_NOTIFY_CODE(*CLI_CALLBACK)(const char * pu8Command, void * pValueList);

/** @brief handle of a command
 */
typedef uint8_t CommandHandle;


/*****************************************************************************
* Component Variable Definitions
*****************************************************************************/
/* None */

/*****************************************************************************
* Function Prototypes
*****************************************************************************/
#if 0  /* TODO */
/******************************************************************************
* Function : ezCli_Init
*//** 
* \b Description:
*
* This function initializes the command line interface. It resets the resource and
* start the low level serial interface driver
*
* PRE-CONDITION: has dependency on UART HAL and deriver interface modules.
* Therefore, they must be initiated before calling the cli module 
*
* POST-CONDITION: None
* 
* @param    uart_driver: driver instance of the uart peripheral
*
* @return   true: success
*           false fail
*
* \b Example Example:
* @code
* ezCli_Init();
* @endcode
*
* @see sum
*
*******************************************************************************/
bool ezCli_Init(UartDrvApi* uart_driver);
#endif


/******************************************************************************
* Function : ezCli_RegisterCommand
*//** 
* \b Description:
*
* This function register a new command 
*
* PRE-CONDITION: ezCli_Init() must be called before
*
* POST-CONDITION: None
* 
* @param    *command:     (IN)pointer to the command buffer
* @param    *description: (IN)pointer to the description buffer
* @param    callback:    (IN)pointer to the callback function, where the command is executed
*
* @return   index of the command, or 0xff of fail
*
* \b Example Example:
* @code
* uint8_t u8Index;
* u8Index = ezCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
* @endcode
*
* @see sum
*
*******************************************************************************/
CommandHandle ezCli_RegisterCommand(const char * command,
                                    const char *  description, 
                                    CLI_CALLBACK callback);


/******************************************************************************
* Function : ezCli_AddArgument
*//** 
* \b Description:
*
* This function initializes the ring buffer
*
* PRE-CONDITION: ezCli_Init() and ezCli_RegisterCommand() must be called before
*
* POST-CONDITION: None
* 
* @param    cmd_handle:  (IN)index of the command, which the argument will be added
* @param    *long_arg:   (IN)argument in long form, starting with --
* @param    *short_arg:  (IN)argument in short form, starting with -
* @param    *description:(IN)argument description
*
* @return   True if success, else false
*
* \b Example Example:
* @code
* uint8_t u8Index;
* u8Index = ezCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
* ezCli_AddArgument(u8Index, "--arg1", "-a1", "argument 1");
* @endcode
*
* @see sum
*
*******************************************************************************/
bool ezCli_AddArgument (CommandHandle cmd_handle,
                        const char * long_arg, 
                        const char * short_arg, 
                        const char * description);


#if 0  /* TODO */
/******************************************************************************
* Function : ezCli_Run
*//**
* \b Description:
*
* This function must be call in a loop/or scheduler to run the CLI
*
* PRE-CONDITION: CLI module must be initialized
*
* POST-CONDITION: None
*
* @param    None
*
* @return   None
*
* \b Example Example:
* @code
* while(1)
* {
*     ezCli_Run();
* }
* @endcode
*
* @see sum
*
*******************************************************************************/
void ezCli_Run(void);
#endif


/******************************************************************************
* Function : ezCli_CommandReceivedCallback
*//** 
* \b Description:
*
* This function resets the contents of the metadata at a specific index
*
* PRE-CONDITION: ezCli_Init() and ezCli_RegisterCommand() 
*                ezCli_AddArgument() must be called before
*
* POST-CONDITION: None
* 
* @param    notify_code:         (IN) notification code
* @param    *command_buffer:     (IN) pointer to the buffer storing the command
* @param    command_buff_size:   (IN) size of the buffer storing the command
*
* @return   True if success, else false
*
* \b Example Example:
* @code
* uint8_t u8Index;
* u8Index = ezCli_RegisterCommand("Test_Command", "this is a test command", &TestCommand);
* ezCli_AddArgument(u8Index, "--arg1", "-a1", "argument 1");
* ezCli_CommandReceivedCallback(u8NotifyCode, pu8CommandBuffer,u16Size);
* @endcode
*
*******************************************************************************/
bool ezCli_CommandReceivedCallback(uint8_t notify_code,
                                   char* command_buffer,
                                   uint16_t command_buff_size);


/******************************************************************************
* Function : ezCli_PrintMenu
*//**
* \b Description:
*
* This function checks prints the complete help
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    None
* @return   None
*
*******************************************************************************/
void ezCli_PrintMenu(void);


#if 0  /* TODO */
/******************************************************************************
* Function : ezCli_Printf
*//**
* \b Description:
*
* it is the printf function, but use a different IO
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
*
* @param    fmt: (IN)input string
* @param    ...: (IN)list of arguments
*
* @return   None
*
*******************************************************************************/
void ezCli_Printf(char *fmt, ...);
#endif

#endif /* EZ_CLI_ENABLE == 1 */
#endif /* _EZ_CLI_H */


/* End of file */
