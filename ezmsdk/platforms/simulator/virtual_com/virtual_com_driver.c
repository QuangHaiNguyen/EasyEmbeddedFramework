
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
#include <string.h>

#include "utilities/hexdump/hexdump.h"


/******************************************************************************
* Module Preprocessor Macros
*******************************************************************************/

/* Logging module conficts with some macro from win32 api so we hack it here */
#define DEBUG       1U                      /**< debug activation flag */
#define MOD_NAME    "VIRTUAL_COM_HW"        /**< module name */


#if (DEBUG == 1U)
#define PRNT_DBG(fmt, ...)      printf("["MOD_NAME"] " fmt "\n", ##__VA_ARGS__)
#define HEXDUMP(x,y)            ezmHexdump(x,y)
#else
#define PRNT_DBG(fmt, ...)
#define HEXDUMP(x, y)
#endif


/******************************************************************************
* Module Typedefs
*******************************************************************************/
typedef struct
{
    wchar_t      port_name[32];
    HANDLE       driver_h;
    DCB          config;
    COMMTIMEOUTS timeouts;
}HwVirtualComDrv;


/******************************************************************************
* Module Variable Definitions
*******************************************************************************/
static HwVirtualComDrv drv[1] = {0};

/******************************************************************************
* Function Definitions
*******************************************************************************/
static bool VirtualCom_Configure(uint8_t drv_index, char *port_name);
static bool VirtualCom_SendBlocking(uint8_t drv_index, uint8_t *buff, uint32_t buff_size);
static bool VirtualCom_RecvBlocking(uint8_t drv_index, uint8_t* buff, uint32_t buff_size);

/******************************************************************************
* External functions
*******************************************************************************/


bool VirtualCom_Initialization(void)
{
    bool success = true;

    PRNT_DBG("VirtualCom_Initialization()");

    return success;
}

#if 0
VirtualCom_Interface *VirtualCom_GetInterface(void)
{
    VirtualCom_Interface *ret_interface = NULL;

    return ret_interface;
}
#endif

/******************************************************************************
* Internal functions
*******************************************************************************/
static bool VirtualCom_Configure(uint8_t drv_index, char* port_name)
{
    bool            success = true;
    size_t          byte_converted = 0;
    char            helper[32] = { 0 };
    HwVirtualComDrv *hw_drv = NULL;
 
    PRNT_DBG("VirtualCom_Configure(%s)", port_name);

    if (port_name != NULL
        && drv_index < sizeof(drv)/sizeof(HwVirtualComDrv))
    {
        hw_drv = &drv[drv_index];

        /* Convert from char to wchar_t thingy */
        snprintf(helper, sizeof(helper), "\\\\.\\%s", port_name);
        mbstowcs_s(&byte_converted, hw_drv->port_name, 32, helper, 31);



        hw_drv->driver_h = CreateFile((wchar_t*)hw_drv->port_name,    // port name
                                        GENERIC_READ | GENERIC_WRITE,               // Read/Write
                                        0,                                          // No Sharing
                                        NULL,                                       // No Security
                                        OPEN_EXISTING,                              // Open existing port only
                                        0,                                          // Non Overlapped I/O
                                        NULL);

        if (hw_drv->driver_h == INVALID_HANDLE_VALUE)
        {
            success = false;
        }

        if (success)
        {
            hw_drv->config.DCBlength = sizeof(DCB);
            success &= GetCommState(hw_drv->driver_h, &hw_drv->config);
        }

        if (success)
        {
            hw_drv->config.BaudRate = 1500000;      //BaudRate = 9600
            hw_drv->config.ByteSize = 8;            //ByteSize = 8
            hw_drv->config.StopBits = ONESTOPBIT;   //StopBits = 1
            hw_drv->config.Parity = NOPARITY;       //Parity = None

            success &= SetCommState(hw_drv->driver_h, &hw_drv->config);
        }

        if (success)
        {
            hw_drv->timeouts.ReadIntervalTimeout = 50;
            hw_drv->timeouts.ReadTotalTimeoutConstant = 50;
            hw_drv->timeouts.ReadTotalTimeoutMultiplier = 10;
            hw_drv->timeouts.WriteTotalTimeoutConstant = 50;
            hw_drv->timeouts.WriteTotalTimeoutMultiplier = 10;

            success &= SetCommTimeouts(hw_drv->driver_h, &hw_drv->timeouts);
        }
    }
    else
    {
        success = false;
    }

    return success;
}

static bool VirtualCom_SendBlocking(uint8_t drv_index, uint8_t *buff, uint32_t buff_size)
{
    bool            success = true;
    HwVirtualComDrv *hw_drv = NULL;
    uint32_t        num_byte_writen = 0;
    uint32_t        remain_bytes = buff_size;
    uint8_t         *write_ptr = buff;

    PRNT_DBG("VirtualCom_SendBlocking(%lu bytes)", buff_size);

    if (drv_index < sizeof(drv) / sizeof(HwVirtualComDrv)
        && buff != NULL
        && buff_size > 0)
    {
        hw_drv = &drv[drv_index];

        do
        {
            success &= WriteFile(hw_drv->driver_h,
                                    write_ptr,
                                    buff_size,
                                    &num_byte_writen,
                                    NULL);

            remain_bytes -= num_byte_writen;
            write_ptr += num_byte_writen;

            PRNT_DBG("wrote %lu(%lu) bytes", 
                        buff_size - remain_bytes,
                        buff_size);
        } while (success == true && remain_bytes > 0);

        if (!success)
        {
            PRNT_DBG("write fail, [remain bytes = %lu]", remain_bytes);
        }
    }
    else
    {
        success = false;
    }

    return success;
}

static bool VirtualCom_RecvBlocking(uint8_t drv_index, uint8_t* buff, uint32_t buff_size)
{
    bool            success = true;
    HwVirtualComDrv *hw_drv = NULL;
    uint32_t        num_byte_read = 0;
    uint32_t        remain_bytes = buff_size;
    uint8_t         *read_ptr = buff;

    PRNT_DBG("VirtualCom_RecvBlocking(%lu bytes)", buff_size);

    if (drv_index < sizeof(drv) / sizeof(HwVirtualComDrv)
        && buff != NULL
        && buff_size > 0)
    {
        hw_drv = &drv[drv_index];

        do
        {
            success &= ReadFile(hw_drv->driver_h,
                                    read_ptr,
                                    buff_size,
                                    &num_byte_read,
                                    NULL);

            remain_bytes -= num_byte_read;
            read_ptr += num_byte_read;

            PRNT_DBG("read %lu(%lu) bytes",
                        buff_size - remain_bytes,
                        buff_size);

        } while (success == true && remain_bytes > 0);

        if (!success)
        {
            PRNT_DBG("read fail, [remain bytes = %lu]", remain_bytes);
        }
    }
    else
    {
        success = false;
    }

    return success;
}
/* End of file*/

