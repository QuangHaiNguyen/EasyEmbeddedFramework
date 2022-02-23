/*******************************************************************************
* Title                 :   module 
* Filename              :   module.c
* Author                :   Quang Hai Nguyen
* Origin Date           :   21.02.2021
* Version               :   1.0.0
*
* <br><b> - HISTORY OF CHANGES - </b>
*  
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials         </td><td> Description </td></tr>
* <tr><td> 21.02.2021 </td><td> 1.0.0            </td><td> Quang Hai Nguyen </td><td> Interface Created </td></tr>
* </table><br><br>
* <hr>
*
*******************************************************************************/

/** @file  module.c
 *  @brief This is the source template for a module
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "app_cli.h"
#include "app_config.h"

#if(CLI == 1U)
#include "string.h"
#include "../ezmKernel/ezmKernel.h"
#include "../ezmDriver/ezmDriver.h"
#include "../hal/uart/uart.h"
#include "../ezmDebug/ezmDebug.h"
#include "../cli/cli.h"

/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef enum
{
    GET_BYTE,
    WAIT,
    PROC_CMD,
}CLT_STATE;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint8_t 		cli_buffer[256] = {0U};
static uint8_t 		cli_inst_num = 0xFF;
static uint8_t 		one_byte;
static uint16_t 	buff_index = 0U;
static UartDrvApi* 	uart_driver = NULL;
static process 		cli_process;
static CLT_STATE 	state;

/******************************************************************************
* Function Definitions
*******************************************************************************/
static uint8_t 			CLI_Proccess(void);
static CLI_NOTIFY_CODE 	CommandHandle(const char* pu8Command, void* pValueList);
static uint8_t 			UartCallbackHandle(uint8_t notify_code, void* param1, void* param2);

void AppCli_Init(void)
{
    bool is_success = true;
    state = GET_BYTE;
    is_success = ezmKernel_AddProcess(&cli_process, PROC_REPEATED, 1U, CLI_Proccess);

    if (!is_success)
    {
        PRINT_DEBUG("APPCLI", "add cli process error");
    }

    if (is_success)
    {
        is_success = is_success && ezmDriver_GetDriverInstance(UART0_DRIVER, (void*)&uart_driver);
    }

    if (is_success)
    {
        uart_driver->ezmUart_RegisterCallback(UartCallbackHandle);

        cli_inst_num = ezmCli_RegisterCommand("echo", "echo your text", CommandHandle);
        if (0xFF == cli_inst_num)
        {
            PRINT_DEBUG("APPCLI", "add command error");
        }
        else
        {
            is_success = ezmCli_AddArgument(cli_inst_num, "--text", "-t", "text to echo");
        }

        cli_inst_num = ezmCli_RegisterCommand("proc_load", "show kernel load", CommandHandle);
        if (0xFF == cli_inst_num)
        {
            PRINT_DEBUG("APPCLI", "add command error");
            is_success = false;
        }

        cli_inst_num = ezmCli_RegisterCommand("help", "show help", CommandHandle);
        if (0xFF == cli_inst_num)
        {
            PRINT_DEBUG("APPCLI", "add command error");
            is_success = false;
        }
    }
}

static uint8_t CLI_Proccess(void)
{
    switch (state)
    {
    case GET_BYTE:
        (void)uart_driver->ezmUart_Receive(&one_byte, 1U);
#if(SUPPORTED_CHIP != WIN)
        state = WAIT;
#endif
        break;
    case WAIT:
    	break;
    case PROC_CMD:
        (void)ezmCli_CommandReceivedCallback(0, (char*)cli_buffer, sizeof(cli_buffer));
        memset(cli_buffer, 0, sizeof(cli_buffer));
        buff_index = 0U;
        state = GET_BYTE;
        break;
    default:
        break;;
    }
    return 0;
}

static CLI_NOTIFY_CODE CommandHandle(const char* pu8Command, void* pValueList)
{
    uint32_t* params = (uint32_t*)pValueList;
    if (strcmp(pu8Command, "echo") == 0)
    {
        if (params)
        {
            char * echo_text = ((char*)(*params));
            PRINT_DEBUG1("APPCLI", "%s", echo_text);
        }
    }
    else if (strcmp(pu8Command, "proc_load") == 0U)
    {
        PRINT_DEBUG1("APPCLI", "[load = %d]", ezmKernel_GetLoad());
    }
    else if (strcmp(pu8Command, "help") == 0U)
    {
        ezmCli_PrintMenu();
    }
    else
    {
        PRINT_DEBUG("APPCLI","Unknown command");
    }
    return CLI_NC_OK;
}

static uint8_t UartCallbackHandle(uint8_t notify_code, void* param1, void* param2)
{

    switch ((UART_NOTIFY_CODE)notify_code)
    {
    case UART_TX_COMPLT:
        break;
    case UART_RX_COMPLT:
#if(SUPPORTED_CHIP == WIN)
        cli_buffer[buff_index] = *(char*)param1;;
        if (cli_buffer[buff_index] == '\n' || buff_index == sizeof(cli_buffer) || cli_buffer[buff_index] == '\r')
        {
            state = PROC_CMD;
        }
#else
        cli_buffer[buff_index] = one_byte;
        if (cli_buffer[buff_index] == '\n' || buff_index == sizeof(cli_buffer) || cli_buffer[buff_index] == '\r')
        {
            state = PROC_CMD;
        }
        else
        {
            (void)uart_driver->ezmUart_Receive(&one_byte, 1U);
        }
#endif
        buff_index++;
        break;
    case UART_BUFF_FULL:
        break;
    case UART_UNSUPPORTED:
        break;
    default:
        break;
    }
    return 0U;
}
#endif
/* End of file*/
