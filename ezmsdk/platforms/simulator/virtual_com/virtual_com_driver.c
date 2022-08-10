
/*******************************************************************************
* Filename:         virtual_com_driver.c
* Author:           Hai Nguyen
* Original Date:    07.08.2022
* Last Update:      07.08.2022
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
* Copyright Hai Nguyen - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Hai Nguyen 07.08.2022
*
*******************************************************************************/

/** @file   virtual_com_driver.c
 *  @author Hai Nguyen
 *  @date   07.08.2022
 *  @brief  This is the source for a module
 *  
 *  @details
 * 
 */

/******************************************************************************
* Includes
*******************************************************************************/
#include "virtual_com_driver.h"
#include <Windows.h>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG_LVL   LVL_TRACE               /**< logging level */
#define MOD_NAME    "VIRTUAL_COM_HW"        /**< module name */

#include "utilities/logging/logging.h"

/*the rest of include go here*/

/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/
#define A_MACRO     1   /**< a macro*/

/******************************************************************************
* Module Typedefs
*******************************************************************************/
/* None */

/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static char         *port_no = "\\\\.\\COM15";
static wchar_t      used_port[32] = { 0 };
static HANDLE       driver_h = 0;
static DCB          config = {0};
static COMMTIMEOUTS timeouts = { 0 };
static DWORD        byte_writen = 0;
static const char* msg = "hello world\n";

/******************************************************************************
* Function Definitions
*******************************************************************************/
/* None */

/******************************************************************************
* External functions
*******************************************************************************/


bool VirtualCom_Initialization(void)
{
    bool success = true;
    size_t byte_converted;

    printf("VirtualCom_Initialization()\n");

    /* Convert from char to wchar_t thingy */
    mbstowcs_s(&byte_converted, used_port, 32, port_no, 31);

    driver_h = CreateFile((wchar_t *)used_port,             // port name
                            GENERIC_READ | GENERIC_WRITE,   // Read/Write
                            0,                              // No Sharing
                            NULL,                           // No Security
                            OPEN_EXISTING,                  // Open existing port only
                            0,                              // Non Overlapped I/O
                            NULL);

    if (driver_h == INVALID_HANDLE_VALUE)
    {
        success = false;
    }

    if (success)
    {
        config.DCBlength = sizeof(config);
        success &= GetCommState(driver_h, &config);
    }

    if (success)
    {
        config.BaudRate = 1500000;      //BaudRate = 9600
        config.ByteSize = 8;            //ByteSize = 8
        config.StopBits = ONESTOPBIT;   //StopBits = 1
        config.Parity = NOPARITY;       //Parity = None

        success &= SetCommState(driver_h, &config);
    }

    if (success)
    {
        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;

        success &= SetCommTimeouts(driver_h, &timeouts);
    }

    if (success)
    {
        //Writing data to Serial Port
        success &= WriteFile(driver_h,
                             msg,
                             strlen(msg),
                             &byte_writen,
                             NULL);

        printf("wrote %d bytes\n", byte_writen);
    }

    uint32_t byte_count = 0;
    char one_byte = 0;
    char read_data[32] = { 0 };
    DWORD byte_read = 0;

    if (success)
    {
        do
        {
            success = ReadFile(driver_h, &one_byte, sizeof(one_byte), &byte_read, NULL);
            read_data[byte_count] = one_byte;
            ++byte_count;
        } while (byte_read > 0 && success == true);
    }

    printf("echo: %s\n", read_data);

    return success;
}


VirtualCom_Interface *VirtualCom_GetInterface(void)
{
    VirtualCom_Interface *ret_interface = NULL;

    return ret_interface;
}


/******************************************************************************
* Internal functions
*******************************************************************************/

/* End of file*/

