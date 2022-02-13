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
#include "string.h"
#include "../ezmKernel/ezmKernel.h"
#include "../ezmDriver/ezmDriver.h"
#include "../hal/uart/uart.h"
#include "../ezmDebug/ezmDebug.h"
#include "../cli/cli.h"
#include <conio.h>
/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef enum
{
    GET_BYTE,
    PROC_CMD,
}CLT_STATE;

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint8_t cli_buffer[1000U] = {0U};
static uint8_t cli_inst_num = 0xFF;
static UartDrvApi* uart_driver = NULL;
static uint8_t CLI_Proccess(void);
static process cli_process;
static CLT_STATE state;
static uint16_t index = 0U;
/******************************************************************************
* Function Definitions
*******************************************************************************/
static CLI_NOTIFY_CODE CommandHandle(const char* pu8Command, void* pValueList);
static uint8_t UartCallbackHandle(uint8_t notify_code, void* param1, void* param2);

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
        is_success = is_success && ezmDriver_GetDriverInstance(UART0_DRIVER, &uart_driver);
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
    static uint8_t ch;

    switch (state)
    {
    case GET_BYTE:
        (void)uart_driver->ezmUart_Receive(&ch, 1U);
        break;
    case PROC_CMD:
        (void)ezmCli_CommandReceivedCallback(0, cli_buffer, sizeof(cli_buffer));
        memset(cli_buffer, 0, sizeof(cli_buffer));
        index = 0U;
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
    char* ch;
    switch ((UART_NOTIFY_CODE)notify_code)
    {
    case UART_TX_COMPLT:
        break;
    case UART_RX_COMPLT:
        ch = (char*)param1;
        if (ch)
        {
            cli_buffer[index] = *ch;
            if (cli_buffer[index] == '\n' || index == sizeof(cli_buffer) || cli_buffer[index] == '\r')
            {
                state = PROC_CMD;
            }
            index++;
        }
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

/* End of file*/