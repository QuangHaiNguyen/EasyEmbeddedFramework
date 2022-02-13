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
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static uint8_t cli_buffer[1000U] = {0U};

/******************************************************************************
* Function Definitions
*******************************************************************************/
static uint8_t CLI_Proccess(void);
static process cli_process;
static CLI_NOTIFY_CODE CommandHandle(const char* pu8Command, void* pValueList);
static uint8_t cli_inst_num = 0xFF;
static ezmUart *uart_driver;

void AppCli_Init(void)
{
    bool is_success = true;
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
    static uint16_t index = 0U;
    static uint8_t ch;

    (void)uart_driver->ezmUart_Receive(CLI_UART, &ch, 1U);
    cli_buffer[index] = ch;
    if (cli_buffer[index] == '\n' || index == sizeof(cli_buffer) || cli_buffer[index] == '\r')
    {
        //(void)getchar();
        (void)ezmCli_CommandReceivedCallback(0, cli_buffer, sizeof(cli_buffer));
        memset(cli_buffer, 0, sizeof(cli_buffer));
        index = 0U;
    }
    else
    {
        index++;
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
/******************************************************************************
* Function : sum
*//** 
* \b Description:
*
* This function initializes the ring buffer
*
* PRE-CONDITION: None
*
* POST-CONDITION: None
* 
* @param    a: (IN)pointer to the ring buffer
* @param    b: (IN)size of the ring buffer
* @return   None
*
* \b Example Example:
* @code
* sum(a, b);
* @endcode
*
* @see sum
*
*******************************************************************************/
int sum(int a, int b)
{
    return a + b;
}

/* End of file*/